// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"


FILE *getStream(FILE *fp, buffer B, buffersize k)
{
	//printf("entered getStream\n");
	int i,flag=0;
	i=0;
	char ch;
	int buffer_index=0;
	//comment removal is done here where all characters following % are not included in buffer 
	//till new line char is not found
	while (buffer_index < k && ((ch=fgetc(fp)) != EOF))
	{
		if (ch == 10)
		{
			flag=0;
		}
		if (flag == 0)
		{
			B[buffer_index] = ch;
			buffer_index++;
		}
			
		if (B[buffer_index-1] == '%')
			flag=1;	

		
	}
	B[buffer_index]='\0';
	//printf("value of i is %d\n",i);
	return fp;
}

tokenInfo getNextToken(char *B, FILE* fp)
{
	int i;
		
	buffer_start_index = buffer_end_index ;
	//int current_state = 0;

	while (B[buffer_end_index] != '\0')
	{
		
		if(state[current_state][B[buffer_end_index]]>0 && state[current_state][B[buffer_end_index]]<=44)//noOfStates
		{	
			//printf("here\n");
			current_state = state[current_state][B[buffer_end_index]];
			temp.value[lexeme_index++] = B[buffer_end_index];

			buffer_end_index++;
	   }
	   else
		{	

			if(stateToToken[current_state] == -1)
			{
				temp.line_no = lnum;//change this .. find mechanism to incorporate line number
				temp.token_id = -50;
				temp.value[lexeme_index] = '\0';
				int i1=0;
				for (i1=0;temp.value[i1] != 0;i1++);
				if (i1 == 0)
				{
					printf("Unknown Symbol  <%c> at line <%d>\n",B[buffer_end_index],temp.line_no);
					buffer_end_index++;
					//lexer_errors++;
				}
				else
				{
					printf("Unknown pattern <%s> at line <%d>\n",temp.value,temp.line_no);
					//lexer_errors++;

				}
				return temp;

			}
			else
			{
				temp.line_no = lnum;
				temp.token_id = stateToToken[current_state];
				temp.value[lexeme_index] = '\0';
			
				//check if TK_FIELDID elements are in lookup table
				// 55 is no of rows of look up table or no of tokens in the grammar

				if (temp.token_id == TK_FIELDID)
				{
					for ( i=0; i<53 ; i++)
					{
						if (strcmp(lookup[i],temp.value) == 0)
						{
						temp.token_id = i;
						}
					}
				}

				else if (temp.token_id == TK_FUNID)
				{
				if (strcmp(lookup[TK_MAIN],temp.value) == 0)
					{
						temp.token_id = TK_MAIN;
					}
				}
				int i1;
				for (i1=0;temp.value[i1]!='\0';i1++);

				if (i1 > 30 && (temp.token_id ==TK_FUNID))
				{
					printf("Function identifier name at line %d is longer than the prescribed length of 30 characters\n",temp.line_no);
					//lexer_errors++;
					temp.token_id = -50;
				}
				if (i1 > 20 && (temp.token_id ==TK_ID))
				{
					printf("Identifier name at line %d is longer than the prescribed length of 20 characters\n",temp.line_no);
					//lexer_errors++;
					temp.token_id = -50;
				}

				//current_state = 0;
				
				return temp;
			}
		}//closing else		
		
		
	}
	
	
	if (strlen(B) < k)
	{
		if(stateToToken[current_state] == -1)
		{
			temp.line_no = lnum;//change this .. find mechanism to incorporate line number
				temp.token_id = -50;
				temp.value[lexeme_index] = '\0';
				int i1=0;
				for (i1=0;temp.value[i1] != 0;i1++);
				if (i1 == 0)
				{
					printf("Unknown Symbol  <%c> at line <%d>\n",B[buffer_end_index],temp.line_no);
					//lexer_errors++;
					buffer_end_index++;
				}
				else
				{
					printf("Unknown pattern <%s> at line <%d>\n",temp.value,temp.line_no);
					//lexer_errors++;

				}
				return temp;

		}
		else
		{
			temp.line_no = lnum;//change this .. find mechanism to incorporate line number
			temp.token_id = stateToToken[current_state];
			
			temp.value[lexeme_index] = '\0';
			
			//check if TK_FIELDID elements are in lookup table
			// 55 is no of rows of look up table or no of tokens in the grammar

			if (temp.token_id == TK_FIELDID)
			{
				for ( i=0; i<53 ; i++)
				{
					if (strcmp(lookup[i],temp.value) == 0)
					{
						temp.token_id = i;
					}
				}
			}

			else if (temp.token_id == TK_FUNID)
			{
				if (strcmp(lookup[TK_MAIN],temp.value) == 0)
				{
					temp.token_id = TK_MAIN;
				}
			}
			int i1;
				for (i1=0;temp.value[i1]!='\0';i1++);

				if (i1 > 30 && temp.token_id == TK_FUNID)
				{
					printf("Function identifier name at line %d is longer than the prescribed length of 30 characters\n",temp.line_no);
					//lexer_errors++;
					temp.token_id = -50;
				}
				if (i1 > 20 && temp.token_id == TK_ID)
				{
					printf("Identifier name at line %d is longer than the prescribed length of 20 characters\n",temp.line_no);
					//lexer_errors++;
					temp.token_id = -50;
				}


			//current_state = 0;
			return temp;

		}

	}

	fp=getStream(fp,B,k);
	buffer_start_index = 0;
	buffer_end_index = 0;

	temp = getNextToken(B,fp);
	

	return temp;

}

