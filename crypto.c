#include "crypto.h"

#define SHIFT 5

void encryptText(char* text)
{
    int i = 0;

    while (text[i] != '\0')
    {
        text[i] += SHIFT;
        i++;
    }
}

void decryptText(char* text)
{
    int i = 0;

    while (text[i] != '\0')
    {
        text[i] -= SHIFT;
        i++;
    }
}