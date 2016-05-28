// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P
#include "typechecker.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//check if record id needs to be handles in arithmetic expression function

int error_type_checking_ast(symbol_table_ptr st,abstractSyntaxTree ast)
{
	abstractSyntaxTree tempNode = ast->child[0];
	int type_errors=0;
	int scope =0;

	while (tempNode-> symbol != TK_EPSILON)
	{
		type_errors += error_type_checking_function(st,tempNode->child[0]->child[2]->child[2],scope);//<other_stmts>
		scope++;
		tempNode = tempNode->child[1];

	}
	//printf(" type errors after function errors: %d\n",type_errors);

	type_errors += error_type_checking_function(st,ast->child[1]->child[0]->child[2],scope);
	return type_errors;
}

int error_type_checking_function(symbol_table_ptr st,abstractSyntaxTree temp,int scope)
{

	//printf("scope of function: %d\n",scope,temp->value);
	int type_errors = 0;
	int type1,type2,type3;

	while (temp->symbol != TK_EPSILON)
	{
		//printf("saurabh 1\n");
		abstractSyntaxTree t1 = temp->child[0];//<stmt>		
		type_errors += error_type_checking_stmt(st,t1,scope);
		temp = temp -> child[1];

		//also check for boolean expression : remark done
	}

	return type_errors;
}

int type_function_var(symbol_table_ptr st,abstractSyntaxTree ast,int scope) //abs is TKID
{
	//printf("compiler\n");
	//checks for type of id in func inputpar, output par , function ids and global ids

	identifier_list tmp = st->head;

	while (tmp != NULL)
	{
		if (strcmp(ast->value,tmp->id_name) == 0)
		{
			return tmp->id_type;
		}
		tmp = tmp -> next;
	}
	funcptr funcvar;
	funcvar = st->array[scope];
	tmp = funcvar -> head;
	while (tmp != NULL)
	{
		if (strcmp(ast->value,tmp->id_name)==0)
		{
			return tmp->id_type;
		}
		tmp = tmp->next;
	}
	tmp = funcvar -> input_par;
	while (tmp != NULL)
	{
		if (strcmp(ast->value,tmp->id_name) == 0)
		{
			return tmp->id_type;
		}
		tmp = tmp -> next;
	}
	tmp = funcvar -> output_par;
	while (tmp != NULL)
	{
		if (strcmp(ast->value,tmp->id_name) == 0)
		{
			return tmp->id_type;
		}
		tmp = tmp -> next;
	}

	return -1; //-1 means no variable declaration found in symbol table

}

int type_record_var(symbol_table_ptr st,abstractSyntaxTree ast,int scope) //ast is TK_ID whose child is TK_FIELDID
{
	////////////////////////////////
	//we need record name corresponding to a particular record id
	//printf("entered : %s\n",ast->value);

	funcptr funcvar = st->array[scope];
	//printf("funtion name is : %s\n",funcvar->func_name);
	identifier_list t5 = funcvar -> head;
	char tempstr[200];
	int success = 0;
	while (t5 != NULL)
	{
		
		//printf("the variable type is %d and tk_id is %s\n",t5->id_type,t5->id_name);
		if (t5->id_type == 2)// 2 is record id
		{
			if (strcmp(t5->id_name,ast->value) == 0)
			{
				
				strcpy(tempstr,t5->record_name);
				success = 1;
			}
		}
		t5 = t5->next;
	}
	// if no success search in input and output par
	//printf("record name is : %s\n",tempstr);
	t5 = funcvar ->input_par;
	if (success == 0)
	{
	while (t5 != NULL)
	{
		
		//printf("the variable type is %d and tk_id is %s\n",t5->id_type,t5->id_name);
		if (t5->id_type == 2)// 2 is record id
		{
			if (strcmp(t5->id_name,ast->value) == 0)
			{
				
				strcpy(tempstr,t5->record_name);
				success = 1;
			}
		}
		t5 = t5->next;
	}

	}//closing if 

	//check in output par if success is still 0

	t5 = funcvar ->output_par;

	if (success == 0)
	{
	while (t5 != NULL)
	{
		
		//printf("the variable type is %d and tk_id is %s\n",t5->id_type,t5->id_name);
		if (t5->id_type == 2)// 2 is record id
		{
			if (strcmp(t5->id_name,ast->value) == 0)
			{
				
				strcpy(tempstr,t5->record_name);
				success = 1;
			}
		}
		t5 = t5->next;
	}

	}//closing if 

	t5 = st ->head;

	if (success == 0)
	{
	while (t5 != NULL)
	{
		
		//printf("the variable type is %d and tk_id is %s\n",t5->id_type,t5->id_name);
		if (t5->id_type == 2)// 2 is record id
		{
			if (strcmp(t5->id_name,ast->value) == 0)
			{
				
				strcpy(tempstr,t5->record_name);
				success = 1;
			}
		}
		t5 = t5->next;
	}

	}//closing if 



	if (success == 0)
		return -1; // since no declaration of given RECORDID



	//printf("the record name is : %s\n",tempstr);

	////////////////////////////////
	//printf("entered type_record_var with record name id : %s\n",ast->child[0]->value);
	/*funcvar;
	funcvar = st->array[scope];*/
	recptr tmp =st -> rp;
	while (tmp != NULL)
	{
		if (strcmp(tmp->record_name,tempstr) == 0)
		{
			//printf("record is is : %s\n",tempstr);
			identifier_list t1 = tmp-> head;
			while (t1 != NULL)
			{
				if (strcmp(t1->id_name,ast->child[0]->value) == 0)
				{
					return (t1->id_type);
				}
				t1 = t1->next;
			}
			return -1;
		}
		tmp = tmp->next;
	}
	return -1; //-1 means no variable declaration found in symbol table
}