void initLookUpTable()
{
	int i;
	lookup=(char**)malloc(53*sizeof(char *));
	for (i=0;i<53;i++)
	{
		lookup[i]=(char *)malloc(15*sizeof(char));
		if (i != TK_WITH && i != TK_PARAMETERS && i!= TK_END && i != TK_WHILE && i != TK_INT && i != TK_REAL && i!= TK_TYPE && i != TK_MAIN &&
			i != TK_GLOBAL && i != TK_PARAMETER && i!= TK_LIST && i != TK_INPUT && i != TK_OUTPUT && i != TK_ENDWHILE && i !=TK_IF && i!= TK_THEN &&
			i != TK_ENDIF && i != TK_READ && i != TK_WRITE && i!= TK_RETURN && i != TK_CALL && i != TK_RECORD && i != TK_ENDRECORD && i!= TK_ELSE )
		{
			strcpy(lookup[i],"   ");
		}
	}
	//memset(lookup,'\0',sizeof(lookup));

	strcpy(lookup[TK_WITH] , "with");
	strcpy(lookup[TK_PARAMETERS] , "parameters");
	strcpy(lookup[TK_END] , "end");
	strcpy(lookup[TK_WHILE] , "while");
	strcpy(lookup[TK_INT] , "int");
	strcpy(lookup[TK_REAL] , "real");
	strcpy(lookup[TK_TYPE] , "type");
	strcpy(lookup[TK_MAIN] , "_main");
	strcpy(lookup[TK_GLOBAL] , "global");
	strcpy(lookup[TK_PARAMETER] , "parameter");
	strcpy(lookup[TK_LIST] , "list");
	strcpy(lookup[TK_INPUT] , "input");
	strcpy(lookup[TK_OUTPUT] , "output");
	strcpy(lookup[TK_ENDWHILE] , "endwhile");
	strcpy(lookup[TK_IF] , "if");
	strcpy(lookup[TK_THEN] , "then");
	strcpy(lookup[TK_ENDIF] , "endif");
	strcpy(lookup[TK_READ] , "read");
	strcpy(lookup[TK_WRITE] , "write");
	strcpy(lookup[TK_RETURN] , "return");
	strcpy(lookup[TK_CALL] , "call");
	strcpy(lookup[TK_RECORD] , "record");
	strcpy(lookup[TK_ENDRECORD] , "endrecord");
	strcpy(lookup[TK_ELSE] , "else");

}

