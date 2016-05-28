// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P


#include "semanticAnalyzer.h"
#include "typechecker.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



int error_semantics(symbol_table_ptr st,abstractSyntaxTree ast)
{
	//points covered 1,2,3,4,5,6,7,8,9,10,11,12,13,14,
	//remaining 15
	// 8 ka kya matlab
	int semantic_check_errors = 0;
	semantic_check_errors += error_declaration_multiple(st);
	//printf("1\n");
	semantic_check_errors += error_function_name_multiple(st);//check for overloading
	//printf("2\n");
	semantic_check_errors += error_if_global_no_other_declaration(st);
	//printf("3\n");
	semantic_check_errors += error_check_for_par_func(st,ast);
	//printf("4\n");



	return semantic_check_errors;

}

int error_declaration_multiple(symbol_table_ptr st)
{
	int semantic_check_errors = 0;




	identifier_list temp = st->head,k;
	
	while (temp != NULL)
	{
		if (temp->next != NULL)
		{
			k=temp->next;
			while (k != NULL)
			{
				if (strcmp(temp->id_name,k->id_name) == 0 && temp->id_type == temp->id_type)
				{
					printf("The variable <%s> is redeclared at line number <%d>\n",k->id_name,k->line_no);
					semantic_check_errors++;
					k=NULL;
				}
				if (k!= NULL)
					k = k->next;
			}

		}		
		temp = temp->next;
	}

	//now check if no duplication of fieldid in record is allowed and also no two declarations of 
	// same record name is present

	//first checking for record name
	char tempstr[200];
	recptr t6 = st->rp,t7;
	while (t6!=NULL)
	{
		//printf("record name: %s\n",t6->record_name);
		//strcpt(tempstr,t6->record_name);
		if (t6->next != NULL)
		{
			t7 = t6 -> next;
			while (t7 != NULL)
			{
				if (strcmp(t7->record_name,t6->record_name) == 0)
				{
					printf("record with name <%s> is redeclared at line <%d>\n",t7->record_name,t7->head->line_no);
					t7 = NULL;
				}
				if (t7 != NULL)
					t7 = t7->next;
			}
		}
		t6 = t6->next;
	}

	//now check for duplication declarations of each record field

	t6 = st->rp;
	while (t6!=NULL)
	{
		//printf("record name: %s\n",t6->record_name);
		//strcpt(tempstr,t6->record_name);
		temp = t6-> head;
		if (temp->next != NULL)
		{
			k=temp->next;
			while (k != NULL)
			{
				if (strcmp(temp->id_name,k->id_name) == 0 && temp->id_type == temp->id_type)
				{
					printf("The record field <%s> is redeclared at line number <%d>\n",k->id_name,k->line_no);
					semantic_check_errors++;
					k=NULL;
				}
				if (k!= NULL)
					k = k->next;
			}
		}
		
		t6 = t6->next;
	}

	//check for duplication fields of each record checked



		



	int j=0;
	for (j=0;j<(st->num_of_function);j++)
	{
		funcptr t1=st->array[j];

		temp = t1-> head;

		while (temp != NULL)
		{
		if (temp->next != NULL)
		{
			k=temp->next;
			while (k != NULL)
			{
				if (strcmp(temp->id_name,k->id_name) == 0 && temp->id_type == temp->id_type)
				{
					printf("The variable <%s> is redeclared at line number <%d>\n",k->id_name,k->line_no);
					semantic_check_errors++;
					k=NULL;
				}
				if (k!= NULL)
					k = k->next;
			}
		}
		temp = temp -> next;

		}//closing while		

	}
	return semantic_check_errors;
}

int error_function_name_multiple(symbol_table_ptr st)
{
	int semantic_check_errors = 0;
	int i,j;
	funcptr funcvar;

	for (i=0;i< (st->num_of_function-1);i++)
	{
		funcvar = st->array[i];

		for (j=i+1;j<st->num_of_function;j++)
		{
			if (strcmp(st->array[j]->func_name,funcvar->func_name) == 0)
			{
				printf("Error: The function <%s> at line <%d> has multiple declarations\n",
					funcvar->func_name,st->array[j]->input_par->line_no);
				j=st->num_of_function;
				semantic_check_errors++;

			}
		}


	}
	return semantic_check_errors;
}

