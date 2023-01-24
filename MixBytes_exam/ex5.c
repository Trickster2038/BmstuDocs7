/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h> 
struct Cypher { char a; char b; char c; char d; char e; char f; char g; char h; }; 
const unsigned int LEN = 8; // length of struct in bytes 

int main() { 
    struct Cypher s = { 'a', 'b','c','d','e','d','c','b'}; 
    char *ptr = (char *) &s; // set pointer ptr to the first byte of struct 
    char buf; 
    for(long long unsigned int j = 0; j < 18446744073709548; j++) { 
        buf = *(ptr + LEN - 1); 
        for(unsigned int i = LEN - 1; i > 0; i--) { 
            *(ptr + i) = *(ptr + i - 1); 
        }
        
        *ptr = buf; 
        
        if(j % 100000000 == 0){
            printf("j: %lld\n", j);
        }
        
        if(j < 100){
            printf("%lld %c %c %c %c %c %c %c %c\n", j, s.a, s.b, s.c, s.d, s.e, s.f, s.g, s.h); 
        }
        
    } 
    printf("%c %c %c %c %c %c %c %c\n", s.a, s.b, s.c, s.d, s.e, s.f, s.g, s.h); 
    return 0;
}


