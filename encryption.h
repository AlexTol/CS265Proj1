#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringHelper.h"
#include <gmp.h>
#include <math.h> //need gcc main.c -lm   to link math library
//Invitation to my birthday party. The party will take place on December 20th, 2012 in Bletchley.
int base64DecodeSize(char str[])
{
    int len = strlen(str);

    return (3 * (len/4));
}

int base64EncodeSize(char str[])
{
    int len = strlen(str);

    return (4 * ((len +2)/ 3));
}

int * decToBin(int d, int size)
{
    int *bin = (int *)malloc(sizeof(int) * size);
    
    int mNum = d;
    int spot = size-1;
    for(int i = 0; i < size; i++)
    {
        if(pow(2,spot) > mNum)
        {
            bin[i] = 0;
        }
        else
        {
            bin[i] = 1;
            mNum = mNum - pow(2,spot);
        }

        spot--;
    }
    
    return bin;
}

int * charToBin(char c,int size)
{
        int *bin = (int *)malloc(sizeof(int) * size);
        for(int i = 0; i < size; i++)
        {
            bin[i] = ( ((c << i) & 128)/128);
        }

        //bin[8] = '\0';

        return bin;
}

int * zeroArr()
{
    static int bin[8];

        for(int i = 0; i < 8; i++)
        {
            bin[i] = 0;
        }

    return bin;
}

int binArrToDec(int *bin,int len)
{
    int val = 0;
    int power = len-1;

    for(int i=0; i < len; i++)
    {
        if(bin[i] == 1)
        {
            val += (int) pow(2,power);
        }
        power--;
    }

    return val;
}

void concatArrs(int newArr[],int *arr1,int *arr2,int size1,int size2)
{
    for(int i=0; i < size1;i++)
    {
        newArr[i] = arr1[i];
    }

    int counter = 0;
    for(int i=size1;i < size1+size2;i++)
    {
        newArr[i] = arr2[counter];
        counter++;
    }

}

//todo finish this
void intArrCpy(int *targ,int *source,int size)
{
    for(int i = 0; i < size; i++)
    {
        targ[i] = source[i];
    }
}

char *asciiToHex(char str[],int len)
{
    char *newArr = (char *)malloc((len * 2)+1);
    for(int i = 0, j = 0;i < len;i++, j +=2)
    {
        sprintf(newArr + j,"%02x",str[i] & 0xff);
    }

    return newArr;
}

void hexToAscii(char str[],char ascii[],int len)
{
    int base = 64;
    for(int i = 0, j = 0; j < len; i++, j += 2)
    {
        int val[1]; //this needs to be an array since it matches with the int[] type not int type
        sscanf(str + j,"%2x",val);
        ascii[i] = val[0];
    }
    ascii[len/2] ='\0';
}

void hexToInt(mpz_t val,char hex[])
{
    mpz_set_str(val,hex,16);
}

void intToHex(mpz_t val,char hex[])
{
    mpz_get_str(hex, 16, val);
}

/*char *intToHex(long int num)
{
    char *hex = (char *)malloc(100);
    
}*/

//instead of using the buffs , copy them into arrays using intArrCpy
char *encodeBase64(char str[])
{
    const char base64tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int base64Size = base64EncodeSize(str);
    char *newStr = (char*)malloc(base64Size+1);

    int pos = 0;
    for(int i = 0; i < strlen(str); i = i + 3)
    {
        int zeroOnTwo = 0;
        int zeroOnThree = 0;

        int *buf1;
        int *buf2;
        int *buf3;

        buf1 = charToBin(str[i],8);
        int pad1[8];
        intArrCpy(pad1,buf1,8);
        free(buf1);

        if(i + 1 > strlen(str) - 1)
        {
            buf2 = zeroArr();
            zeroOnTwo = 1;
        }
        else
        {
            buf2 = charToBin(str[i+1],8);
        }

        int pad2[8];
        intArrCpy(pad2,buf2,8);
        if(zeroOnTwo == 0)
        {
            free(buf2);
        }

        if(i + 2 > strlen(str) - 1)
        {
            buf3 = zeroArr();
            zeroOnThree = 1;
        }
        else
        {
            buf3 = charToBin(str[i+2],8);
        }

        int pad3[8];
        intArrCpy(pad3,buf3,8);
        if(zeroOnThree == 0)
        {
            free(buf3);
        }

        int  temp2[16];
        concatArrs(temp2,pad1,pad2,8,8);
        //intArrCpy(temp2,temp1,16);
        //free(temp1);

        int padStr[24];
        concatArrs(padStr,temp2,pad3,16,8);
        //intArrCpy(padStr,temp3,24);
        //free(temp3);

        int group1[6];
        int group2[6];
        int group3[6];
        int group4[6];
    
        int count1 = 0;
        int count2 = 0;
        int count3 = 0;
        int count4 = 0;

        for(int i = 0; i < 24; i++)
        {
            if(count1 < 6)
            {
                group1[count1] = padStr[i];
                count1++;
            }
            else if(count2 < 6)
            {
                group2[count2] = padStr[i];
                count2++;
            }
            else if(count3 < 6)
            {
                group3[count3] = padStr[i];
                count3++;
            }
            else if(count4 < 6)
            {
                group4[count4] = padStr[i];
                count4++;
            }

        }

        int val1 = binArrToDec(group1,6);
        int val2 = binArrToDec(group2,6);
        int val3 = binArrToDec(group3,6);
        int val4 = binArrToDec(group4,6);

        newStr[pos] = base64tab[val1];
        newStr[pos+1] = base64tab[val2];
        newStr[pos+2] = (zeroOnTwo == 1) ? '=' : base64tab[val3];
        newStr[pos+3] = (zeroOnThree == 1) ? '=' :base64tab[val4];

        pos += 4;
    }

    newStr[base64Size] = '\0';

    //printf("%s\n",newStr);
    return newStr;
}

