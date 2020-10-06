#include <stdio.h>
#include <stdlib.h>
#include "data.h"

typedef struct
{
    FILE *fp;
    String currentCommand;
    int finish;
} ESimReader;

ESimReader *openESimCommandFile(char *fileName)
{
    FILE *fp = fopen(fileName, "r");

    if(fp == NULL)
    {
        perror("openESimCommandFile: Couldn't open file");
        return NULL;
    }

    ESimReader *obj = (ESimReader*)malloc(sizeof(ESimReader));

    if(obj == NULL)
    {
        perror("openESimCommandFile: Couldn't allocate memory");
        return NULL;
    }

    obj->fp = fp;
    obj->currentCommand.length = 0;
    obj->finish = 1;

    return obj;
}

char *readESimCommand(ESimReader *obj)
{
    if(obj == NULL)
    {
        perror("readESimCommand: Reader cannot be NULL");
        return 0;
    }

    if(obj->fp == NULL)
    {
        perror("readESimCommand: Reader has no open file");
        return 0;
    }

    size_t bufferSize = 200;
    char buffer[bufferSize];
    char *b = buffer;

    int finish = getline(&b, &bufferSize, obj->fp);

    if(finish == -1)
    {
        obj->finish = 0;
        buffer[0] = '\0';
        SetString(&(obj->currentCommand), buffer);
        return obj->currentCommand.data;
    }

    /* Find end of line */
    for(int i = 0; i < bufferSize; i++)
    {
        /* Replace linux newline with windows newline (CR LF) */
        if((buffer[i] == '\n' || buffer[i] == '\0') && i < (bufferSize - 2))
        {
            buffer[i] = '\r';
            buffer[i+1] = '\n';
            buffer[i+2] = '\0';

            break;
        }
        else if((buffer[i] == '\n' || buffer[i] == '\0') && i == (bufferSize - 2) )
        {
            buffer[i-1] = '\r';
            buffer[i] = '\n';
            buffer[i+1] = '\0';

            break;
        }
    }

    buffer[bufferSize-1] = '\0';

    SetString(&(obj->currentCommand), buffer);

    return obj->currentCommand.data;
}

int closeESimCommandFile(ESimReader *reader)
{
    if(reader == NULL)
    {
        perror("ESimReader cannot be NULL");
        return 0;
    }

    int closeResult = fclose(reader->fp);

    if(closeResult == -1)
    {
        perror("Couldn't close command file!");
        return closeResult;
    }

    FreeString(&(reader->currentCommand));

    free(reader);
}