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
#define TESTING     0     // 1 - main disabled, 0 - main enabled (submission)
#define DEBUG       0     // 1 - debug shown, 0 - debug hidden

// Prototypes
int Allocate(int*, int);
int Free(int* heap, int);
int blockList(int*);
int writeHeap(int*, int, char, int);
int printHeap(int*, int, int);
int parsecommand(char*, char* opts[MAXARGS]);
int* findBlockId(int*, int);
bool isZero(int);
bool nextInHeap(int*, int*);
bool isAllocated(int*);
bool isValid(int*, int);
void nextBlock(int**);
bool hasBlockId(int*, int);

// Global variables
static int bID;


// Main function
#if TESTING == 0
int main () {
  // variable declaration
  char command[MAXCMD];
  int* heap = malloc(HEAPSIZE);
  char* argv[MAXARGS];

  // memory management
  *heap = HEAPSIZE;
  bID = 0;

  // Loop - fetch command, call functions
  while(1) {
     printf("> ");
     gets(command);
     parsecommand(command, argv);

     if(strcmp(argv[0], "allocate") == 0)
       Allocate(heap, atoi(argv[1]));
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


/*
    FUNCTIONS
*/

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
        (*p < len)))                     // too small
    nextBlock(&p);                  // goto next block (word addressed)

  // no room in heap
  if(p >= end)
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

int Free (int* p, int blockId) {
  if (isZero(blockId)) return -1;
  p = findBlockId(p, blockId);
  if(!p) return -1;
  *p = *p & -2;
  return 0;
}

int blockList (int* p) {
  int* end = p + (HEAPSIZE/4);            // pointer to end of heap
  if (p == end)
    return -1;
  printf("Size\t\tAllocated\tStart\t\t\tEnd\n");
  for (; nextInHeap(p, end) ; nextBlock(&p)) {
    intptr_t payloadEnd = (long)p;
    payloadEnd += (*p & -2) - 1;
    printf("%i\t\t%i\t\t%p\t\t%p\n", *p & -2, *p & 1, p, (int*)payloadEnd); // just stuff :)
  }
  return 0;
}

int writeHeap(int* p, int blockId, char content, int bytes) {
  if (isZero(blockId) || isZero(bytes)) return -1;
  char* insertionPointer;
  if(!(isValid(p, blockId) && isAllocated(p))) {
    #if TESTING == 0
    printf("Invalid blockId: %d\n", blockId);
    #endif
    return -1;
  }
  insertionPointer = (char*)(&(p[3]));
  int i;
  for(i = 0; i < bytes && i < p[2]; i++) {
    insertionPointer[i] = content;
  }
  return 0;
}

int printHeap(int* p, int blockId, int bytes) {
  if (isZero(blockId) || isZero(bytes)) return -1;
  char* readPointer;
  char* end = (char*)p + HEAPSIZE;
  if(!(isValid(p, blockId) && isAllocated(p))) {
    #if TESTING == 0
    printf("Invalid blockId: %d\n", blockId);
    #endif
    return -1;
  }
  readPointer = (char*)(&(p[3]));
  int i;
  for(i = 0; i < bytes && i < p[2]; i++) {
    printf("%c", readPointer[i]);
    if(&(readPointer[i]) == end) {
      break;
    }
  }
  printf("\n");
  return 0;
}


/*
    HELPER FUNCTIONS
*/

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

int* findBlockId(int* p, int blockId) {
  /*
    helper function - give it pointer from start of heap and blockId
    will return pointer to block that contains the blockId or return 0
    if no block was found with that blockId
  */
  if (isZero(blockId)) return NULL;
  int* end = p + (HEAPSIZE/4);
  while((p < end) && !hasBlockId(p, blockId))
    nextBlock(&p);
  if(p == end)
    p = 0;
  return p;
}

bool isZero(int param) {
  return param == 0;
}

bool nextInHeap(int* p, int* end) {
  return p + (*p & -2)/4 != end;
}

void nextBlock(int** p) {
  *p = *p + (**p & -2)/4;
}

bool hasBlockId(int* p, int blockId) {
  return *(p + 1) == blockId;
}

bool isAllocated(int* p) {
  return (*p & 1) == 1;
}

bool isValid(int* p, int blockId) {
  return (p = findBlockId(p, blockId)) != NULL;
}
