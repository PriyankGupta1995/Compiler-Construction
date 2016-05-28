// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P

#include<stdio.h>
#include<stdlib.h>

#include "lexerDef.h"

extern FILE *getStream(FILE *fp, buffer B, buffersize k);

//innitializes the look up table for the grammar
extern void initLookUpTable();

//initializing the id to Token mapping
extern void inittoken_idToTokenName();

extern void computeState();


//does DFA state to token mapping
extern void initStateToToken();

extern FILE *getStream(FILE *fp, buffer B, buffersize k);

extern tokenInfo getNextToken(char *B,FILE* fp);
extern int charAllowed(char c);
