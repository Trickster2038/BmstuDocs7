#include <stdio.h> 

int main() { 
unsigned long long accum = 3; // unsigned 64-bit integer 
unsigned long long i = 0; 
while(i++ < 1000000000) { 
    accum *= 991; 
    accum = accum ^ (accum << 5); 
    accum *= 777; 
    accum = accum ^ (accum >> 8); 
    accum *= 1424; 
    accum = accum ^ (accum << 3); 
    if(i % 50000000 == 0){
        printf("%llu: %llu\n", i, accum); 
    }
} 
printf("%llu\n", accum); 
return 0; 
}