int getTypeArithmeticExpression(symbol_table_ptr st,abstractSyntaxTree ast,int scope)
{
	//printf("%d and %s and child 1 is %s\n",ast->symbol,tokenName[ast->symbol],tokenName[ast->child[1]->symbol]);
	int type1 = -1,type2 = -1;
	
	if (ast->symbol == TK_EPSILON && ast->childnum == 0)
	{
		return TK_EPSILON;
	}
	/*else if (ast->symbol == TK_EPSILON && ast->childnum>0)
	{
		return getTypeArithmeticExpression(st,ast->child[0],scope);
	}*/
	else if (ast->symbol == TK_NUM)
	{
		return 0;// 0 is int
	}
	else if (ast->symbol == TK_RNUM)
	{
		return 1;// 1 is for real
	}
	else if (ast->symbol == TK_ID && ast->child[0]->symbol == TK_EPSILON)
	{
		//return type_function_var(st,ast,scope);
		int temporary = type_function_var(st,ast,scope);
		if (temporary == -1)
			{
				/*printf("Error: Variable <%s> at line no <%d> might not have been declared\n",ast->value,
				ast->line_no);*/
				printf("line %d : variable %s is not declared\n",ast->line_no,ast->value);
				return 20;// 20 implies variable not declared
			}
			return temporary;
			//return 20; // 20 implies variable not declared 

	}
	else if (ast->symbol == TK_ID && ast->child[0]->symbol == TK_FIELDID)
	{
		int temporary = type_record_var(st,ast,scope);
		if (temporary == -1)
			{
				printf("Line %d: Record field <%s.%s> is not declared\n",
				ast->child[0]->line_no,
				ast->value,
				ast->child[0]->value);
				return 20;
			}
		return temporary;
	}
	else if (ast->symbol == TK_PLUS || ast->symbol == TK_MINUS ||
	 ast->symbol == TK_MUL ||ast->symbol == TK_DIV || strcmp(tokenName[ast->symbol],"<term>") == 0 ||
	 strcmp(tokenName[ast->symbol],"<arithmeticExpression>") == 0)
	{
		//printf("%s:**********************\n",tokenName[ast->]);
		type1 = getTypeArithmeticExpression(st,ast->child[0],scope);
		type2 = getTypeArithmeticExpression(st,ast->child[1],scope);
		
		//printf("%s : TYPE1 is : %d and TYPE2 is : %d\n",tokenName[ast->symbol],type1,type2);
		if (type1 == 20 || type2 == 20)// 20 implies that some variable is not declared
			return 20;
		else if ((type1 == type2) && (type1 != -1) && type1 != 2)
		{
			//printf("the chosen one\n");
			//printf("return value: %d : %s\n",type1,tokenName[ast->symbol]);
			return type1;
		}
		else if ((type1 == type2) && (type1 == 2) && strcmp(getRecN(ast->child[0],st,scope),getRecN(ast->child[1],st,scope)) == 0)
		{
			return type1;
		}
		else if (type1 == TK_EPSILON && type2 != type1 && type2 != -1)
		{
			return type2;
		}
		else if (type2 == TK_EPSILON && type1 != type2 && type1 != -1)
		{
			//printf("here: type1 = :%d\n",type1);
			return type1;
		}
		else if (type1 == 2 && (ast->child[1]->symbol == TK_MUL || ast->child[1]->symbol == TK_DIV))
		{
			//int get= getAllRecType(ast->child[0],st,scope);
			//printf("get is : %d\n",get);
			int hi = getAllRecType(ast->child[0],st,scope);
			if (type2 == hi )
				return type1;//for case of record scalar mul and division
			else
			{
				char arr1[200];
				char arr2[200];
				abstractSyntaxTree t90 = ast->child[0];
				while (t90->symbol != TK_ID)
				{
					t90 = t90->child[0];
				}
				strcpy(arr1,t90->value);

				t90 = ast->child[1];
				while (t90 -> symbol != TK_ID)
				{
					t90 = t90->child[0];
				}
				strcpy(arr2,t90->value);
				printf("Line %d: Type of record <%s> fields does not match with type of <%s> \n",ast->child[1]->line_no,arr1,arr2);
				return 20;
			}
		}
		return -1;


	}

	return -1;
}


