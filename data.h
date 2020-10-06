#include <stdlib.h>
#include <string.h>

#include "memoryLeak.h"

typedef struct
{
    char *data;
    int length;
} String;

#define InitData(X) Data X = {.length = 0}

int SetString(String *d, char *text)
{

    int n = 0;

    /* Count number of characters in text */
    while(text[n] != '\0')
        n++;

    if(d->length != n)
    {
        /* Free old data before allocating more */
        free(d->data);

        /* Allocate memory for text */
        d->data = (char*)malloc(n);

        d->length = n;
    }

    strcpy(d->data, text);

    return n;
}

void FreeString(String *d)
{
    free(d->data);
    d->length = 0;
}