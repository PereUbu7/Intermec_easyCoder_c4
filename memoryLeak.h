#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct allocMem
{
    void *p;
    char file[50];
    int line;
    char func[50];
    size_t size;

    struct allocMem *next;
} AllocMem;

static short init = 0;
static int numberOfEntries = 0;

AllocMem *memoryAllocList;

void printMemoryAllocationList()
{
    if(memoryAllocList == NULL)
    {
        printf("No memory leaks detected!\n");
    }
    else
    {
        printf("Detected memory %i leaks:\n", numberOfEntries);

        AllocMem *entry = memoryAllocList;
        while(entry != NULL)
        {
            printf("%s\t%i\t%s\t%p[%li]\n", entry->file, entry->line, entry->func, entry->p, entry->size);
            AllocMem *next = entry->next;
            free(entry);
            entry = next;
        }
    }   
}

void *my_malloc(size_t size, const char *file, int line, const char *func)
{
    if(!init)
    {
        atexit(printMemoryAllocationList);
        init = 1;
    }

    void *p = malloc(size);
    printf("Allocated = %s, %i, %s, %p[%li]\n", file, line, func, p, size);

    AllocMem *lastEntry = memoryAllocList;

    /* Go to last */
    while(lastEntry != NULL && lastEntry->next != NULL) 
    {
        lastEntry = lastEntry->next; 
    }

    AllocMem *newEntry = (AllocMem*)malloc(sizeof(AllocMem));

    /* Save to log */
    strcpy(newEntry->file, file);
    newEntry->line = line;
    newEntry->size = size;
    newEntry->p = p;
    newEntry->next == NULL;
    strcpy(newEntry->func, func);

    if(lastEntry != NULL) { lastEntry->next = newEntry; }
    else { memoryAllocList = newEntry; }

    numberOfEntries++;    

    return p;
}

void *my_calloc(size_t size, const char *file, int line, const char *func)
{
    my_malloc(size, file, line, func);
}

void my_free(void *p, const char *file, int line, const char *func)
{
    if(!init)
    {
        atexit(printMemoryAllocationList);
        init = 1;
    }

    free(p);
    
    AllocMem *entry = memoryAllocList;
    AllocMem *prevEntry = NULL;

    /* Find matching entry */
    while(entry != NULL && entry->p != p) 
    { 
        prevEntry = entry;
        entry = entry->next; 
    }

    if(entry == NULL)
    {
        perror("Couldn't find memory block to free");
        return;
    }
    else
    {
        printf("Freed = %s, %i, %s, %p[%li]\n", file, line, func, p, entry->size);

        /* If removing only entry */
        if(prevEntry == NULL && entry->next == NULL)
        {
            memoryAllocList = NULL;
        }
        /* If removing first entry */
        else if(prevEntry == NULL)
        {
            memoryAllocList = entry;
        }
        /* If removing middle entry */
        else if(entry->next != NULL)
        {
            prevEntry->next = entry->next;
        }
        /* Removing last entry */
        else
        {
            prevEntry->next = NULL;
        }
        

        numberOfEntries--;

        free(entry);
    }

    return;
}

#undef malloc
#undef calloc
#undef free
#define malloc(X) my_malloc(X, __FILE__, __LINE__, __func__)
#define calloc(X) my_calloc(X, __FILE__, __LINE__, __func__)
#define free(X) my_free(X, __FILE__, __LINE__, __func__)