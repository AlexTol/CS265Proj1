#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> //need gcc main.c -lm   to link math library

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

int * concatArrs(int *arr1,int *arr2,int size1,int size2)
{
    int *newArr = (int *) malloc(sizeof(int) * (size1+size2));
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

    return newArr;
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
    char *newArr = (char *)malloc(len * 2);
    for(int i = 0, j = 0;i < len;i++, j +=2)
    {
        sprintf(newArr + j,"%02x",str[i] & 0xff);
    }

    return newArr;
}

char *hexToAscii(char str[],int len)
{
    char *newArr = (char *)malloc((len / 2)+1);

    for(int i = 0, j = 0; j < len; i++, j += 2)
    {
        int val[1]; //this needs to be an array since it matches with the int[] type not int type
        sscanf(str + j,"%2x",val);
        newArr[i] = val[0];
    }
    newArr[len/2] ='\0';

    return newArr;
}

//instead of using the buffs , copy them into arrays using intArrCpy
char *encodeBase64(char str[])
{
    const char base64tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int base64Size = base64EncodeSize(str);
    char *newStr = (char*) malloc(base64Size+1);

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
        //free(buf1);

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
        //free(buf2);

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
        //free(buf3);

        int *temp1 = concatArrs(pad1,pad2,8,8);
        int  temp2[16];
        intArrCpy(temp2,temp1,16);
        free(temp1);

        int *temp3 = concatArrs(temp2,pad3,16,8);
        int padStr[24];
        intArrCpy(padStr,temp3,24);
        free(temp3);

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

char *decodeBase64(char str[])
{
    int regSize = base64DecodeSize(str);
    char *newStr = (char*)malloc(regSize+1);


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

        buf2 = decToBin(s2,6);
        int pad2[6];
        intArrCpy(pad2,buf2,6);

        int pad3[6];
        if(s3 == -1)
        {
            ignore3 = 1;
        }
        else
        {
            buf3 = decToBin(s3,6);
            intArrCpy(pad3,buf3,6);
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
        }

        int *temp1 = concatArrs(pad1,pad2,6,6);
        int  temp2[12];
        intArrCpy(temp2,temp1,12);
        free(temp1);

        int *temp3 = concatArrs(temp2,pad3,12,6);
        int temp4[18];
        intArrCpy(temp4,temp3,18);
        free(temp3);

        int *temp5 = concatArrs(temp4,pad4,18,6);
        int padStr[24];
        intArrCpy(padStr,temp5,24);
        free(temp5);

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

    return newStr;
}

//todo rename this to decryption
void encryption(char str[])
{
    char * M = "Einladung zu meiner Geburtstagsparty. Die Party findet am 20.12.2012 in Bletchley statt.";


}