// Michael R. Callan III 12912150
// James Albert III 16004325
// John Reissmueller 29856616

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Definitions
#define MAXBLOCKS 128 // maximum amount of blocks
#define MAXARGS 128   // maximum amount of arguments
#define MAXCMD 255    // maximum length of command

// Prototypes
int parsecommand(char*, char* opts[MAXARGS]);
int Allocate(int);
void Free(int);
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

// Mike's
int main () {

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

// Mikes
int Allocate (int bytes) {

  return 0;
}

// Mikes
void Free (int blockId) {

  return;
}

struct Block* blockList () {
  static struct Block blocks[MAXBLOCKS];
  return blocks;
}

// Johns
void writeHeap (int blockId, char content, int bytes) {

}

// Johns
char* printHeap (int blockId, int bytes) {
  char* lololol = "skadoosh";
  return lololol;
}

void quit() {

}