int error_type_checking_stmt(symbol_table_ptr st,abstractSyntaxTree t1,int scope)
{
	int type_errors = 0;
	int type1 = -1,type2 = -1,type3=-1;

	if (t1->symbol == TK_ASSIGNOP)
		{
			
			type1=-1,type2=-1;
			//check if left hand side of assignop is record fieldid or not
			if (t1->child[0]->symbol == TK_ID)
			{
				if (t1->child[0]->child[0]->symbol == TK_EPSILON)
				{
					//printf("saurabh 3\n");
					//semantic analyzer
					//check if TK_ID is declared
					type1 = type_function_var(st,t1->child[0],scope);

					if (type1 == -1)
					{
						/*printf("Error: Variable <%s> at line no <%d> might not have been declared\n",t1->child[0]->value,
							t1->child[0]->line_no);*/

						printf("line %d : variable %s is not declared\n",t1->child[0]->line_no,t1->child[0]->value);
					}
					//incorporate checking in function inputpar and output par
				}
				else
				{
					
					type1 = type_record_var(st,t1->child[0],scope);
					//printf("type1 is : %d\n",type1);
					//printf("hiadsf : %s\n",t1->child[0]->value);
					//printf("type 1 of record var is : %d and record name is %s and field name is : %s\n",type1,t1->child[0]->value,t1->child[0]->child[0]->value);
					if (type1 == -1)
					{
						printf("Line %d: Record field <%s.%s> is not declared\n",
							t1->child[0]->child[0]->line_no,
							t1->child[0]->value,
							t1->child[0]->child[0]->value);
					}

				}
			}
			//printf("left of assignop type1 is : %d\n",type1);

			//printf("check1****************************\n");
			//printf("%s\n",tokenName[t1->child[1]->symbol]);

			//printf("type1 is %d\n",type1);
			
			type2 = getTypeArithmeticExpression(st,t1->child[1],scope);
			//printf("type2 is right of assign op: %d\n",type2);

			if (type2 == 20 || type1 == -1);// don't print anything as variable is not declared
			else if ((type1 == type2) && (type1!=-1) && type1 != 2)
			{
				//no increment in errors
			}
			else if ((type1 == type2 ) && (type1 == 2) && strcmp(getRecN(t1->child[0],st,scope),getRecN(t1->child[1],st,scope)) == 0)
			{

				//no increment in errors
			}
			else
			{
				type_errors++;
				//printf("Error: type mismatch in line number : %d\n",t1->line_no);
				printf("line %d: type mismatch error\n",t1->line_no);
				//print some error report 
			}

		}//closing condition check for arithmetic expressions
		//check for boolean expression
		else if (t1->symbol == TK_WHILE)
		{
			type3 = checkTypeBooleanExpression(st,t1->child[0],scope);
			if (type3 != 4)//4 means boolean type
			{

				type_errors++;
				//printf("Error: type mismatch in line : %d\n",t1->line_no);
				printf("line %d: type mismatch error\n",t1->line_no);
			}
			type_errors += error_type_checking_stmt(st,t1->child[1],scope);
			type_errors += error_type_checking_function(st,t1->child[2],scope);

		}
		//else if (strcmp(tokenName[t1->symbol],"<conditionalStmt>") == 0)
		else if (t1->symbol == TK_IF)
		{
			//printf("entered conditional block\n");

			type3 = checkTypeBooleanExpression(st,t1->child[0],scope);
			//printf("returned type is : %d\n",type3);
			if (type3 != 4)//4 means boolean type
			{
				type_errors++;
				//printf("Error: Type mismatch at line  : %d\n",t1->line_no);
				printf("line %d: type mismatch error\n",t1->line_no);
				//printf("Error: Type mismatch at line : %d\n",t1->line_no);
			}
			type_errors += error_type_checking_stmt(st,t1->child[1],scope);
			type_errors += error_type_checking_function(st,t1->child[2],scope);
			if (t1->child[3]->symbol == TK_ENDIF);
			else
			{
				type_errors +=error_type_checking_stmt(st,t1->child[3]->child[0],scope);
				type_errors +=error_type_checking_function(st,t1->child[3]->child[1],scope);
			}

		}
		else if (t1->symbol == TK_FUNID)
		{
			//check if this funid exists and is not equal to funid of calling function
			if (strcmp(st->array[scope]->func_name,t1->value) == 0)
			{
				printf("Error: Recursive function call to <%s> at line number <%d>\n",t1->value,t1->line_no);
			}
			else
			{
				//confirm whether it exists some
			}
		}


		return type_errors;

}



