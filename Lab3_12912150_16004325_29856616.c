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
void Allocate(char*, int);
void Free(char* heap, int);
void blockList(char*);
void writeHeap(char*, int, char, int);
char* printHeap(char*, int, int);

// Mike's (Remove name before submission)
int main () {
  // variable declaration
  char command[MAXCMD];
  char* heap = malloc(HEAPSIZE);
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

  free(heap);
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
void Allocate (char* heap, int bytes) {
  /* Allocate
      implicit list - header will contain size of block and allocation status
      first fit - search until first block that will fit

      The heap will be WORD aligned (4 bytes for 32 bit system), the first byte
      of the header will be the size of the allocation and the status.
      The 2nd byte will be the ID of the block. The 3rd byte will be the size
      of the payload to prevent writeHeap() from exceeding the size of the
      payload. Then the payload and finally the buffer(unused) to align to
      WORD increments.

      Example: Allocate 6  [12|1][0][6][payload][buffer 3]
      First WORD tells us it is size 12 chunk and 1 is allocated. Second WORD
      tells us it is block 0. Next WORD tells us the payload is 6 bytes.
      Then the actual data, followed by a buffer of 3 bytes to make it 12.
  */
  static int bID = 0;                     // block ID for new blocks
  int* p = (int*)heap;                    // typecast to integer for management
  int* end = p + (HEAPSIZE/4);            // pointer to end of heap

  // find free block
  int len = ((bytes + 12)%4 == 0) ? (bytes + 12) : ((((bytes + 12) >> 2) << 2) + 4);
  while ((p < end) &&                     // not passed end
        ((*p & 1) ||                      // already allocated
        (*p <= len))) {                   // too small
    int offset = *p;
    offset = offset & -2;
    offset = offset/4;
    p = p + offset;                  // goto next block (word addressed)
  }

  // no room in heap
  if(p == end)
    return;

  // allocate
  int newsize = len;
  int oldsize = *p & -2;
  *p = newsize | 1;                      // set size & allocation
  *(p + 1) = ++bID;                      // set blockId
  *(p + 2) = bytes;                      // set payload
  if(newsize < oldsize)
    *(p + newsize/4) = oldsize - newsize;  // set next block

  printf("Header: Address[0x%p] ", p);
  printf("Size[%i bytes] Allocated[%i] blockId[%i] payload[%i bytes]\n",
    *p & -2, *p & 1, *(p+1), *(p+2));

  printf("%i\n", bID);
  return;
}

// Mike's (Remove name before submission)
void Free (char* heap, int blockId) {
  // find block

  // free block

  return;
}

void blockList (char* heap) {
  /*
    reviewed the write up, we dont need a structure for this. blocklist will
    be called in main with the heap pointer. blockList will then parse the
    heap printing the information.

    see allocate comments for design of the heap
  */

  return;
}

// Johns (Remove name before submission)
void writeHeap (char* heap, int blockId, char content, int bytes) {

}

// Johns (Remove name before submission)
char* printHeap (char* heap, int blockId, int bytes) {
  char* lololol = "skadoosh";
  return lololol;
}

void quit() {

}
