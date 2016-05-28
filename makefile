
all:
	gcc -g driver.c lexer.c parser.c symbolTable.c typechecker.c semanticAnalyzer.c ircode.c codegen.c -o ./toycompiler
