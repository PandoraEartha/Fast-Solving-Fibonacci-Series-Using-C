//  fibonacci
//  Created by Pandora on 2023/4/11.

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
//#include "/usr/local/gmp.h"
#include "gmp.h"

#define ROW 2
#define COL 2

// USE SQUARE MATRIX ONLY
// ROW==COL
// result=A*B
void mpzMatrixMultiply(mpz_t A[ROW][COL],mpz_t B[ROW][COL],mpz_t result[ROW][COL]){
    mpz_t preResult[ROW][COL];
    for(unsigned indexR=0;indexR<ROW;indexR++){
        for(unsigned indexC=0;indexC<COL;indexC++){
            mpz_init(preResult[indexR][indexC]);
            mpz_set_ui(preResult[indexR][indexC],0);
        }
    }
    mpz_t preResultElement;
    mpz_init(preResultElement);
    for(unsigned indexR=0;indexR<ROW;indexR++){
        for(unsigned indexRecultC=0;indexRecultC<COL;indexRecultC++){
            for(unsigned indexC=0;indexC<COL;indexC++){
                mpz_mul(preResultElement,A[indexR][indexC],B[indexC][indexRecultC]);
                mpz_add(preResult[indexR][indexRecultC],preResult[indexR][indexRecultC],preResultElement);
            }
        }
    }
    for(unsigned indexR=0;indexR<ROW;indexR++){
        for(unsigned indexC=0;indexC<COL;indexC++){
            mpz_set(result[indexR][indexC],preResult[indexR][indexC]);
        }
    }
}

char* binaryExponentiationArray(unsigned n){
    unsigned n_inner=n;
    char* result=(char*)malloc(32*sizeof(unsigned));
    unsigned char resultIndex=0;
    while(n_inner){
        if(n_inner&1){
            result[resultIndex]=1;
        }
        n_inner=n_inner>>1;
        resultIndex++;
    }
    return result;
}

void calculateA(mpz_t A[ROW][COL],mpz_t A_power[ROW][COL],char* needStore,unsigned n){
    unsigned nCopy=n;
    unsigned char bitIndex=0;
    while(nCopy){
        if(needStore[bitIndex]){
            mpzMatrixMultiply(A,A_power,A);
        }
        mpzMatrixMultiply(A_power,A_power,A_power);
        nCopy=nCopy>>1;
        bitIndex++;
    }
}

void fibonacci(mpz_t A[ROW][COL],mpz_t A_power[ROW][COL],char* needStore,unsigned n,mpz_t result){
    if(!n){
        printf("n MUST BE GREATER THAN 0!\n");
        return;
    }
    if(n<=2){
        mpz_set_ui(result,1);
        return;
    }
    if(n&1){
        mpz_t F_N_Subtract1_divide2_Add1,F_N_Subtract1_divide2;
        mpz_inits(F_N_Subtract1_divide2_Add1,F_N_Subtract1_divide2,NULL);
        
        needStore=binaryExponentiationArray((n>>1)-1);
        calculateA(A,A_power,needStore,(n>>1)-1);
        
        mpz_add(F_N_Subtract1_divide2_Add1,A[0][0],A[0][1]);
        mpz_add(F_N_Subtract1_divide2,A[1][0],A[1][1]);
        mpz_pow_ui(F_N_Subtract1_divide2_Add1,F_N_Subtract1_divide2_Add1,2);
        mpz_pow_ui(F_N_Subtract1_divide2,F_N_Subtract1_divide2,2);
        mpz_add(result,F_N_Subtract1_divide2_Add1,F_N_Subtract1_divide2);
    }
    else{
        mpz_t F_N_divide2,F_N_divide2_Subtract1;
        mpz_inits(F_N_divide2,F_N_divide2_Subtract1,NULL);
        
        needStore=binaryExponentiationArray((n>>1)-2);
        calculateA(A,A_power,needStore,(n>>1)-2);
        
        mpz_add(F_N_divide2,A[0][0],A[0][1]);
        mpz_add(F_N_divide2_Subtract1,A[1][0],A[1][1]);
        mpz_mul_2exp(F_N_divide2_Subtract1,F_N_divide2_Subtract1,1);
        mpz_add(F_N_divide2_Subtract1,F_N_divide2,F_N_divide2_Subtract1);
        mpz_mul(result,F_N_divide2,F_N_divide2_Subtract1);
    }
}

int main(int argc, const char * argv[]) {
    unsigned n=10000000;
    if(argc>=2){
        n=strtoul(argv[1],NULL,10);
    }
    char* needStore=NULL;
    mpz_t A[ROW][COL],A_power[ROW][COL],result;
    for(unsigned indexR=0;indexR<ROW;indexR++){
        for(unsigned indexC=0;indexC<COL;indexC++){
            mpz_init(A[indexR][indexC]);
            mpz_init(A_power[indexR][indexC]);
        }
    }
//  STE A AS E AT THE BEGINNING
    mpz_set_ui(A[0][0],1);
    mpz_set_ui(A[0][1],0);
    mpz_set_ui(A[1][0],0);
    mpz_set_ui(A[1][1],1);
    
    mpz_set_ui(A_power[0][0],1);
    mpz_set_ui(A_power[0][1],1);
    mpz_set_ui(A_power[1][0],1);
    mpz_set_ui(A_power[1][1],0);
    
    mpz_init(result);
    struct timeval stopM,startM;
    gettimeofday(&startM,NULL);
    fibonacci(A,A_power,needStore,n,result);
    gettimeofday(&stopM,NULL);
//    gmp_printf("%Zd\n",result);
    printf("Elapsed time: %ld milliseconds\n",(stopM.tv_sec-startM.tv_sec)*1000+(stopM.tv_usec-startM.tv_usec)/1000);
    return 0;
}
