#include <stdlib.h>
#include <string.h>

#include "memoryLeak.h"

#define __FILE__ "data.h"

typedef struct
{
    char *data;
    int length;
} Data;

#define InitData(X) Data X = {.length = 0}

int SetData(Data *d, char *text)
{
    #define __FUNC__ "SetData"

    int n = 0;

    /* Count number of characters in text */
    while(text[n] != '\0')
        n++;

    if(d->length != n)
    {
        /* Free old data before allocating more */
        #define __LINE__ 29
        free(d->data);

        /* Allocate memory for text */
        #define __LINE__ 33
        d->data = (char*)malloc(n);

        d->length = n;
    }

    strcpy(d->data, text);
}