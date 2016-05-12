// Michael R. Callan III 12912150
// James Albert III 16004325
// John Reissmueller 29856616

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

// Definitions
#define MAXBLOCKS   128   // maximum amount of blocks
#define MAXARGS     128   // maximum amount of arguments
#define MAXCMD      255   // maximum length of command
#define HEAPSIZE    400   // size of heap
#define TESTING     0
#define DEBUG       1

static int bID;

// Prototypes
int parsecommand(char*, char* opts[MAXARGS]);
int Allocate(int*, int);
int Free(int* heap, int);
int blockList(int*);
int writeHeap(int*, int, char, int);
int printHeap(int*, int, int);
int* findBlockId(int*, int);

// Mike's (Remove name before submission)
#if TESTING == 0
int main () {
  // variable declaration
  char command[MAXCMD];
  int* heap = malloc(HEAPSIZE);
  char* argv[MAXARGS];
  int argc;

  // memory management
  *heap = HEAPSIZE;
  bID = 0;

  // Loop - fetch command, call functions
  while(1) {
     printf("> ");
     gets(command);
     argc = parsecommand(command, argv);

     if(strcmp(argv[0], "allocate") == 0) {
       printf("Calling Allocate with %s\n", argv[1]);
       Allocate(heap, atoi(argv[1]));
     }
     else if(strcmp(argv[0], "free") == 0)
       Free(heap, atoi(argv[1]));
     else if(strcmp(argv[0], "blocklist") == 0)
       blockList(heap);
     else if(strcmp(argv[0], "writeheap") == 0)
       writeHeap(heap, atoi(argv[1]), argv[2][0], atoi(argv[3]));
     else if(strcmp(argv[0], "printheap") == 0)
       printHeap(heap, atoi(argv[1]), atoi(argv[2]));
     else if(strcmp(argv[0], "quit") == 0)
       break;
  }

  free(heap);
  return 0;
}
#endif

bool isZero(int param) {
  return param == 0;
}

int parsecommand(char* command, char* opts[MAXARGS]) {
  /* parsecommand
      @command: full command from prompt
      @opts: options to binary

      takes a full command (eg service nginx start) and places the
      command name (service) and arguments ([nginx, start]) in opts.
  */
  int argc = 0;
  char* delim = " \t\n";
  char* tok = strtok(command, delim);
  // first argument of opts must be the filename with path
  opts[argc] = malloc(strlen(command)*sizeof(char));
  strcpy(opts[argc], tok); // append command to binary name
  for (tok = strtok(NULL, delim); tok != NULL; tok = strtok(NULL, delim))
    opts[++argc] = tok;
  return argc;
}

// Mike's (Remove name before submission)
int Allocate (int* p, int bytes) {
  /* Allocate
      implicit list - header will contain size of block and allocation status
      first fit - search until first block that will fit

      The heap will be WORD aligned (4 bytes for 32 bit system), the first WORD
      of the header will be the size of the allocation and the status.
      The 2nd WORD will be the ID of the block. The 3rd WORD will be the size
      of the payload to prevent writeHeap() from exceeding the size of the
      payload. Then the payload and finally the buffer(unused) to align to
      WORD increments.

      Example: Allocate 6  [20|1][0][6][payload][buffer 2]
      First WORD tells us it is 20 bytes and 1 is allocated. Second WORD
      tells us it is block 0. Next WORD tells us the payload is 6 bytes.
      Then the actual data, followed by a buffer of 2 bytes to make it 12.
  */
  if (isZero(bytes)) return -1;
  int* end = p + (HEAPSIZE/4);            // pointer to end of heap

  // find free block
  int len = ((bytes + 12)%4 == 0) ? (bytes + 12) : ((((bytes + 12) >> 2) << 2) + 4);
  while ((p < end) &&                     // not passed end
        ((*p & 1) ||                      // already allocated
        (*p <= len)))                     // too small
    p = p + (*p & -2)/4;                  // goto next block (word addressed)

  // no room in heap
  if(p == end)
    return -1;

  // allocate
  int newsize = len;
  int oldsize = *p & -2;
  *p = newsize | 1;                      // set size & allocation
  *(p + 1) = ++bID;                      // set blockId
  *(p + 2) = bytes;                      // set payload
  if(newsize < oldsize)
    *(p + newsize/4) = oldsize - newsize;  // set next block

  #if DEBUG == 1
  printf("Header: Address[%p] ", p);
  // remove these debug lines before submission
  printf("Size[%i bytes] Allocated[%i] blockId[%i] payload[%i bytes]\n",
    *p & -2, *p & 1, *(p+1), *(p+2));
  #endif

  #if TESTING == 0
  printf("%i\n", bID);
  #endif
  return 0;
}

// Mike's (Remove name before submission)
int Free (int* p, int blockId) {
  if (isZero(blockId)) return -1;
  p = findBlockId(p, blockId);
  if(!p) return -1;
  *p = *p & -2;
  return 0;
}

int* findBlockId(int* p, int blockId) {
  /*
    helper function - give it pointer from start of heap and blockId
    will return pointer to block that contains the blockId or return 0
    if no block was found with that blockId
  */
  if (isZero(blockId)) return NULL;
  int* end = p + (HEAPSIZE/4);
  while((p < end) && *(p + 1) != blockId)
    p = p + (*p & -2)/4;
  if(p == end)
    p = 0;
  return p;
}

int blockList (int* heap) {
  /*
    reviewed the write up, we dont need a structure for this. blocklist will
    be called in main with the heap pointer. blockList will then parse the
    heap printing the information.

    see allocate comments for design of the heap
  */

  int* p = (int*)heap;                    // typecast to integer for management
  int* end = p + (HEAPSIZE/4);            // pointer to end of heap
  if (p == end)
    return -1;
  printf("Size\t\tAllocated\t\tStart\t\tEnd\n");
  for (; p + (*p & -2)/4 != end; p = p + (*p & -2)/4) {
    intptr_t payloadEnd = (long)p;
    payloadEnd += (*p & -2) - 1;
    printf("%i\t\t%i\t\t%p\t\t%p\n", *p & -2, *p & 1, p, (int*)payloadEnd); // just stuff :)
  }
  return 0;
}

void writeHeap(int* heap, int blockId, char content, int bytes)
{
	int* p = (int*)heap;
	char* insertionPointer;
    if((p = findBlockId(p, blockId)) == NULL) 
	{
		printf("Invalid blockId: %d\n", blockId);
		return;
	}
	insertionPointer = (char*)(&(p[3]));
	int i = 0;
	for(; i < bytes && i < p[2]; i++)
	{
	    insertionPointer[i] = content;
	}
}

char* printHeap(int* heap, int blockId, int bytes)
{
    int* p = (int*)heap;
	char* readPointer;
	char* end = (char*)p + HEAPSIZE;
    if((p = findBlockId(p, blockId)) == NULL) 
	{
		printf("Invalid blockId: %d\n", blockId);
		return;
	}
	readPointer = (char*)(&(p[3]));
	int i = 0;
	for(; i < bytes; i++)
	{
	    printf("%c", readPointer[i]);
	    if(&(readPointer[i]) == end)
	    {
	    	break;
	    }
	}
	printf("\n");
}

