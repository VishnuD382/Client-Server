//
// Created by Christopher Pellegrino and Vishn on 10/19/2019.
//

#include <stdio.h>


//Your memory array
static unsigned char memArray[4096];

//Memory node
struct memListNode {
	int size;
	int free; // 1 = free 0 = not free
	struct memListNode *next;
};

//Define the first node
struct memListNode *firstNode = (void *) memArray;

/*/////////////////////////////////////////////
/											  /
/	    Begin mymalloc helper functions		  /
/											  /
/////////////////////////////////////////////*/


//Initialize first Node
void init() {

	//Set the first node
	firstNode->free = 1;
	firstNode->size = sizeof(memArray);// -sizeof(*mainNode);
	firstNode->next = NULL;

}

//Split the free nodes and assign the first one that will fit the info
void *split(struct memListNode *newNode, int size, char* file, int* line) {

	//init two nodes to make swaps
	//struct memListNode* newNode = (void*)memArray;
	struct memListNode *moveNode = (void *) memArray;



	//Find the first one that will fit the required space
	while (moveNode->size < size || !moveNode->free ) {
		if(moveNode -> next == NULL)
			return NULL;
		moveNode = moveNode->next;
	}

	//If the user asks for too much memory
	if (moveNode->size < size) {

		printError("didn't watch how much mmory you're taking!!\nPlease make sure ALL of you allocations equal no more than 4096 bytes (including 16 bytes for EACH allocation)\n\n");
		return NULL;

	}
	//Swap out the node without losing the reference to the next node
	newNode = moveNode;
	char *movePtr = moveNode;

	if ((moveNode->next - moveNode) > sizeof(*moveNode) + 1) {
		movePtr += (size);
		moveNode = (void *) movePtr;

		//Set the free space node
		moveNode->free = 1;
		moveNode->next = newNode->next;
		moveNode->size = ((newNode->size) - size);

		//Set the requested node
		newNode->free = 0;
		newNode->next = moveNode;
		newNode->size = size;
	}

	else {
		newNode->free = 0;
		newNode->next = moveNode->next;
		newNode->size = (moveNode->next - moveNode);

	}

	//Send back the address of the requested node's free information
	return (newNode + 1);


}

/*/////////////////////////////////////////////
/											  /
/          Begin myfree helpers 			  /
/											  /
/////////////////////////////////////////////*/


//Defragment function
void defrag()
{

	//Declare a lead and follow node
	struct memListNode* followNode = (void*)memArray;
	struct memListNode* leadNode = (void*)memArray;

	//Increment leadNode
	if (leadNode->next != NULL)
	{

		leadNode = leadNode->next;

	}

	//Increment the nodes so that one follows the other...
	while (leadNode->next != NULL)
	{
		//If two in a row are free, we combine them and only incremenet the lead node (in case we have three in a row that are free after the combining)
		if (followNode->free && leadNode->free)
		{
			followNode->size += leadNode->size;
			followNode->next = leadNode->next;

			leadNode = leadNode->next;
		}

		//Otherwise, increment them both...
		else if (leadNode->next != NULL)
		{
			followNode = leadNode;
			leadNode = leadNode->next;

		}

		else
		{

			followNode = leadNode;

		}
	}

	if (followNode->free && leadNode->free)
	{

		followNode->size += leadNode->size;
		followNode->next = leadNode->next;
		leadNode = leadNode->next;

	}


}

void printError(char *msg, char *file, int line) {
	//Fun print line...
	printf("ERROR in \"%s\" on line %d....\n\nYou %s", file, line, msg);

}


/*/////////////////////////////////////////////
/											  /
/		  Begin mymalloc & myfree			  /
/											  /
/////////////////////////////////////////////*/

//myMalloc
void *mymalloc(int memReqSize, char *file, int line) {
	//If a joker wants to order a negative number

	if (memReqSize < 1) {
		printError("tried to allocate a memory block of value less than 1...\nPlease enter a value greater than 0!\n\n", file, line);
		return NULL;

	}
	if (memReqSize > sizeof(memArray)) {

		printError("tried to allocate a memory block of value greater than size of Memory...\nPlease enter a value less than 4080!\n\n", file, line);
		return NULL;
	}

	//Increase the request size to include the size of the struct
	memReqSize = memReqSize + sizeof(*firstNode);


	//initialize the first node
	if (firstNode->size == 0) {

		init();


	}

	else {

		struct memListNode *firstNode = (void *) memArray;


	}

	////////BEGIN TEST /////////
	struct memListNode* newNode = split(firstNode, memReqSize, file, line);

	//Return the memory address of the next available block that fits this
	return (void *) newNode;


}

void *myfree(void *freeAddress, char *file, int line) {




	//Grab char address for the requested free address

	//Move the pointer back to before the Node
	//nodeAddress -= sizeof(*firstNode);

	if (freeAddress == NULL) {
		printError(	" provided a NULL pointer! Please provide an allocated pointer... \n\n", file, line);
		return 1;

	}

	char* nodeAddress = (freeAddress - 16);

	//Declare two nodes for tracking of the new setup
	struct memListNode* freeNode = nodeAddress;
	struct memListNode* tempNode = (void*)memArray;

	if (freeNode->free) {//|| *nodeAddress == '\0') {
		printError("tried to deallocate something twice, or tried to deallocate a pointer that doesn't exists... Go back and try again!\n\n", file, line);
		return 1;

	}

	int counter = 0;

	//While tempNode is not the correct node, push it down the line
	while (tempNode != freeNode) {

		tempNode = tempNode->next;


	}

	counter++;

	//Make tempNode free
	tempNode->free = 1;


	//defrag the cells
	defrag(tempNode);

	return 0;
}