int error_if_global_no_other_declaration(symbol_table_ptr st)
{

	/////////////////////
	char** type;

	type=(char**)malloc(4*sizeof(char*));
	type[0]=malloc(10*sizeof(char));
	type[1]=malloc(10*sizeof(char));
	type[2]=malloc(10*sizeof(char));
	type[3]=malloc(10*sizeof(char));
	strcpy(type[0],"int");
	strcpy(type[1],"real");
	strcpy(type[2],"record");
	strcpy(type[3],"undefined");



	//////////////////
	//printf("here\n");

	int semantic_check_errors = 0,i,j;
	//check if global variables not record_id are defined in any other function
	identifier_list temp = st->head,k;
	while (temp != NULL)
	{
		for (i=0;i<st->num_of_function;i++)
		{
			//for each function chk if output par and return list have same type ele report as error
			identifier_list s1 = st->array[i]->output_par;
			identifier_list s2 = st->array[i]->return_par;
			
			if (s1 == NULL && s2 == NULL)
			{
				//printf("ho\n");
			}

			while (s1 != NULL && s2 != NULL)
			{
				//printf(" s1 ka id type is : %d\n",s1->id_type);
				//printf(" s2 ka id type is : %d\n",s2->id_type);
				int d1 = s1->id_type;
				int d2 = s2->id_type;
				if ((d1 == d2) && d1 != 2)
				{


				}
				else if (d1 == d2 && (d1 == 2) && strcmp(getRecordName(s1->id_name,st)!=NULL?(getRecordName(s1->id_name,st)):("random"),getRecordName(s2->id_name,st)!= NULL ?(getRecordName(s2->id_name,st)):("random"))==0)
				{

				}
				else 
				{
					//printf("finally \n");
					//report error

					printf("line %d : The type <%s> of variable <%s> returned does not match with the type <%s> of the formal output parameter <%s>\n",
						s2->line_no,type[d2>=0?d2:3],s2->id_name,type[d1>=0?d1:3],s1->id_name);
				}
				s1 = s1->next;
				s2 = s2->next;

			}
			//printf("1\n");




			//////////////////new module ended
			funcptr funcvar = st->array[i];
			k=funcvar -> input_par;
			while (k!=NULL)
			{
				if (strcmp(temp->id_name,k->id_name) == 0 && temp->id_type == temp->id_type)
				{
					printf("Error: The variable <%s> is global and can't be redeclared at line <%d>\n",
						temp->id_name,k->line_no);
					semantic_check_errors++;
					k=NULL;
				}
				if (k!=NULL)
				k=k->next;
			}
			k=funcvar -> output_par;
			while (k!=NULL)
			{
				if (strcmp(temp->id_name,k->id_name) == 0 && temp->id_type == temp->id_type)
				{
					printf("Error: The variable <%s> is global and can't be redeclared at line <%d>\n",
						temp->id_name,k->line_no);
					semantic_check_errors++;
					k=NULL;
				}
				if (k!=NULL)
				k=k->next;
			}
			k=funcvar -> head;
			while (k!=NULL)
			{
				if (strcmp(temp->id_name,k->id_name) == 0 && temp->id_type == temp->id_type)
				{
					printf("Error: The variable <%s> is global and can't be redeclared at line <%d>\n",
						temp->id_name,k->line_no);
					semantic_check_errors++;
					k=NULL;
				}
				if (k!=NULL)
				k=k->next;
			}
			//printf("22\n");
		}
		//printf("halo1\n");
		temp = temp -> next;
		//printf("halo2\n");
	}

	//now check for global record ids
	//note all global record ids will be checked by above procedure


	return semantic_check_errors;
}

