#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
// compile with gcc main.c -lm -lgmp / clang main.c -lm -lgmp -o main
//
//local
#include "encryption.h"
#include "cryptanalysis.h"



int main (int argc, char *argv[]) 
{   
    mpz_t c1,c2,c3,N1,N2,N3,M;
    mpz_init(c1);
    mpz_init(c2);
    mpz_init(c3);
    mpz_init(N1);
    mpz_init(N2);
    mpz_init(N3);
    mpz_init(M);

    char *message = "Invitation/to/my/birthday/party/The/party/will/take/place/on/December/20th/2012/in/Bletchley";
    int len1 = strlen(message);
    char *eStr = "3";
    char *N1Str = "514745167025222387434132377137056715954750729807151447929894289695587285793889099978536904494455862473045694392353612260528582074521711735864082380505874261026769465596315849668245703081452047808798727647904141791488099702631575692170683102622471798376397440600292225038412176681344166204027842724877162681931";
    char *N2Str = "332459552799915544356022641605448137617079921391832222557892949808060953028449422328281413629912335051440744955455010851012308918294549765005480121061697711447087615327860789708246235156912421474047484838827777697938563515420810650393553528058831317409340577149233554235346445890238642955390137465511286414033";
    char *N3Str = "665701912162243069059653781669230805473457427767514323262762891771122352328706695409103713864384833437438648120217615990765220365745013739246022203593234785338178963805463643869398986119431772931646042972240277833431035018628949924813463553419243108837309078316455504749755062865258063926243606206806549969161";
    mpz_set_str(N1,N1Str,10);
    mpz_set_str(N2,N2Str,10);
    mpz_set_str(N3,N3Str,10);


    encryption(message,c1,eStr,N1Str);
    encryption(message,c2,eStr,N2Str);
    encryption(message,c3,eStr,N3Str);

    printf("\n Encryption Complete. Press Any Key to Continue\n");  
    getchar(); 
    
    calculateM(M,N1,N2,N3,c1,c2,c3);

    //gmp_printf("gmp: %Zd \n", M);
    char *buffer =(char*) malloc((len1*2)+1);
    mpz_get_str(buffer,10, M);
    //printf("gmp to string %s\n",buffer);
    int len2 = strPointerLen(buffer);
    free(buffer);

    char *hiddenMessage;
    hiddenMessage = decryptionTrunc(M,len2);
    mpz_clear(N1);
    mpz_clear(N2);
    mpz_clear(N3);
    mpz_clear(c1);
    mpz_clear(c2);
    mpz_clear(c3);
    mpz_clear(M);

    // call hack with initialized array from cryptanalysis.h
    //hack(M, N1_str, N2_str, N3_str, c1_str, c2_str, c3_str);

    //M is returned, action before clear memory
    //free(M);

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



