#include <stdio.h>
#include <stdlib.h>
#include "encryption.h"


int main (int argc, char *argv[]) 
{   
    char *str;
    str = encodeBase64("Beispieltext");
    int len = base64EncodeSize("Beispieltext");

    printf("%s\n",str);
    return 0;
} 