// Michael R. Callan III 12912150
// James Albert III 16004325
// John Reissmueller 29856616

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXBLOCKS 128 // maximum amount of blocks
#define MAXARGS 128   // maximum amount of arguments
#define MAXCMD 255    // maximum length of command

int parsecommand(char*, char* opts[MAXARGS]);
int Allocate(int);
void Free(int);
struct BlockList* blockList();
void writeHeap(int, char, int);
char* printHeap(int, int);
void quit();

struct BlockList {
  int size;
  char allocated;
  void* start;
  void* end;
};

int main () {

  return 0;
}


int allocate () {

  return 0;
}

void free () {

  return;
}

struct BlockList blockList () {
  struct BlockList toReturn;


  return toReturn;
}

void writeHeap () {

}