void inittoken_idToTokenName()
{
	int i;
	//110 is max id for token
	tokenName=(char**)malloc(111*sizeof(char *));
	for (i=0;i<=110;i++)
	{
		tokenName[i]=(char *)malloc(30*sizeof(char));
		strcpy(tokenName[i],"TK_ERROR");
	}
	//Assumptions: maximum size of a token is 30 and number of token in grammar is 53

	strcpy(tokenName[0],"TK_ASSIGNOP");

	strcpy(tokenName[1],"TK_COMMENT");
	strcpy(tokenName[2],"TK_FIELDID");
	strcpy(tokenName[3],"TK_ID");
	strcpy(tokenName[4],"TK_NUM");
	strcpy(tokenName[5],"TK_RNUM");
	strcpy(tokenName[6],"TK_FUNID");
	strcpy(tokenName[7],"TK_RECORDID");
	strcpy(tokenName[8],"TK_WITH");
	strcpy(tokenName[9],"TK_PARAMETERS");
	strcpy(tokenName[10],"TK_END");
	strcpy(tokenName[11],"TK_WHILE");
	strcpy(tokenName[12],"TK_INT");
	strcpy(tokenName[13],"TK_REAL");
	strcpy(tokenName[14],"TK_TYPE");
	strcpy(tokenName[15],"TK_MAIN");
	strcpy(tokenName[16],"TK_GLOBAL");
	strcpy(tokenName[17],"TK_PARAMETER");
	strcpy(tokenName[18],"TK_LIST");
	strcpy(tokenName[19],"TK_SQL");
	strcpy(tokenName[20],"TK_SQR");
	strcpy(tokenName[21],"TK_INPUT");
	strcpy(tokenName[22],"TK_OUTPUT");
	strcpy(tokenName[23],"TK_SEM");
	strcpy(tokenName[24],"TK_COLON");
	strcpy(tokenName[25],"TK_DOT");
	strcpy(tokenName[26],"TK_ENDWHILE");
	strcpy(tokenName[27],"TK_OP");
	strcpy(tokenName[28],"TK_CL");
	strcpy(tokenName[29],"TK_IF");
	strcpy(tokenName[30],"TK_THEN");
	strcpy(tokenName[31],"TK_ENDIF");
	strcpy(tokenName[32],"TK_READ");
	strcpy(tokenName[33],"TK_WRITE");
	strcpy(tokenName[34],"TK_RETURN");
	strcpy(tokenName[35],"TK_PLUS");
	strcpy(tokenName[36],"TK_MINUS");
	strcpy(tokenName[37],"TK_MUL");
	strcpy(tokenName[38],"TK_DIV");	
	strcpy(tokenName[39],"TK_CALL");
	strcpy(tokenName[40],"TK_RECORD");
	strcpy(tokenName[41],"TK_ENDRECORD");
	strcpy(tokenName[42],"TK_ELSE");
	strcpy(tokenName[43],"TK_AND");
	strcpy(tokenName[44],"TK_OR");
	strcpy(tokenName[45],"TK_NOT");
	strcpy(tokenName[46],"TK_LT");
	strcpy(tokenName[47],"TK_LE");
	strcpy(tokenName[48],"TK_EQ");
	strcpy(tokenName[49],"TK_GT");
	strcpy(tokenName[50],"TK_GE");
	strcpy(tokenName[51],"TK_NE");
	strcpy(tokenName[52],"TK_COMMA");
	strcpy(tokenName[53],"TK_DOLLAR");
	strcpy(tokenName[54],"TK_EPSILON");
	

	strcpy(tokenName[60],"<program>");
	strcpy(tokenName[61],"<otherFunctions>");
	strcpy(tokenName[62],"<mainFunction>");
	strcpy(tokenName[63],"<stmts>");
	
	strcpy(tokenName[64],"<function>");
	strcpy(tokenName[65],"<input_par>");
	strcpy(tokenName[66],"<output_par>");
	strcpy(tokenName[67],"<parameter_list>");
	strcpy(tokenName[68],"<dataType>");
	strcpy(tokenName[69],"<remaining_list>");
	strcpy(tokenName[70],"<primitiveDatatype>");
	strcpy(tokenName[71],"<constructedDatatype>");
	
	
	strcpy(tokenName[72],"<typeDefinitions>");
	strcpy(tokenName[73],"<declarations>");
	strcpy(tokenName[74],"<otherStmts>");
	strcpy(tokenName[75],"<returnStmt>");

	strcpy(tokenName[76],"<typeDefinition>");

	strcpy(tokenName[77],"<fieldDefinitions>");
	strcpy(tokenName[78],"<fieldDefinition>");
	strcpy(tokenName[79],"<moreFields>");
	
	
	strcpy(tokenName[80],"<global_or_not>");
	strcpy(tokenName[81],"<declaration>");
	strcpy(tokenName[82],"<stmt>");
	strcpy(tokenName[83],"<assignmentStmt>");
	strcpy(tokenName[84],"<iterativeStmt>");
	strcpy(tokenName[85],"<conditionalStmt>");
	strcpy(tokenName[86],"<ioStmt>");

	strcpy(tokenName[87],"<funCallStmt>");

	strcpy(tokenName[88],"<singleOrRecId>");
	strcpy(tokenName[89],"<new_24>");
	strcpy(tokenName[90],"<arithmeticExpression>");

	
	strcpy(tokenName[91],"<outputParameters>");
	strcpy(tokenName[92],"<inputParameters>");
	strcpy(tokenName[93],"<idList>");
	strcpy(tokenName[94],"<booleanExpression>");
	
	
	strcpy(tokenName[95],"<elsePart>");
	
	strcpy(tokenName[96],"<allVar>");
	strcpy(tokenName[97],"<var>");
	strcpy(tokenName[98],"<all>");
	strcpy(tokenName[99],"<new_32>");
	
	strcpy(tokenName[100],"<expPrime>");
	strcpy(tokenName[101],"<lowPrecedenceOperators>");
	strcpy(tokenName[102],"<term>");
	strcpy(tokenName[103],"<highPrecedenceOperators>");
	strcpy(tokenName[104],"<factor>");
	strcpy(tokenName[105],"<termPrime>");
	strcpy(tokenName[106],"<logicalOp>");
	strcpy(tokenName[107],"<relationalOp>");
	strcpy(tokenName[108],"<optionalReturn>");
	strcpy(tokenName[109],"<more_ids>");
	strcpy(tokenName[110],"<new_34>");
}
	

