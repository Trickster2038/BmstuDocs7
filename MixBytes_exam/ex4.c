/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>

using namespace std;

float pool[3] = {1100.0, 900.0, 1000.0};

// initial USDA, USDB, USDC balances of pool float pool[3] = {1100.0, 900.0, 1000.0}; 
void swap(float diffs[3]) { 
// performs swaps between three tokens: USDA, USDB, USDC, // keeping invariant (constant product) // automatically increasing/decreasing relative prices between tokens // constant product = pool[0] * pool[1] * pool[2] (multiple of token balances) // and must be greater or equal to previous value when performing a swap in the pool // // e.g. diff: [+50.0, -10.0, -40.0] means // "pay" 50 USDA + "withdraw" 10.0 USDB + "withdraw" 40.0 USDC /1 swap will be performed if constant product holds: // (USDA * USDB * USDC still the same or increases) 
for (int i = 0; i < 3; i++) { 
    if (diffs[i] < 0 && abs(diffs[i]) > pool[i]) { 
        printf("Swap failed (not enough tokens in one of pools)\n"); 
        return; 
    } 
} 

if ((pool[0] + diffs[0]) * (pool[1] + diffs[1]) * (pool[2] + diffs[2]) < pool[0] * pool[1] * pool[2]) { 
    printf("Swap failed(constant product)\n"); 
    return; 
} 

pool[0] = pool[0] + diffs[0]; pool[1] = pool[1] + diffs[1]; pool[2] = pool[2] + diffs[2]; 
printf("Swap ok, new pool: (%f, %f, %f\n", pool[0], pool[1], pool[2]); 
return; 
}
//You have a swap function, operating with the pool with three balances of tokens: USDA, USDB, USDC (1100.0 USDA, 900.0 USDB, 1000.0 USDC). You plan to receive exactly 300.0 USDC and spend exactly 20.0 USDA. What is the minimal integer(!) USDB amount to perform a swap (select correct minimal diff)? 

float diffs[3] = {20.0, 362.0, -300.0};
int main()
{
    cout<<"Hello World\n";
    swap(diffs);

    return 0;
}