void printId(identifier_list temp)
{
	while (temp != NULL)
	{
		printf("%s ",temp->id_name);
		temp = temp -> next;
	}

	printf("\n");
}
int error_check_for_par_func(symbol_table_ptr st,abstractSyntaxTree ast)
{
	//////////////////////////////////////////////////////////////////////////////////////////////

	char** type;

	type=(char**)malloc(3*sizeof(char*));
	type[0]=malloc(10*sizeof(char));
	type[1]=malloc(10*sizeof(char));
	type[2]=malloc(10*sizeof(char));
	strcpy(type[0],"int");
	strcpy(type[1],"real");
	strcpy(type[2],"record");

	////////////////////////////////////////////////////////////////////////////////////////////////
	int semantic_check_errors=0,scope,scope1,typefunc,check_flag,chk=0;;

	abstractSyntaxTree temp,t1,t2,t3;
	identifier_list return_list;
	temp = ast->child[0];
	scope1 = 0;
	while (temp->symbol != TK_EPSILON)
	{
		//printf("entered function block\n");
		t1 = temp->child[0]->child[2];//<stmts
		t1 = t1-> child[2];//<otherStmts>
		return_list = NULL;

		///////////////////////////////////////////
		//for checking if return values of a function has been assigned a value or not

		return_list = getReturnIdList(st,ast,scope1);//scope helps to reach the function
		//printf("check 1\n");
		while (return_list != NULL)
				{

					chk = check_return(return_list,t1,scope1);
					//chk = 0;
					if (return_list -> id_type == -1);
					else if (chk == 0)
					{
						printf("Line %d: The variable <%s> is not assigned a value in function <%s>\n",return_list->line_no,return_list->id_name,st->array[return_list->id_scope]->func_name);//error message ki return variable must be initialized
						semantic_check_errors++;
					}
					return_list = return_list->next;

				}
			//printf("check2\n");




		///////////////////////////////////////




		while (t1->symbol != TK_EPSILON)
		{
			
			
			t2 = t1->child[0];//<stmt>
			//printf("statement ka name : %s\n",tokenName[t2->symbol]);
			/////////////
			//look for stmt in while loop as well
			if (t2 -> symbol == TK_WHILE)
			{
				/////////////////////////////////////////////////////
				//chk if value assigned in while block
				identifier_list temp6 = getWhileList(st,t2,scope1);

				while (temp6 != NULL)
				{
					chk = chk_while(temp6,t2,scope1);
					if (chk == 0)
					{
						temp6 = temp6 -> next;
					}
					else
					{
						break;
					}
				}
				if (chk == 0)
					printf("line %d: None of the variables participating in the iterations of the while loop gets updated\n",t2->line_no);

				/*printf("prining while list\n");
				printId(temp6);

				printf("already printed while list\n");*/


				//////////////////////////////////////////////////////
				//printf("hello\n");
				abstractSyntaxTree t6,t7;
				if (t2->child[1]->symbol == TK_FUNID)
				{
					semantic_check_errors += check_funidStmt(t2->child[1],ast,st,scope1);
				}

				t6 = t2->child[2];//<otherStmts> of <while loop
				while (t6 -> symbol != TK_EPSILON)
				{
					t7 = t6->child[0];
					if (t7->symbol == TK_FUNID)
					{
						semantic_check_errors += check_funidStmt(t7,ast,st,scope1);

					} 
					t6 = t6->child[1];
				}//closing while

				 

			}


			/////////////
			else if (t2->symbol == TK_IF)
			{

				abstractSyntaxTree t6,t7;
				if (t2->child[1]->symbol == TK_FUNID)
				{
					semantic_check_errors += check_funidStmt(t2->child[1],ast,st,scope1);
				}

				t6 = t2->child[2];//<otherStmts> of <while loop
				while (t6 -> symbol != TK_EPSILON)
				{
					t7 = t6->child[0];
					if (t7->symbol == TK_FUNID)
					{
						semantic_check_errors += check_funidStmt(t7,ast,st,scope1);

					} 
					t6 = t6->child[1];
				}//closing while



				t6 = t2 -> child[2];

				if (t6-> symbol == TK_ELSE)
				{

				abstractSyntaxTree t66,t77;
				if (t6->child[0]->symbol == TK_FUNID)
				{
					semantic_check_errors += check_funidStmt(t6->child[0],ast,st,scope1);
				}

				t66 = t6->child[1];//<otherStmts> of <while loop
				while (t66 -> symbol != TK_EPSILON)
				{
					t77 = t66->child[0];
					if (t77->symbol == TK_FUNID)
					{
						semantic_check_errors += check_funidStmt(t77,ast,st,scope1);

					} 
					t66 = t66->child[1];
				}//closing while

				}


			}

			else if (t2 -> symbol == TK_FUNID)
			{
				//first check if output paramater of function call is null
				//then output par in function definition must be null
				semantic_check_errors += check_funidStmt(t2,ast,st,scope1);
				

			}


			t1 = t1->child[1];
				
			

		}


	scope1++;
	temp = temp->child[1];
		
	}

	//same thing done for functions is also done for main function
	//printf("entered main block\n");
	//scope1++;
	t1 = ast->child[1]->child[0];//<stmts
	t1 = t1-> child[2];//<otherStmts>
	//printf("scope1 is : %d\n",scope1);
	return_list = NULL;


	

		while (t1->symbol != TK_EPSILON)
		{
			t2 = t1->child[0];//<stmt>
			//printf("In main : stmt ka namm %s\n",tokenName[t2->symbol]);


			if (t2 -> symbol == TK_WHILE)
			{
				//printf("main hello\n");
				abstractSyntaxTree t6,t7;
				if (t2->child[1]->symbol == TK_FUNID)
				{
					semantic_check_errors += check_funidStmt(t2->child[1],ast,st,scope1);
				}

				t6 = t2->child[2];//<otherStmts> of <while loop
				while (t6 -> symbol != TK_EPSILON)
				{
					t7 = t6->child[0];
					if (t7->symbol == TK_FUNID)
					{
						semantic_check_errors += check_funidStmt(t7,ast,st,scope1);

					} 
					t6 = t6->child[1];
				}//closing while

				 

			}

			else if (t2->symbol == TK_IF)
			{

				abstractSyntaxTree t6,t7;
				if (t2->child[1]->symbol == TK_FUNID)
				{
					semantic_check_errors += check_funidStmt(t2->child[1],ast,st,scope1);
				}

				t6 = t2->child[2];//<otherStmts> of <while loop
				while (t6 -> symbol != TK_EPSILON)
				{
					t7 = t6->child[0];
					if (t7->symbol == TK_FUNID)
					{
						semantic_check_errors += check_funidStmt(t7,ast,st,scope1);

					} 
					t6 = t6->child[1];
				}//closing while



				t6 = t2 -> child[2];

				if (t6-> symbol == TK_ELSE)
				{

				abstractSyntaxTree t66,t77;
				if (t6->child[0]->symbol == TK_FUNID)
				{
					semantic_check_errors += check_funidStmt(t6->child[0],ast,st,scope1);
				}

				t66 = t6->child[1];//<otherStmts> of <while loop
				while (t66 -> symbol != TK_EPSILON)
				{
					t77 = t66->child[0];
					if (t77->symbol == TK_FUNID)
					{
						semantic_check_errors += check_funidStmt(t77,ast,st,scope1);

					} 
					t66 = t66->child[1];
				}//closing while

				}


			}


			else if (t2 -> symbol == TK_FUNID)
			{

				semantic_check_errors += check_funidStmt(t2,ast,st,scope1);				

			}


			t1 = t1->child[1];

		}//closing while

	return semantic_check_errors;
}



