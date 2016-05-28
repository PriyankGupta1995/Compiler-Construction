// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexerDef.h"
#include "parserDef.h"
#include "symbolTableDef.h"

extern int error_type_checking_ast(symbol_table_ptr st,abstractSyntaxTree ast);
extern int error_type_checking_function(symbol_table_ptr st,abstractSyntaxTree temp,int scope);
extern int type_function_var(symbol_table_ptr st,abstractSyntaxTree ast,int scope);
extern int type_record_var(symbol_table_ptr st,abstractSyntaxTree ast,int scope);

extern int getTypeArithmeticExpression(symbol_table_ptr st,abstractSyntaxTree ast,int scope);
extern int error_type_checking_stmt(symbol_table_ptr st,abstractSyntaxTree t1,int scope);
extern int checkTypeBooleanExpression(symbol_table_ptr st,abstractSyntaxTree ast,int scope);
extern int checkTypeVar(symbol_table_ptr st,abstractSyntaxTree ast,int scope);


extern char* getRecN(abstractSyntaxTree ast,symbol_table_ptr st,int scope);

extern int getAllRecType(abstractSyntaxTree ast,symbol_table_ptr st,int scope);
