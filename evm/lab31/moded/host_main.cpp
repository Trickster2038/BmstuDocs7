#include <iostream>
#include <stdio.h>
#include <stdexcept>
#include <iomanip>
#ifdef _WINDOWS
#include <io.h>
#else
#include <unistd.h>
#endif


#include "experimental/xrt_device.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_bo.h"
#include "experimental/xrt_ini.h"

#include "gpc_defs.h"
#include "leonhardx64_xrt.h"
#include "gpc_handlers.h"

#define BURST 256
#define TESTSET_SIZE 10

uint64_t orig_values[256];

union uint64 {
    uint64_t 	u64;
    uint32_t 	u32[2];
    uint16_t 	u16[4];
    uint8_t 	u8[8];
};

uint64_t rand64() {
    uint64 tmp;
    tmp.u32[0] =  rand();
    tmp.u32[1] =  rand();
    return tmp.u64;
}

static void usage()
{
	std::cout << "usage: <xclbin> <sw_kernel>\n\n";
}

int main(int argc, char** argv)
{

	unsigned int cores_count = 0;
	float LNH_CLOCKS_PER_SEC;

	__foreach_core(group, core) cores_count++;

	//Assign xclbin
	if (argc < 3) {
		usage();
		throw std::runtime_error("FAILED_TEST\nNo xclbin specified");
	}

	//Open device #0
	leonhardx64 lnh_inst = leonhardx64(0,argv[1]);
	__foreach_core(group, core)
	{
		lnh_inst.load_sw_kernel(argv[2], group, core);
	}


	// /*
	//  *
	//  * Запись множества из BURST key-value 
	//  *
	//  */


	//Выделение памяти под буферы gpc2host и host2gpc для каждого ядра и группы
	uint64_t *host2gpc_buffer[LNH_GROUPS_COUNT][LNH_MAX_CORES_IN_GROUP];
	__foreach_core(group, core)
	{
		host2gpc_buffer[group][core] = (uint64_t*) malloc(2*BURST*sizeof(uint64_t));
	}
	uint64_t *gpc2host_buffer[LNH_GROUPS_COUNT][LNH_MAX_CORES_IN_GROUP];
	__foreach_core(group, core)
	{
		gpc2host_buffer[group][core] = (uint64_t*) malloc(2*BURST*sizeof(uint64_t));
	}

	//Создание массива ключей и значений для записи в lnh64
	__foreach_core(group, core)
	{
		for (int i=0;i<BURST;i++) {
			//Первый элемент массива uint64_t - key
			host2gpc_buffer[group][core][2*i] = i;
			//Второй uint64_t - value
			orig_values[i] = rand64() % 128;
			host2gpc_buffer[group][core][2*i+1] = orig_values[i];
		}
	}


	//Запуск обработчика insert_burst
	__foreach_core(group, core) {
		lnh_inst.gpc[group][core]->start_async(__event__(insert_burst));
	}

	//DMA запись массива host2gpc_buffer в глобальную память
	__foreach_core(group, core) {
		lnh_inst.gpc[group][core]->buf_write(BURST*2*sizeof(uint64_t),(char*)host2gpc_buffer[group][core]);
	}

	//Ожидание завершения DMA
	__foreach_core(group, core) {
		lnh_inst.gpc[group][core]->buf_write_join();
	}

	//Передать количество key-value
	__foreach_core(group, core) {
		lnh_inst.gpc[group][core]->mq_send(BURST);
	}

	unsigned int value[LNH_GROUPS_COUNT][LNH_MAX_CORES_IN_GROUP];

	bool error = false;
	//Проверка целостности данных
	__foreach_core(group, core) {
		for (int i=0; i<TESTSET_SIZE; i++) {
			int key = rand64() % 256;
			lnh_inst.gpc[group][core]->start_async(__event__(search_burst)); //Запустить обработчик
			lnh_inst.gpc[group][core]->mq_send(key); // Запрос
        		value[group][core] = lnh_inst.gpc[group][core]->mq_receive(); // Ответ
			if (value[group][core] != orig_values[key]) {
				error = true;
				printf("[x] ip: 195.19.32.%d int: %d orig_int: %d \n", key, value[group][core], orig_values[key]);
			} else {
				printf("[v] ip: 195.19.32.%d int: %d orig_int: %d \n", key, value[group][core], orig_values[key]);
			}
		}
	}


	__foreach_core(group, core) {
		free(host2gpc_buffer[group][core]);
		free(gpc2host_buffer[group][core]);
	}

	if (!error)
		printf("Тест пройден успешно!\n");
	else
		printf("Тест завершен с ошибкой!\n");


	return 0;
}

