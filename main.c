#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
// compile with gcc main.c -lm -lgmp / clang main.c -lm -lgmp -o main
//
//local
#include "encryption.h"
#include "cryptanalysis.h"



int main (int argc, char *argv[]) 
{   
    //char *str;
    //str = encodeBase64("Beispieltext");
    //int len = base64EncodeSize("Beispieltext");
    char* M, N1, N2, N3, c1, c2, c3;
    printf("Start running\n");
    // call hack with initialized array from cryptanalysis.h
    hack(M, N1_str, N2_str, N3_str, c1_str, c2_str, c3_str);

    //M is returned, action before clear memory
    free(M);

} 


/*Calculate M using Chinese Remainder Theorem
We proved in main that N1, N2, N3 gcd is 1. 
So we will find a unique X so that: 
M = X mod N1*N2*N3 
Based on the theorem 
X = sum(ABB') where 
    Ai = ci
    Bi = N1*N2*N3/Ni 
    B'i = modular invert of Bi mod Ni 
    or mpz_invert (mpz_t B', const mpz_t B, const mpz_t Ni)

Then forward search M so that M^e is X MOD N1*N2*N3 (BigN)
*/
void calculateM(mpz_t M, mpz_t N1, mpz_t N2,mpz_t N3, mpz_t c1, mpz_t c2, mpz_t c3){
    
    mpz_t       X, temp, B, Bi, BigN; 

    mpz_inits  (X, temp, B, Bi, BigN, M);
    //mpz_init(temp);
    //mpz_init(B);

    // calculate B
    mpz_mul (B, N2, N3);
    //calculate B'i
    mpz_invert (Bi, B, N1);
    //calculate temp = B*Bi
    mpz_mul(temp, B, Bi);
    // X = X + ci*temp
    mpz_addmul (X, c1, temp);


    //loop for 2 
    // calculate B
    mpz_mul (B, N1, N3);
    //calculate B'i
    mpz_invert (Bi, B, N2);
    //calculate temp = B*Bi
    mpz_mul(temp, B, Bi);
    // X = X + ci*temp
    mpz_addmul (X, c2, temp);


    //and 3 
    // calculate B
    mpz_mul (B, N1, N2);
    //calculate B'i
    mpz_invert (Bi, B, N3);
    //calculate temp = B*Bi
    mpz_mul(temp, B, Bi);
    // X = X + ci*temp
    mpz_addmul (X, c3, temp);


    //calculate BigN then add to X
    mpz_mul(temp, N1, N2);
    mpz_mul(BigN, temp, N3);

    int exact = 0;

    mpz_mod (temp, X, BigN);
    exact = mpz_root(M, temp, 3);
    printf("Exact root: %d\n", exact);

    mpz_clear(X);
    mpz_clear(temp);
    mpz_clear(B);
    mpz_clear(Bi);
    mpz_clear(BigN);

    return;
}

/*  Take string N1, N2, N3, c1, c2, c3
    Return string M
*/
void hack (char* message, char* N1_s, char* N2_s,char* N3_s,
    char* c1_s, char* c2_s, char* c3_s){
    mpz_t N1, N2, N3, c1, c2, c3,
    gcd12, gcd23 ,gcd13; 
    
    //FILE * nullP = NULL;
    // check if all the parameters are initialize 
    mpz_inits(N1, N2, N3, c1, c2, c3);
    
    mpz_set_str(N1, N1_s,10);
    mpz_set_str(N2, N2_s,10);
    mpz_set_str(N3, N3_s,10);
    mpz_set_str(c1, c1_s,10);
    mpz_set_str(c2, c2_s,10);
    mpz_set_str(c3, c3_s,10);

    printf("After initializing big numbers\n");
    /*
    free(N1_s);
    free(N2_s);
    free(N3_s);
    free(c1_s);
    free(c2_s);
    free(c3_s);
    */
    //find 3 greatest common divisors - gcd 
    //for each pair c1-c2, c1-c3, c2-c3

    mpz_init(gcd12);
    mpz_gcd (gcd12, N1, N2); 
    mpz_init(gcd13);
    printf("Greatest Common Divisor of N1 and N2: ");
    mpz_out_str(NULL, 10, gcd12);
    printf("\n");

    mpz_gcd (gcd13, N1, N3); 
    mpz_init(gcd23);
    printf("Greatest Common Divisor of N1 and N3: ");
    mpz_out_str(NULL, 10, gcd13);
    printf("\n");

    mpz_gcd (gcd23, N2, N3); 
    printf("Greatest Common Divisor of N2 and N3: ");
    mpz_out_str(NULL, 10, gcd23);
    printf("\n");

    mpz_t M;
    calculateM(M, N1, N2, N3, c1, c2, c3);
    printf("M in base16 is: ");
    mpz_out_str(NULL, 16, M);
    char* M_str;
    M_str = mpz_get_str(NULL, 16, M);
    int len = strlen(M_str);
    
    printf("\n");
    char * M_ascii = hexToAscii(M_str, len);
    printf("Message in base64 is: ");
    for (int i=0; i< strlen(M_ascii); i++){
        printf("%c",M_ascii[i]);   
    }
    printf("\n");
    message = decodeBase64(M_ascii);
    printf("Message is: ");
    for (int i=0; i< strlen(message); i++){
        printf("%c",message[i]);  
    }
    printf("\n");
    
    free(M_ascii);
    free(M_str);
    mpz_clear(gcd12);
    mpz_clear(gcd13);
    mpz_clear(gcd23);
    mpz_clear(M);
    mpz_clear(N1);
    mpz_clear(N2);
    mpz_clear(N3);
    mpz_clear(c1);
    mpz_clear(c2);
    mpz_clear(c3);
    return;
}