int getScope(symbol_table_ptr st,abstractSyntaxTree ast)
{
	int scope = -1;
	int t=0;
	int i;
	for (i=0;i<st->num_of_function-1;i++)
	{
		if (strcmp(st->array[i]->func_name,ast->value) == 0)
		{
			return i;
		}
	}
	return scope;


}



identifier_list getReturnIdList(symbol_table_ptr st,abstractSyntaxTree ast,int scope)
{
	//printf("I n function to find return type : %s\n",tokenName[ast->symbol]);

	identifier_list temp=NULL;
	ast = ast->child[0];
	int t=0;
	while (ast->symbol != TK_EPSILON && t < scope)
	{
		ast = ast->child[1];
		t++;
	}
	ast = ast->child[0];//<function>
	ast = ast->child[2]->child[3];//<returnStmt>
	if (ast->symbol == TK_EPSILON)
		return temp;

	while (ast->symbol != TK_EPSILON)
	{
		int type1 = type_function_var(st,ast,scope);
			if (type1 == -1)
				{
					/*printf("Error: Variable <%s> at line no <%d> might not have been declared\n",ast->value,
					ast->line_no);*/

					printf("line %d : variable %s is not declared\n",ast->line_no,ast->value);
					//semantic_check_errors++;
				}
		identifier_list tmp = (identifier_list)malloc(sizeof(struct identifier));
			tmp -> id_type = type1;
			tmp -> id_scope = scope;
			tmp -> line_no = ast->line_no;
			strcpy(tmp->record_name," "); // return type can never be record id
			strcpy(tmp->id_name,ast->value);
			tmp->next = NULL;
		if (temp == NULL)
		{			temp = tmp;		



		}
		else
		{
			identifier_list t9 = temp;
			while (t9 -> next != NULL)
			{
				t9 = t9-> next;
			}
			t9 -> next = tmp;

		}
		ast = ast->child[0];
	}
	return temp;

}

