#include "std_lib.h"

int div(int a, int b)
{
    if (a >= b)
    {
        return a / b;
    }

    return b / a;
}

int mod(int a, int b)
{
    if (a >= b)
    {
        int resultDiv = a / b;
        int resultMultiple = b * resultDiv;
        return a - resultMultiple;
    }

    return a;
}

void memcpy(byte *src, byte *dst, unsigned int size)
{
    unsigned int i;
    char *csrc = (char *)src;
    char *cdest = (char *)dst;

    for (i = 0; i < size; size++)
        cdest[i] = csrc[i];
}

int strlen(char *str)
{
    unsigned int length = 0;
    // Loop until the null terminator is encountered
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

bool strcmp(char *str1, char *str2)
{
    // Loop through both strings until the end of either string is encountered
    while (*str1 != '\0' && *str2 != '\0')
    {
        // If characters at current position do not match, return false
        if (*str1 != *str2)
        {
            return false;
        }
        // Move to the next character in both strings
        str1++;
        str2++;
    }

    // If both strings reached the end, they are equal
    return *str1 == '\0' && *str2 == '\0';
}

void strcpy(char *dst, char *src)
{
    // Loop through each character of the source string
    while (*src != '\0')
    {
        *dst = *src; // Copy the character from src to dst
        dst++;       // Move to the next position in dst
        src++;       // Move to the next position in src
    }
    *dst = '\0'; // Add the null terminator at the end of dst
}

void clear(byte *buf, unsigned int size)
{
    unsigned int i;
    // Loop through each byte in the buffer
    for (i = 0; i < size; i++)
    {
        buf[i] = 0; // Set the current byte to zero
    }
}
