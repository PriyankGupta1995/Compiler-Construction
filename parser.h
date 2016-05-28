// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P


#include <stdio.h>
#include <stdlib.h>
#include "parserDef.h"

extern void createParseTable(grammar G);

extern grammar initGrammar(FILE* fp);

//extern void inittoken_idToTokenName();
//above function is already called and defined in lexer files


extern grammar initFirstFollow(FILE* fp1,FILE* fp2, grammar G);

extern grammar firstOfRule(grammar G);

extern int getId(char* tempstr);

extern stack initialise_stack();
extern stack stack_push(stack s,int value);
extern int stack_top(stack s);
extern stack stack_pop(stack s);

extern parseTree initialise_parsetree();
extern parseTree create(parseTree pt,int nt ,int value,int currentRuleNo);
extern parseTree nextNode(parseTree pt);
extern parseTree root(parseTree pt);
extern parseTree  parseInputSourceCode(char *testcaseFile, int T[50][54], grammar G, tokenInfo *tokenList, int no_of_tokens);

//extern void dfs(parseTree pt,FILE *filep);
//extern void printParseTree(parseTree  pt, char *outfile);

extern abstractSyntaxTree initialise_AST();
extern abstractSyntaxTree add_to_AST(abstractSyntaxTree ast, parseTree pt);
extern abstractSyntaxTree reduction(abstractSyntaxTree ast, parseTree pt);
extern abstractSyntaxTree generateAST(abstractSyntaxTree ast ,parseTree pt);
extern abstractSyntaxTree copyContents(abstractSyntaxTree ptr1,abstractSyntaxTree ptr2);


extern void printParseTree(parseTree  pt, char *outfile,int print_yes,int *nodes,int *mem);
extern void dfs(parseTree pt,FILE *filep,int print_yes,int *nodes,int *mem);


extern void print_parse_errors();

extern int get_parse_errors();