// 0 means not assigned a value or read from anywhere

int check_return(identifier_list return_list,abstractSyntaxTree ast,int scope)
{
	int chk=0;
	abstractSyntaxTree t1,t10;
	while (ast->symbol != TK_EPSILON)
	{
		t1 = ast->child[0];
		if (t1 -> symbol == TK_ASSIGNOP && (strcmp(t1->child[0]->value,return_list->id_name) == 0))
		{
			chk = 1;
			return chk;
		}
		else if (t1->symbol == TK_READ && strcmp(t1->child[0]->value,return_list->id_name) == 0)
		{
			chk =1;
			return chk;
		}
		else if (t1->symbol == TK_FUNID)
		{
			t10 = t1->child[0];
			while (t10->symbol != TK_EPSILON)
			{
				if (strcmp(t10->value,return_list->id_name) == 0)
				{
					chk = 1;
					return chk;
				}
				t10 = t10->child[0];
			}

		}

		ast = ast->child[1];

	}

	return chk;
}

int chk_while(identifier_list temp,abstractSyntaxTree ast,int scope)
{
	int chk = 0;
	chk = check_return(temp,ast->child[2],scope);
	if (chk == 1)
		return chk;
	abstractSyntaxTree t1 = ast->child[1];
	abstractSyntaxTree t10;
	if (t1 -> symbol == TK_ASSIGNOP && (strcmp(t1->child[0]->value,temp->id_name) == 0))
		{
			chk = 1;
			return chk;
		}
		else if (t1->symbol == TK_READ && strcmp(t1->child[0]->value,temp->id_name) == 0)
		{
			chk =1;
			return chk;
		}
		else if (t1->symbol == TK_FUNID)
		{
			t10 = t1->child[0];
			while (t10->symbol != TK_EPSILON)
			{
				if (strcmp(t10->value,temp->id_name) == 0)
				{
					chk = 1;
					return chk;
				}
				t10 = t10->child[0];
			}

		}


	chk = 0;
	return chk;

}



