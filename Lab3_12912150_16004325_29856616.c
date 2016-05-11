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
int Allocate(void*, int);
void Free(void* heap, int);
struct Block* blockList();
void writeHeap(int, char, int);
char* printHeap(int, int);
void quit();

// Data Type Definitions
struct Block {
  int size;
  char allocated;
  void* start;
  void* end;
};

// Mike's (Remove name before submission)
int main () {
  // variable declaration
  char command[MAXCMD];
  void* heap = malloc(HEAPSIZE);

  // memory management
  *((int*)heap) = HEAPSIZE;

  // Loop - fetch command, call functions
  while(1) {
    printf("> ");
    gets(command);

    if(strcmp(command, "quit") == 0) {
      break;
    }

    // Call functions

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
int Allocate (void* heap, int bytes) {
  /* Allocate
      implicit list - header will contain size of block and allocation status
      first fit - search until first block that will fit

      The heap will be word aligned (4 bytes for 32 bit system), the first byte
      of the header will be the size of the allocation and the status.
      The 2nd byte will be the ID of the block. The 3rd byte will be the size
      of the payload to prevent writeHeap() from exceeding the size of the
      payload. Then the payload and finally the buffer(unused) to align to
      word increments.

      Example: Allocate 5  [12|1][0][6][payload][buffer 3]
      First byte tells us it is size 12 chunk and 1 is allocated. Second byte
      tells us it is block 0. Next byte tells us the payload is 6 bytes.
      Then the actual data, followed by a buffer of 3 to make it 12.
  */
  void* end = heap + 400;

  // find free block

  // allocate

  return 0;
}

// Mike's (Remove name before submission)
void Free (void* heap, int blockId) {
  // find block

  // free block

  return;
}

struct Block* blockList () {
  static struct Block blocks[MAXBLOCKS];
  return blocks;
}

// Johns (Remove name before submission)
void writeHeap (int blockId, char content, int bytes) {

}

// Johns (Remove name before submission)
char* printHeap (int blockId, int bytes) {
  char* lololol = "skadoosh";
  return lololol;
}

void quit() {

}
