#include <string.h>
#include <math.h> //need gcc main.c -lm   to link math library

int base64EncodeSize(char str[])
{
    int len = strlen(str);

    return (4 * ((len +2)/ 3));
}

int * charToBin(char c)
{
        static int bin[8];
        for(int i = 0; i < 8; i++)
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
    int *newArr = malloc(sizeof(int) * (size1+size2));
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

//instead of using the buffs , copy them into arrays using intArrCpy
char *encodeBase64(char str[])
{
    const char base64tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int base64Size = base64EncodeSize(str);
    char *newStr = malloc(base64Size+1);

    int pos = 0;
    for(int i = 0; i < strlen(str); i = i + 3)
    {
        int zeroOnTwo = 0;
        int zeroOnThree = 0;

        int *buf1;
        int *buf2;
        int *buf3;

        buf1 = charToBin(str[i]);
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
            buf2 = charToBin(str[i+1]);
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
            buf3 = charToBin(str[i+2]);
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

char *decodeBase64(char str[])
{
    char * c;
    return c;
}

void encryption(char str[])
{

}