identifier_list getBool(symbol_table_ptr st,abstractSyntaxTree ast,int scope)
{
	identifier_list temp;
	if (ast->symbol == TK_NOT)
	{
		temp = getBool(st,ast->child[0],scope);
	}
	else if (ast->symbol == TK_AND || ast->symbol == TK_OR)
	{
		temp = getBool(st,ast->child[0],scope);
		identifier_list y6=temp;

		while (y6->next != NULL)
		{
			y6 = y6->next;
		}
		y6 -> next = getBool(st,ast->child[1],scope);

	}
	else if(ast->symbol == TK_LE || ast->symbol == TK_LT || ast->symbol == TK_EQ ||ast->symbol == TK_GT ||ast->symbol == TK_GE ||ast->symbol == TK_NE)
	{
		identifier_list qa=(identifier_list)malloc(sizeof(struct identifier));

		qa->id_type = -1;
		qa -> id_scope = scope;
		qa ->line_no = ast->line_no;
		strcpy(qa->record_name," ");
		strcpy(qa->id_name,ast->child[0]->value);
		qa -> offset = -1;
		qa->next = NULL;

		
		identifier_list qb=(identifier_list)malloc(sizeof(struct identifier));
		qb->id_type = -1;
		qb -> id_scope = scope;
		qb ->line_no = ast->line_no;
		strcpy(qb->record_name," ");
		strcpy(qb->id_name,ast->child[1]->value);
		qb -> offset = -1;
		qb->next = NULL;
		qa->next = qb;
		temp = qa;
		//return temp;
	}

	return temp;
}
identifier_list getWhileList(symbol_table_ptr st,abstractSyntaxTree ast,int scope)
{
	identifier_list temp;
	temp = getBool(st,ast->child[0],scope);

	return temp;
}


char* getRecordName(char arr[200],symbol_table_ptr st)
{
	//i have changed this
	/*recptr t6 = st->rp;

	while (t6 != NULL)
	{
		if(strcmp(t6->record_name,arr) == 0)
		{
			//return t6->off;
			return t6->record_name;
		}
	}*/

	int i;
	identifier_list temp;
	for (i=0;i<st->num_of_function;i++)
	{
		temp = st->array[i]->input_par;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,arr) == 0)
				return temp->record_name;

			temp = temp ->next;
		}

		temp = st->array[i]->output_par;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,arr) == 0)
				return temp->record_name;

			temp = temp -> next;
		}

		temp = st->array[i]->head;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,arr) == 0)
				return temp->record_name;

			temp = temp -> next;
		}

	}

	temp = st->head;
		while (temp != NULL)
		{
			if (strcmp(temp->id_name,arr) == 0)
				return temp->record_name;
		}

		char arr5[200];
		strcpy(arr5," ");
		return NULL;
}


int getTypeMainOut(abstractSyntaxTree t3,symbol_table_ptr st)
{
	int type=-1;
	identifier_list temp = st->array[st->num_of_function-1]->head;

	while (temp != NULL)
	{
		if (strcmp(temp->id_name,t3->value) == 0)
		{
			return temp->id_type;
		}
		temp = temp->next;
	}

	temp = st->head;

	while (temp != NULL)
	{
		if (strcmp(temp->id_name,t3->value) == 0)
		{
			return temp->id_type;
		}
		temp = temp->next;
	}

	return -1;

}


