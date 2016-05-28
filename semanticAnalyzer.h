// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexerDef.h"
#include "parserDef.h"
#include "symbolTableDef.h"

extern int error_semantics(symbol_table_ptr st,abstractSyntaxTree ast);
extern int error_declaration_multiple(symbol_table_ptr st);
extern int error_function_name_multiple(symbol_table_ptr st);
extern int error_if_global_no_other_declaration(symbol_table_ptr st);
extern int error_check_for_par_func(symbol_table_ptr st,abstractSyntaxTree ast);
extern int getScope(symbol_table_ptr st,abstractSyntaxTree ast);
extern identifier_list getReturnIdList(symbol_table_ptr st,abstractSyntaxTree ast,int scope);


extern char* getRecordName(char arr[200],symbol_table_ptr st);


extern int getTypeMainOut(abstractSyntaxTree t3,symbol_table_ptr st);

extern int check_funidStmt(abstractSyntaxTree t2,abstractSyntaxTree ast,symbol_table_ptr st,int scope1);

extern void printId(identifier_list temp);

extern identifier_list getWhileList(symbol_table_ptr st,abstractSyntaxTree ast,int scope);
extern identifier_list getBool(symbol_table_ptr st,abstractSyntaxTree ast,int scope);


extern int chk_while(identifier_list temp,abstractSyntaxTree ast,int scope);