void computeState()
{
	int i;
	memset(state,-1,sizeof(state));
	//state arrar is state
	state[0]['-']=1;
	state[0]['+']=2;
	state[0]['.']=3;
	state[0][':']=4;
	state[0][';']=5;
	state[0]['%']=6;
	state[0][']']=7;
	state[0]['[']=8;
	state[0]['(']=9;
	state[0][')']=10;
	state[0]['*']=11;
	state[0]['/']=12;
	state[0]['~']=13;
	state[0][',']=14;
	state[0]['!']=15;	
	state[15]['=']=16;
	state[0]['=']=17;
	state[17]['=']=18;
	state[0]['&']=19;
	state[19]['&']=20;
	state[20]['&']=21;
	state[0]['@']=22;
	state[22]['@']=23;
	state[23]['@']=24;
	state[0]['>']=25;
	state[25]['=']=26;
	state[0]['<']=27;
	state[27]['-']=28;
	state[28]['-']=29;
	state[29]['-']=30;
	state[27]['=']=31;
	//(i+'0') ensures value stores at ASCII index of digit
	for (i='0';i<='9';i++)
	{
		state[0][i]=32;
		state[32][i]=32;
	}

	state[32]['.']=33;

	//state 34,35
	for (i='0';i<='9';i++)
	{
		state[33][i]=34;
		state[34][i]=35;
	}

	//state 36,37
	state[0]['#']=36;

	for (i='a';i<='z';i++){
		state[36][i]=37;
		state[37][i]=37;
		}

	//38,39,40
	state[0]['_']=38;

	for (i='a';i<='z';i++)
	{
		state[38][i]=39;
		state[39][i]=39;
	}

	for (i='A';i<='Z';i++)
	{
		state[38][i]=39;
		state[39][i]=39;
	}

	for (i='0';i<='9';i++)
	{
		state[39][i]=40;
		state[40][i]=40;
	}

	//state 41

	state[0]['a']=41;
	
	for (i='e';i<='z';i++)
		state[0][i]=41;

	for (i='a';i<='z';i++)
	{
		state[41][i]=41;
		state[42][i]=41;
	}

	//state 42
	state[0]['b']=42;
	state[0]['c']=42;
	state[0]['d']=42;


	//state 43
	for (i='2';i<='7';i++)
	{
		state[42][i]=43;
	}

	state[43]['b']=43;
	state[43]['c']=43;
	state[43]['d']=43;

	//state 44
	for (i='2';i<='7';i++)
	{
		state[43][i]=44;
		state[44][i]=44;
	}



}//closing computeState

void initStateToToken()
{
	memset(stateToToken,-1,sizeof(stateToToken));
	stateToToken[1]=TK_MINUS;
	stateToToken[2]=TK_PLUS;
	stateToToken[3]=TK_DOT;
	stateToToken[4]=TK_COLON;
	stateToToken[5]=TK_SEM;
	stateToToken[6]=TK_COMMENT;
	stateToToken[7]=TK_SQR;
	stateToToken[8]=TK_SQL;
	stateToToken[9]=TK_OP;
	stateToToken[10]=TK_CL;
	stateToToken[11]=TK_MUL;
	stateToToken[12]=TK_DIV;
	stateToToken[13]=TK_NOT;
	stateToToken[14]=TK_COMMA;
	stateToToken[16]=TK_NE;
	stateToToken[18]=TK_EQ;
	stateToToken[21]=TK_AND;
	stateToToken[24]=TK_OR;
	stateToToken[25]=TK_GT;
	stateToToken[26]=TK_GE;
	stateToToken[27]=TK_LT;
	stateToToken[30]=TK_ASSIGNOP;
	stateToToken[31]=TK_LE;
	stateToToken[32]=TK_NUM;
	stateToToken[35]=TK_RNUM;
	stateToToken[37]=TK_RECORDID;
	stateToToken[39]=TK_FUNID;
	stateToToken[40]=TK_FUNID;
	stateToToken[41]=TK_FIELDID;
	stateToToken[42]=TK_FIELDID;
	stateToToken[43]=TK_ID;
	stateToToken[44]=TK_ID;


}