int checkTypeBooleanExpression(symbol_table_ptr st,abstractSyntaxTree ast,int scope)
{
	int type3 = -1;
	int type4 = -1;
	if (ast->symbol == TK_AND || ast->symbol == TK_OR)
	{
		//printf("in and or block\n");
		type3 = checkTypeBooleanExpression(st,ast->child[0],scope);
		type4 = checkTypeBooleanExpression(st,ast->child[1],scope);
		if (type3 == 4 && type4 == 4)
		{
			return 4;// 4 stands for boolean variable
		}
		else
		{
			//printf("type mismatch at line : %d \n",ast->line_no);
			return -1;
		}

	}
	else if (ast->symbol == TK_NOT)
	{
		return checkTypeBooleanExpression(st,ast->child[0],scope);
	}
	else if (ast->symbol == TK_LE || ast->symbol == TK_LT || ast->symbol == TK_EQ ||ast->symbol == TK_GT ||
		ast->symbol == TK_GE || ast->symbol == TK_NE)
	{
		//printf("check if it enters here\n");

		type3 = checkTypeVar(st,ast->child[0],scope);
		type4 = checkTypeVar(st,ast->child[1],scope);

		//printf("type3 is : %d and type4 is %d\n",type3,type4);
		if (type3 == type4 && type3 != -1)
		{
			//implies both sides has same type
			return 4;// 4 implies boolean return 
		}
		else
			return -1;

	}
	return -1;

}

int checkTypeVar(symbol_table_ptr st,abstractSyntaxTree ast,int scope)
{
	int type=-1;
	if (ast->symbol == TK_NUM)
		return 0;
	else if (ast->symbol == TK_RNUM)
		return 1;
	else if (ast->symbol == TK_ID)
	{
		//printf("entered here inthis section\n");
		int temporary = type_function_var(st,ast,scope);
		//printf("temporary is : %d\n",temporary);
		if (temporary == -1)
		{
			/*printf("Error: Variable <%s> at line no <%d> might not have been declared\n",ast->value,
							ast->line_no);*/

			printf("line %d : variable %s is not declared\n",ast->line_no,ast->value);




		}
		else
			return temporary;
	}
	return -1;
}

char* getRecN(abstractSyntaxTree ast,symbol_table_ptr st,int scope)
{
	while (ast->symbol != TK_ID)
	{
		ast = ast->child[0];
	}

	funcptr funcvar = st->array[scope];

	identifier_list temp;

	temp = funcvar -> input_par;
	while (temp != NULL)
	{
		if (strcmp(temp->id_name,ast->value) == 0)
			return temp->record_name;
		temp = temp->next;
	}

	temp = funcvar -> output_par;
	while (temp != NULL)
	{
		if (strcmp(temp->id_name,ast->value) == 0)
			return temp->record_name;
		temp = temp->next;
	}


	temp = funcvar -> head;
	while (temp != NULL)
	{
		if (strcmp(temp->id_name,ast->value) == 0)
			return temp->record_name;
		temp = temp->next;
	}


	temp = st->head;
	while (temp != NULL)
	{
		if (strcmp(temp->id_name,ast->value) == 0)
			return temp->record_name;
		temp = temp->next;
	}

	printf("Line %d : The variable %s is not declared\n",ast->line_no,ast->value);
	return " ";

}



int getAllRecType(abstractSyntaxTree ast,symbol_table_ptr st,int scope)
{
	

	char recName[200];
	int type,dummy;
	strcpy(recName,getRecN(ast,st,scope));
	//printf("recname is : %s\n",recName);

	recptr t6 = st->rp;

	while (t6 != NULL)
	{
		if (strcmp(t6->record_name,recName) == 0)
		{
			dummy = t6 -> off;
			/*printf("in record\n");
			printf("off is : %d\n",t6->off);*/

			type = ((dummy%10)/2-1);
			//printf("type is : %d\n",type);
			
			while (dummy >0)
			{
				if (((dummy%10)/2-1) != type)
					return -1;
				dummy = dummy/10;
			}
			//printf("type returned is : %d\n",type);
			return type;
		}
		t6 = t6 -> next;
	}
	return -1; // implies error

}




