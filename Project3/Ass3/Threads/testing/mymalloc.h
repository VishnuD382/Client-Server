#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef  _mymalloc_h
#define _mymalloc_h

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)


//Global vars
extern char* memArray;
extern int sizeof_memArray;
static int numCalls;

//First Node
typedef struct memList_node memListNode;

//Helper functions
void init();
void* split(int);
void defrag();

//Main functions
void* mymalloc(int memReqSize, char* FILE, int LINE);
void* myfree(void* freeAddress, char* FILE, int LINE);

void printError(char* msg, char* FILE, int LINE);

#endif // ! _mymalloc_h
