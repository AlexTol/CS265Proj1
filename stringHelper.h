#ifndef _STRINGHELPERH_
#define _STRINGHELPERH_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *stripNewLine(char *str)
{
    int len = strlen(str);
    char *newArr = (char *)malloc(len+1);

    for(int i = 0; i < len;i++)
    {
        if(i == (len-1) && str[len-1] == '\n')
        {
            newArr[i] = '\0';
        }
        else
        {
            newArr[i] = str[i];
        }
    }
    newArr[len] = '\0';
    return newArr;
}

int strPointerLen(char *ptr)
{
    int count = 0;
    int pos = 0;

    while(ptr[pos] != '\0')
    {
        count++;
        pos++;
    }

    return count;
}

#endif