int check_funidStmt(abstractSyntaxTree t2,abstractSyntaxTree ast,symbol_table_ptr st,int scope1)
{
	int semantic_check_errors=0,typefunc;
	identifier_list return_list;
	int scope;
	abstractSyntaxTree t3;
	identifier_list idlist;
				scope = getScope(st,t2);
				//printf("scope")
				if (scope > scope1 && scope != -1 && scope1 != -1)
				{
					//printf("Error: function <%s> must be defined before <%s>\n",st->array[scope]->func_name,st->array[scope1]->func_name);
					printf("Line %d : The function %s is undefined.\n",t2->line_no,st->array[scope]->func_name);
					semantic_check_errors++;
				}


				if (scope != -1)
				{
					//printf("function called for func scope :%d\n",scope);
				return_list = getReturnIdList(st,ast,scope);//scope helps to reach the function
				

				///////////////////////////////////////////////////////



				if (t2->child[0]->symbol == TK_EPSILON)
				{
					if (st->array[scope]->output_par != NULL)
					{
						semantic_check_errors++;
						//printf("Line %d: No of output parameters of function <%s> does not match with function call \n",
						//	t2->line_no,st->array[scope]->func_name);
						printf("Line %d : The number of output parameters at function call %s is incorrect\n",t2->line_no,st->array[scope]->func_name);

					}
					else if (return_list != NULL)
					{
						semantic_check_errors++;
						//printf("Error: No of return type of function <%s> does not match with function call at line <%d>\n",
						//	st->array[scope]->func_name,t2->line_no);
						printf("Line %d : The number of return type variables of function at function call %s is incorrect\n",t2->line_no,st->array[scope]->func_name);

					}

				}
				else
				{
					// check for conditions when no of output parameters in defi or call is less
					idlist = st->array[scope]->output_par;
					//check for types of output parameters : func definition and func call
					t3 = t2->child[0];//<outputParameters>
					//it is else block so i know t3 is not null
					//t3 is now <idlist> because it is pulled up in ast
					int check_flag = 0;
					while (t3->symbol != TK_EPSILON)
					{
						if (idlist == NULL)
						{
							//printf("Error: Number of output par for function <%s> at line <%d> do not match with defintion\n",
							//	st->array[scope]->func_name,t2->line_no);
							printf("Line %d : The number of output parameters at function call %s is incorrect\n",t2->line_no,st->array[scope]->func_name);

							semantic_check_errors++;
							check_flag = 1;
							break;
						}
						typefunc = type_function_var(st,t3,scope1);
						if (typefunc == -1)
						{
							/*printf("Error: Variable <%s> at line no <%d> might not have been declared\n",t3->value,
							t3->line_no);*/

							printf("line %d : variable %s is not declared\n",t3->line_no,t3->value);
							semantic_check_errors++;

						}
						else if (idlist->id_type == -1);
						else if (typefunc == idlist->id_type && typefunc != 2)// no prob
						{
							idlist = idlist->next;
						}
						else if (typefunc == idlist->id_type && typefunc == 2 && strcmp(getRecordName(t3->value,st)!= NULL?(getRecordName(t3->value,st)):("random"),getRecordName(idlist->id_name,st)!=NULL?(getRecordName(idlist->id_name,st)):("random1"))==0)
						{
							idlist = idlist -> next;
						}
						else
						{
							/*printf("Line %d: type of output par for function <%s> at line <%d> do not match with defintion\n",t2->line_no,
								st->array[scope]->func_name);*/

							printf("line %d : The type of output paramater <%s> does not match with the type of the formal output parameter <%s>\n",t2->line_no,
								t3->value,idlist->id_name);
							semantic_check_errors++;
							//check_flag = 1;
							break;
						}
						t3 = t3->child[0];
					}
					if (check_flag == 0)
					{
						if (idlist != NULL)
						{
							/*printf("Error: Number of output par for function <%s> at line <%d> do not match with defintion\n",
								st->array[scope]->func_name,t2->line_no);*/
							printf("Line %d : The number of output parameters at function call <%s> is incorrect.\n",
								t2->line_no,st->array[scope]->func_name);
							semantic_check_errors++;

						}
					}

					//now check for return value type and calling function type

					idlist = return_list;
					//check for types of output parameters : func definition and func call
					t3 = t2->child[0];//<outputParameters>
					//it is else block so i know t3 is not null
					//t3 is now <idlist> because it is pulled up in ast
					check_flag = 0;
					while (t3->symbol != TK_EPSILON)
					{
						if (idlist == NULL)
						{
							//printf("Error: Number of output par for function <%s> at line <%d> do not match with function return variables\n", st->array[scope]->func_name,t2->line_no);
							printf("Line %d : The number of output parameters at function call <%s> do not match with function return type definition.\n",t2->line_no,
								st->array[scope]->func_name);
							semantic_check_errors++;
							check_flag = 1;
							break;
						}
						typefunc = type_function_var(st,t3,scope1);
						if (typefunc == -1)
						{
							/*printf("Error: Variable <%s> at line no <%d> might not have been declared\n",t3->value,
							t3->line_no);*/
							printf("line %d : variable %s is not declared\n",t3->line_no,t3->value);
							semantic_check_errors++;

						}
						else if (idlist->id_type == -1);
						else if (typefunc == idlist->id_type && typefunc!=2)// no prob
						{
							idlist = idlist->next;
						}
						else if (typefunc == idlist->id_type && typefunc == 2 && strcmp(getRecordName(t3->value,st)!=NULL?(getRecordName(t3->value,st)):("random"),getRecordName(idlist->id_name,st)!= NULL?(getRecordName(idlist->id_name,st)):("random1"))==0)
						{
							idlist = idlist -> next;
						}
						else
						{
							/*printf("Error: type of output par for function <%s> at line <%d> do not match with function return type defintion\n",st->array[scope]->func_name,t2->line_no);
							semantic_check_errors++;*/

							printf("line %d : The type of output paramater <%s> does not match with the type of return type variable <%s>\n",t2->line_no,
								t3->value,idlist->id_name);
							//check_flag = 1;
							break;
						}
						t3 = t3->child[0];
					}
					if (check_flag == 0)
					{
						if (idlist != NULL)
						{
							/*printf("Error: Number of output par for function <%s> at line <%d> do not match with return type defintion\n",st->array[scope]->func_name,t2->line_no);
							semantic_check_errors++;*/

							printf("Line %d : The number of output parameters at function call <%s> do not match with no of return variables.\n",
								t2->line_no,st->array[scope]->func_name);

						}
					}




				}
				//checking for type of input par
				if (t2->child[1]->symbol != TK_EPSILON)
				{
					
					// check for conditions when no of output parameters in defi or call is less
					//printf("hey\n");
					idlist = st->array[scope]->input_par;
					//check for types of output parameters : func definition and func call
					t3 = t2->child[1];//<inputParameters>
					//it is else block so i know t3 is not null
					//t3 is now <idlist> because it is pulled up in ast
					int check_flag = 0;
					while (t3->symbol != TK_EPSILON)
					{
						if (idlist == NULL)
						{
							/*printf("Error: Number of input par for function <%s> at line <%d> do not match with defintion\n",
								st->array[scope]->func_name,t2->line_no);*/

							printf("Line %d : The number of input parameters at function call %s is incorrect\n",t2->line_no,st->array[scope]->func_name);
							semantic_check_errors++;
							check_flag = 1;
							break;
						}

						typefunc = type_function_var(st,t3,scope1);
						if (typefunc == -1)
						{
							printf("line %d : variable %s is not declared\n",t3->line_no,t3->value);
							semantic_check_errors++;

						}
						else if (idlist->id_type == -1);
						else if (typefunc == idlist->id_type && typefunc != 2)// no prob
						{
							idlist = idlist->next;
						}
						else if (typefunc == idlist->id_type && typefunc == 2 && strcmp(getRecordName(t3->value,st)!= NULL? (getRecordName(t3->value,st)):("random"),getRecordName(idlist->id_name,st)!= NULL? (getRecordName(idlist->id_name,st)):("random1"))==0)
						{
							idlist = idlist -> next;
						}
						else
						{
							/*printf("Error: type of input par for function <%s> at line <%d> do not match with defintion\n",
								st->array[scope]->func_name,t2->line_no);*/
							printf("line %d : The type of input parameter <%s> does not match with the type of the formal input parameter <%s>.\n",
								t2->line_no,t3->value,idlist->id_name);

							semantic_check_errors++;
							//check_flag = 1;
							break;
						}
						t3 = t3->child[0];
					}
					if (check_flag == 0)
					{
						if (idlist != NULL)
						{
							/*printf("Error: Number of input par for function <%s> at line <%d> do not match with defintion\n",
								st->array[scope]->func_name,t2->line_no);*/

							printf("Line %d : The number of input parameters at function call <%s> is incorrect.\n",
								t2->line_no,st->array[scope]->func_name);
							semantic_check_errors++;

						}
					}


				}
				}//closing if for scope condition
				else
				{
					printf("Line %d: The function call <%s> has no definition\n",
						t2->line_no,t2->value);
					semantic_check_errors++;
				}

				return semantic_check_errors;
}
