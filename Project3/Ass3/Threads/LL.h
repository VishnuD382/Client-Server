#ifndef _LL_h
#define _LL_h

typedef struct messages
{
	char inMessage[2000];
	struct messages *next;

} messages_t;

typedef struct box
{
	pthread_mutex_t lock;
	char name[26];
	int openStatus;
	messages_t *firstMessage;
	struct box *next;

} box_t;

int chkMalloc(size_t sz);
void addBox(box_t *first, int sock);
void addMessage(box_t *first, char *message);
void dumpDetails(box_t *currBox);

#endif // ! _LL_h