int returnSpace(char c)
{
    const char base64tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    for(int i = 0; i <64; i++)
    {
        if(c == base64tab[i])
        {
            return i;
        } 
    }

    return -1;
}

void decodeBase64(char str[],char newStr[],int regSize)
{
    regSize += 1;

    int pos = 0;
    for(int i = 0; i < strlen(str); i = i + 4)
    {
        int s1 = returnSpace(str[i]);
        int s2 = returnSpace(str[i+1]);
        int s3 = returnSpace(str[i+2]);
        int s4 = returnSpace(str[i+3]);

        int ignore3 = 0;
        int ignore4 = 0;

        int *buf1;
        int *buf2;
        int *buf3;
        int *buf4;

        buf1 = decToBin((char) s1,6);
        int pad1[6];
        intArrCpy(pad1,buf1,6);
        free(buf1);

        buf2 = decToBin(s2,6);
        int pad2[6];
        intArrCpy(pad2,buf2,6);
        free(buf2);

        int pad3[6];
        if(s3 == -1)
        {
            ignore3 = 1;
        }
        else
        {
            buf3 = decToBin(s3,6);
            intArrCpy(pad3,buf3,6);
            free(buf3);
        }
        int pad4[6];
        if(s4 == -1)
        {
            ignore4 = 1;
        }
        else
        {
            buf4 = decToBin(s4,6);
            intArrCpy(pad4,buf4,6);
            free(buf4);
        }

        int  temp2[12];
        concatArrs(temp2,pad1,pad2,6,6);
        //intArrCpy(temp2,temp1,12);
        //free(temp1);

        int temp4[18];
        concatArrs(temp4,temp2,pad3,12,6);
        //intArrCpy(temp4,temp3,18);
        //free(temp3);

        int padStr[24];
        concatArrs(padStr,temp4,pad4,18,6);
        //intArrCpy(padStr,temp5,24);
        //free(temp5);

        int group1[8];
        int group2[8];
        int group3[8];
    
        int count1 = 0;
        int count2 = 0;
        int count3 = 0; 

        for(int i =0; i < 24; i++)
        {
            if(count1 < 8)
            {
                group1[count1] = padStr[i];
                count1++;
            }
            else if(count2 < 8)
            {
                group2[count2] = padStr[i];
                count2++;
            }
            else if(count3 < 8)
            {
                group3[count3] = padStr[i];
                count3++;
            }
        }

        

        int val1 = binArrToDec(group1,8);
        int val2 = binArrToDec(group2,8);
        int val3 = binArrToDec(group3,8);

        newStr[pos] = val1;
        newStr[pos+1] = (ignore3  == 1) ? '\0' : val2;
        newStr[pos+2] = (ignore4  == 1) ? '\0' : val3;

        pos +=3;
    }
    newStr[regSize] = '\0';

}

//todo rename this to decryption
void encryption(char str[],mpz_t ret,char *eStr,char *NStr)
{
    char *buf1;
    buf1 = stripNewLine(str);

    char *buf2;
    buf2 = encodeBase64(buf1);
    int len = base64EncodeSize(buf1);

    char *hex;
    hex = asciiToHex(buf2,len);
    //printf("%s\n",hex);

    hexToInt(ret,hex);
    mpz_t e,N;
    mpz_init(e);
    mpz_init(N);
    mpz_set_str(e, eStr,10);
    mpz_set_str(N, NStr,10);
    //gmp_printf("gmp: %Zd \n", ret);
    mpz_powm(ret,ret,e,N);

    free(buf1);
    free(buf2);
    free(hex);
    mpz_clear(e);
    mpz_clear(N);
    gmp_printf("Encryption Result: %Zd \n", ret);
}

void decryption(mpz_t val,char *dStr,char *NStr)
{
    mpz_t d,N;
    mpz_init(d);
    mpz_init(N);
    mpz_set_str(d, dStr,10);
    mpz_set_str(N, NStr,10);
    mpz_powm(val,val,d,N);
    //gmp_printf("gmp: %Zd \n", val);
}

char *decryptionTrunc(mpz_t val,int len)
{
    //gmp_printf("gmp2: %Zd \n", val);
    //printf("here0\n");
    char newHex[len];
    intToHex(val,newHex);
    //printf("%s\n",newHex);

    //printf("here1\n");
    char ascii[len/2+1];
    hexToAscii(newHex,ascii,len);
    //printf("%s\n",ascii);

    //printf("here2\n");
    int regSize = base64DecodeSize(ascii);
    char str[regSize+1];
    decodeBase64(ascii,str,regSize);
    printf("Final Decryption: %s\n",str);
    char *str2 = str;


    return str2;
}