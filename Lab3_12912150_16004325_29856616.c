// Michael R. Callan III 12912150
// James Albert III 16004325
// John Reissmueller 29856616

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Definitions
#define MAXBLOCKS   128   // maximum amount of blocks
#define MAXARGS     128   // maximum amount of arguments
#define MAXCMD      255   // maximum length of command
#define HEAPSIZE    400   // size of heap

// Prototypes
int parsecommand(char*, char* opts[MAXARGS]);
void Allocate(void*, int);
void Free(void* heap, int);
void blockList(void*);
void writeHeap(void*, int, char, int);
char* printHeap(void*, int, int);
void quit();

// Mike's (Remove name before submission)
int main () {
  // variable declaration
  char command[MAXCMD];
  void* heap = malloc(HEAPSIZE);
  char* argv[MAXARGS];
  int argc;

  // memory management
  *(int*)heap = HEAPSIZE;

  // Loop - fetch command, call functions
  while(1) {
    printf("> ");
    gets(command);

    argc = parsecommand(command, argv);

    if(strcmp(argv[0], "allocate") == 0) {
      Allocate(heap, atoi(argv[1]));
    }
    else if(strcmp(argv[0], "free") == 0) {
      Free(heap, atoi(argv[1]));
    }
    else if(strcmp(argv[0], "blocklist") == 0) {
      blockList(heap);
    }
    else if(strcmp(argv[0], "writeheap") == 0) {
      writeHeap(heap, atoi(argv[1]), argv[2], atoi(argv[3]));
    }
    else if(strcmp(argv[0], "printheap") == 0) {
      printHeap(heap, atoi(argv[1]), atoi(argv[2]));
    }
    else if(strcmp(argv[0], "quit") == 0) {
      break;
    }
  }
  return 0;
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
void Allocate (void* heap, int bytes) {
  /* Allocate
      implicit list - header will contain size of block and allocation status
      first fit - search until first block that will fit

      The heap will be DWORD aligned (4 bytes for 32 bit system), the first byte
      of the header will be the size of the allocation and the status.
      The 2nd byte will be the ID of the block. The 3rd byte will be the size
      of the payload to prevent writeHeap() from exceeding the size of the
      payload. Then the payload and finally the buffer(unused) to align to
      DWORD increments.

      Example: Allocate 5  [12|1][0][6][payload][buffer 3]
      First byte tells us it is size 12 chunk and 1 is allocated. Second byte
      tells us it is block 0. Next byte tells us the payload is 6 bytes.
      Then the actual data, followed by a buffer of 3 to make it 12.
  */
  int* p = (int*)heap;
  int* end = p + 400;

  static int bID = 0;                       // block ID for new blocks

  printf("p = %p\n", p);

  // find free block
  while ((p < end) && ((*p & 1) || (*p <= (bytes + 3)))) {
    printf("Header: Address[0x%p] ", p);
    printf("Size[%i] Allocated[%i] blockId[%i] payload[%i]\n",
      *p & -2, *p & 1, *(p+1), *(p+2));
    printf("p = %p\n", p);
    p = p + (*p & -2);
    printf("p = %p\n", p);
  }

  // // no room in heap
  // if(p == end)
  //   return -1;

  // allocate
  int newsize = ((bytes + 3)%4 == 0) ?
    bytes + 3 : (((bytes + 3) >> 2) << 2) + 4;    // round up increment 4
  printf("newsize = %i\n", newsize);
  int oldsize = *p & -2;
  *p = newsize | 1;                      // set size & allocation
  *(p + 1) = ++bID;                      // set blockId
  *(p + 2) = bytes;                      // set payload
  if(newsize < oldsize)
    *(p + newsize) = oldsize - newsize;  // set next block

  printf("Header: Address[0x%p] ", p);
  printf("Size[%i] Allocated[%i] blockId[%i] payload[%i]\n",
    *p & -2, *p & 1, *(p+1), *(p+2));
  printf("%i\n", bID);
  return;
}

// Mike's (Remove name before submission)
void Free (void* heap, int blockId) {
  // find block

  // free block

  return;
}

void blockList (void* heap) {
  /*
    reviewed the write up, we dont need a structure for this. blocklist will
    be called in main with the heap pointer. blockList will then parse the
    heap printing the information.

    see allocate comments for design of the heap
  */

  return;
}

// Johns (Remove name before submission)
void writeHeap (void* heap, int blockId, char content, int bytes) {

}

// Johns (Remove name before submission)
char* printHeap (void* heap, int blockId, int bytes) {
  char* lololol = "skadoosh";
  return lololol;
}

void quit() {

}
