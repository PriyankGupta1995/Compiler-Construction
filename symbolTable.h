// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexerDef.h"
#include "parserDef.h"
#include "symbolTableDef.h"

extern symbol_table_ptr init_symbol_table(abstractSyntaxTree A);
extern funcptr init_func_symbol_table(int i,symbol_table_ptr st,abstractSyntaxTree funca);
extern funcptr init_main_function_table(int i,symbol_table_ptr st,abstractSyntaxTree funca);
extern identifier_list add_to_table(abstractSyntaxTree a2,int i,symbol_table_ptr st);
extern void print_symbol_table(symbol_table_ptr st);
extern symbol_table_ptr init_offset(symbol_table_ptr st);

extern int chk_in_function1(identifier_list k,int scope,symbol_table_ptr st,int chkglobal,funcptr funcvar);
extern int chk_in_function(identifier_list k,int scope,symbol_table_ptr st,int chkglobal,funcptr funcvar);
extern int getRecordOffset(symbol_table_ptr st,identifier_list temp);


extern int type_function_var1(symbol_table_ptr st,identifier_list temp,int scope);
extern void get_record_name(symbol_table_ptr st,identifier_list temp,int scope);



extern void print_sem_errors();

extern int getRecordType(symbol_table_ptr st,identifier_list k);
extern int chk_in_main_function(identifier_list k,int scope,symbol_table_ptr st,int chkglobal,funcptr funcvar);
