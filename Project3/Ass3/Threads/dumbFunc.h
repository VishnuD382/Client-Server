
#ifndef _dumbFunch_h
#define _dumbFunch_h

#include "LL.h"

int helpPrint(int sock);
int isFileExists(const char *path);
box_t *createBox(int sock, box_t *firstBox);
int deleteBox(box_t *box);
box_t *openBox(int sock, box_t *firstBox);
int closeBox(int sock, box_t *curr);
int nextMessage(int sock, box_t *curr);
int putMessage(int sock, box_t *firstBox);

#endif // ! _dumbFunch_h