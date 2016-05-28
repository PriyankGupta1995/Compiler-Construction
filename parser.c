// Batch Number 42
// Saurabh Anand  2013A7PS139P
// Priyank Gupta  2013A7PS060P



#include "lexer.h"
#include "parser.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void createParseTable(grammar G)
{
	int i,j;
	no_of_nonterminals = 51;
	no_of_tokens = 54;
	for (i=0;i<no_of_nonterminals;i++)
	{
		for (j=0;j<no_of_tokens;j++)
		{
			parse_table[i][j]=-1;
		}
	}

	//printf("Bro %d\n",parse_table[2][3]);

	for (i=0;i<no_of_nonterminals;i++)
	{
		rule_ptr t;
		t=G[i].grammar_node_head;
		int index = 0;

		while ( t!= NULL)
		{
			list l;
			l=t->first;


			while (l != NULL)
			{
				parse_table[i][l->num] = (i+60)*10+index;

				l=l->next_node;
			}


			index++;

			t=t->next_rule;

		}
	}


}

grammar initGrammar(FILE* fp)
{
	grammar G;
	int i,j;
	int non_terminal_no , rule_len;
	fscanf(fp,"%d %d\n",&no_of_nonterminals, &no_of_rules_of_grammar);

	G=(struct grammar_node*)malloc(no_of_nonterminals*sizeof(struct grammar_node));

	for (i=0;i<no_of_nonterminals;i++)
	{
		G[i].num_of_rules = 0;
		G[i].grammar_node_head = NULL;
		G[i].first_len = 0;
		G[i].follow_len = 0;
	}

	for (i=0;i<no_of_rules_of_grammar;i++)
	{
		char tempstr[30];
		fscanf(fp,"%s %d",tempstr,&rule_len);
		//fscanf(fp,"%d %d",&non_terminal_no,&rule_len);
		non_terminal_no = getId(tempstr);
		if (non_terminal_no < 0)
		{
			printf("wrong terminal or non terminal entered\n");
			continue;
		}
		//rule index is not used anywhere hence removed
		int* temp_arr = (int*) malloc(rule_len*sizeof(int));

		for (j=0;j<rule_len;j++)
		{
			char tempstr1[30];
			int t;
			//fscanf(fp," %d",&t);
			fscanf(fp," %s",tempstr1);
			t=getId(tempstr1);
			temp_arr[j] = t;
		}


		G[non_terminal_no-60].num_of_rules++;

		rule_ptr temp_rule, trule;

		temp_rule = (rule_ptr)malloc(sizeof(struct rule));
		temp_rule -> len_rule = rule_len;
		temp_rule -> rule_head = temp_arr;
		temp_rule -> next_rule = NULL;
		temp_rule -> eps = 0;
		
		//modified 9th april
		temp_rule -> rule_number = non_terminal_no*10+(G[non_terminal_no-60].num_of_rules)-1;
		//modified 9th april

		if (rule_len == 0)
			temp_rule -> eps = 1;

		trule = G[non_terminal_no-60].grammar_node_head;

		if(G[non_terminal_no-60].grammar_node_head ==NULL) //if the first rule is being added
		{
			G[non_terminal_no-60].grammar_node_head = temp_rule;
		}
		else //all rules traversed and rules added at the last
		{
			while((trule->next_rule)!=NULL)
			{
				trule = trule->next_rule;
			}
			trule->next_rule = temp_rule;

		}
	}
	return G;
}


//token id to token name already initialized in lexer

grammar initFirstFollow(FILE* fp1,FILE* fp2, grammar G)
{
	int i,nt,len,temp,j;
	no_of_nonterminals = 51;
	//printf("chk1\n");
	for (i=0; i < no_of_nonterminals; i++)
	{
		char str4[30];
		//fscanf(fp2,"%d %d",&nt, &len);
		//printf("cool\n");
		fscanf(fp2,"%s %d",str4, &len);
		//printf("cool2\n");
		nt = getId(str4);

		if (nt < 0)
			printf("error in initFirstFollow\n");

		int *arr;
		arr = (int*) malloc(len*sizeof(int));
		for (j=0;j<len;j++)
		{
			char str8[30];
			fscanf(fp2," %s",str8);
			arr[j]=getId(str8);
		}
		G[nt-60].follow = arr;
		G[nt-60].follow_len = len;

	}
	//printf("chk2\n");

	for (i=0; i < no_of_nonterminals; i++)
	{
		//fscanf(fp1,"%d %d",&nt, &len);
		char str5[30];
		fscanf(fp1,"%s %d",str5, &len);
		nt = getId(str5);
		int *arr;
		arr = (int*) malloc(len*sizeof(int));
		for (j=0;j<len;j++)
		{
			char str9[30];
			fscanf(fp1," %s",str9);
			arr[j]=getId(str9);
		}
		G[nt-60].first = arr;
		G[nt-60].first_len = len;
	}

	return G;

}


void print_parse_errors()
{
	int i=0;
	for (i=0;i<num_parse_errors;i++)
	{
		printf ("%s\n",parse_errors[i]);
	}
}

int get_parse_errors()
{
	return num_parse_errors;
}

grammar firstOfRule(grammar G)
{
	int i,k,j;
	for (i=0;i<no_of_nonterminals;i++)
	{
		rule_ptr trule = G[i].grammar_node_head;
		list temp_list;
		temp_list = NULL;



		while (trule != NULL)
		{
			int flag=0,length;
			length = trule -> len_rule;
			for (j=0;j<length && flag == 0; j++)
			{
				if (length ==1 && (trule -> rule_head[j] == 54))
				{
					j=1;
					break;
				}
				//I have not checked for repetitions
				if ((trule -> rule_head[j]) < 54)
				{
					//printf("non terminal is %d and rule head is %d at index : %d\n",(i+60),(trule -> rule_head[j]),j);
					struct node* temp_arr = (struct node*)malloc(1*sizeof(struct node));
					temp_arr -> num = trule -> rule_head[j];
					temp_arr -> next_node = NULL;

					if (temp_list == NULL)
					{
						temp_list = temp_arr;
					}
					else
					{
						temp_arr -> next_node = temp_list;
						temp_list = temp_arr;

					}
					flag =1;
				}
				else
				{

					//two possiblities now exist :
					//either the non terminal is nullable
					//or the non terminal is not nullable

					//check if that non terminal is not nullable
					if (G[(trule -> rule_head[j])-60].first[0] < 54)
					{
						int l2=G[(trule -> rule_head[j])-60].first_len;

						for (k=0; k<l2;k++)
						{
							struct node* temp_arr = (struct node*)malloc(1*sizeof(struct node));
							temp_arr -> num = G[(trule -> rule_head[j])-60].first[k];
							temp_arr -> next_node = NULL;

							if (temp_list == NULL)
							{
							temp_list = temp_arr;
							}
							else
							{

							temp_arr -> next_node = temp_list;
							temp_list = temp_arr;
							}

						}
						flag = 1;
					}//closing if
					else
					{
						int l2=G[(trule -> rule_head[j])-60].first_len;
						//k is 1 to avoid including eps in out first set of rule
						for (k=1; k<l2;k++)
						{
							struct node* temp_arr = (struct node*)malloc(1*sizeof(struct node));
							temp_arr -> num = G[(trule -> rule_head[j])-60].first[k];
							temp_arr -> next_node = NULL;

							if (temp_list == NULL)
							{
							temp_list = temp_arr;
							}
							else
							{

							temp_arr -> next_node = temp_list;
							temp_list = temp_arr;
							}

						}

					}//closing else



				}//closing else


			}
			//if (trule -> eps == 1)
			if( flag == 0 && j == length)
			{
				trule -> eps = 1;
			//append follow of nonterminal to rule ka first

			for (j=0;j<G[i].follow_len;j++)
				{
				struct node* temp_arr = (struct node*)malloc(1*sizeof(struct node));
				temp_arr -> num = G[i].follow[j];
				temp_arr -> next_node = NULL;
				if (temp_list == NULL)
				{
					temp_list = temp_arr;
				}
				else
				{
					temp_arr -> next_node = temp_list;
					temp_list = temp_arr;
				}
				}

			//trule = trule-> next_rule;
			}// if given rule is nullable add the follow of the non terminal


			trule->first = temp_list;
			temp_list = NULL;

			trule = trule -> next_rule;

		}

	}
	return G;
}

int getId(char* tempstr)
{
	int i;
	for (i=0;i<=110;i++)
	{
		if (strcmp(tokenName[i],tempstr) == 0)
		{
			return i;
		}
	}
	return -1;
}

stack initialise_stack()
{
	stack s;
	s= NULL;
	return s;

}
stack stack_push(stack s,int value)
{
	stack temp;
	temp= (stack) malloc(sizeof(struct _stack));
	temp->value=value;
	temp->next=s;
	//printf("Pushing %s\n",tokenName[temp->value]);
	return temp;
}

int stack_top(stack s)
{

	return (s->value);
}

stack stack_pop(stack s)
{
	stack temp;
	temp= s->next;
	//printf("Popping %s\n",tokenName[s->value]);
	free(s);
	return temp;
}

parseTree initialise_parsetree()
{
	parseTree pt;
	pt= NULL;
	return pt;

}

parseTree root(parseTree pt)
{
	while(pt->parent!=NULL)
	{
		pt = pt->parent;
	}
	return pt ;
}

parseTree create(parseTree pt,int nt ,int value,int currentRuleNo)
{
	int i=0;
	parseTree temp;
	temp= (parseTree ) malloc(sizeof(struct _ParseTree));
	//printf("Entering function create() \n");
	if(pt==NULL)
	{
		temp->parent = NULL;
		temp->symbol = value ;
		for(i=0;i<10;i++)
				temp->child[i] = NULL;
		temp->childnum = 0;
		temp->counterChild = -1;
		temp->ruleNo = -1;
		strcpy(temp->value, " ");
		temp->line_no = -1;

		//printf("Root node is %s \n",tokenName[temp->symbol]);
		//printf("Exiting function create() root \n");
		return temp;
	}
	else if(pt->symbol==nt)
	{
		temp->symbol = value ;
		for(i=0;i<10;i++)
				temp->child[i] = NULL;
		temp->childnum = 0;
		temp->counterChild = -1;
		temp->parent = pt;
		pt->child[pt->childnum] = temp;
		pt->ruleNo = currentRuleNo ;
		pt->childnum++;
		strcpy(temp->value, " ");
		temp->line_no = -1;
		//printf("Current node is is %s \n",tokenName[temp->symbol]);
		//printf("Exiting function create() non-root \n");
		//printf("Current Rule No = %d",currentRuleNo);
		return pt;
	}

	else
	{
		printf("Error due to invalid TOKEN\n");
	}
}


parseTree nextNode(parseTree pt)
{
	int i;
	i = pt->counterChild;

	if(pt->childnum == 0)
	{
		pt = nextNode(pt->parent);
		return pt;
	}

	if(i >= pt->childnum)
	{
		if(pt->parent == NULL)
		{
			//printf("Next node = %d  \n",pt->symbol);
			return pt;
		}
		else
		{
			pt = nextNode(pt->parent);
			return pt;
		}
	}
	else if(i == -1)
	{
		i++;
		pt->counterChild = i+1;
		//printf("Next node = %d  \n",pt->symbol);
		return pt->child[i];
	}
	else
	{
		pt->counterChild = i+1;
		//printf("Next node = %d  \n",pt->symbol);
		return pt->child[i];
	}
}



parseTree  parseInputSourceCode(char *testcaseFile, int T[51][54], grammar G, tokenInfo *tokenList, int no_of_tokens)
{
	tokenInfo currentToken ;
	int currentRuleNo;
	rule_ptr currentRule ;
	int ruleLength ;
	int stackTop;

	num_parse_errors = 0;


	stack s = initialise_stack();

	s = stack_push(s,TK_DOLLAR);           //pushing dollar
	s = stack_push(s,60);           // pushing <start symbol>


	parseTree pt = initialise_parsetree();

	pt = create(pt,60,60,0) ;
	int token_counter=0;
	int i=0;
	int flag = 1;
	int error_flag = 0;

	while(1)
	{

		if (flag == 1 && (token_counter < no_of_tokens) )
		{
			currentToken = tokenList[token_counter];
			token_counter++;
			flag = 0;
		}

		if(currentToken.token_id == TK_COMMENT)
		{
			flag = 1;
			continue;
		}

		//printf("Current Token is %s      ",tokenName[currentToken.token_id]);
		stackTop = stack_top(s);
		//printf("Current stack top is %s\n",tokenName[stackTop]);
		//printf("Current parse node is %s\n",tokenName[pt->symbol]);

		while(pt->symbol == 54)
			pt = nextNode(pt);

		if(stackTop == TK_DOLLAR)
		{
			if(currentToken.token_id==TK_DOLLAR)
			{
				if (lexer_errors > 0)
				{
					printf("Error recovery : Parse tree can be made ignoring lexical errors\n");
					break;
				}
				else
				{
				printf("\n---------------------------------------------------------------------\n");
				printf("  Compiled Successfully: Input source code is syntactically correct\n");
				printf("---------------------------------------------------------------------\n\n");
				break;
				}
			}
			else
			{
				//error
				sprintf(parse_errors[num_parse_errors++],"ERROR : The token <%s> for lexeme <%s> does not match at line <%d>.Expected end of input  \n\n",tokenName[currentToken.token_id],currentToken.value,currentToken.line_no);
				error_flag = 1;
				break;
			}
		}
		else if(currentToken.token_id == TK_DOLLAR && (stackTop >=0 && stackTop<=54))
		{
			sprintf(parse_errors[num_parse_errors++],"ERROR : File abrubtly ended at line %d. The expected token here is %s\n\n",currentToken.line_no,tokenName[currentToken.token_id]);
			error_flag = 1;
			break;
		}

		else if(stackTop>=TK_ASSIGNOP && stackTop <= TK_COMMA )
		{

			if(stackTop == currentToken.token_id && pt->symbol == currentToken.token_id)
			{

				pt->line_no = currentToken.line_no;
				strcpy(pt->value,currentToken.value);
				pt = nextNode(pt);
				s = stack_pop(s);
				flag = 1;
			}
			else
			{
				sprintf(parse_errors[num_parse_errors++],"ERROR : The token <%s> for lexeme <%s> does not match at line <%d>. The expected token here is <%s>\n\n",tokenName[currentToken.token_id],currentToken.value,currentToken.line_no,tokenName[stackTop]);
				error_flag = 1;
				break;

				//error
			}
		}

		else
		{
			currentRuleNo = T[stackTop-60][currentToken.token_id] ;
			//printf("Current rule no = %d\n",currentRuleNo);
			if(currentRuleNo <= 0)
			{
				sprintf(parse_errors[num_parse_errors++],"ERROR : The token <%s> for lexeme <%s> does not match at line <%d>. The possible tokens have been printed before\n ",tokenName[currentToken.token_id],currentToken.value,currentToken.line_no);
				printf("\n");
				for(i=0;i<54;i++)
				{
					if(T[stackTop-60][i] > 0)
					printf("<%s>" ,tokenName[i]);
				}
				printf("\n");

				error_flag = 1;
				break;
			}
			else
			{

				currentRule = G[(currentRuleNo/10)-60].grammar_node_head;
				int gen = currentRuleNo%10;
				int gen1=0;

				while (gen1 < gen)
				{
					currentRule = currentRule -> next_rule;
					gen1++;
				}


				//printf("sau\n");
				//printf("---------------%d--------------",G[1]->grammar_node_head[1].len_rule);
				//printf("sau\n");
				ruleLength = currentRule-> len_rule ;
				//printf("RuleLength of current rule = %d\n",ruleLength);

				for (i=0 ; i<ruleLength ; i++)
				{
					pt = create(pt,stackTop,currentRule->rule_head[i],currentRuleNo);
				}

				pt = nextNode(pt);

				s = stack_pop(s);

				for( i=ruleLength-1 ;i>=0 ;i--)
				{
					if(currentRule -> rule_head[i]==54)
						{
							//printf("eps\n");
						}
					else
					s = stack_push(s,currentRule -> rule_head[i]);
				}
			}
		}

	}
	if(error_flag == 0)
	{
		pt = root(pt);
		return pt;
	}
	else
	{
		pt = root(pt);
		return pt;
	}
}

void dfs(parseTree pt,FILE *filep,int print_yes,int *nodes,int *mem) // Recursive function to print the tree using depth-first-search traversal
{
	if (pt->symbol != TK_EPSILON)
	{
	*nodes += 1;
	*mem += sizeof(*pt);
	}
	int i=0, sym;
	sym = pt->symbol;
	/*printf("the current tokenName is : %s\n",tokenName[sym]);
	printf("\n\n\n");*/
	
	//saurabh anand

	if (pt == NULL);
	//else if(pt->child == NULL);
	else
	{
		/*printf("\n\n\n");
		printf("oye\n");
		printf("\n\n\n");*/

		if(sym>=TK_ASSIGNOP && sym<=54)
		{
			if(sym == 54);//change this to avoid epsilon production

			else if(sym == TK_NUM || sym == TK_RNUM)
			{
				//printf("\n\n\n");
				//printf("hey\n");
				//printf("\n\n\n");
				//fprintf(filep ,"Current Node = %s\n",tokenName[pt->symbol]);
				if (print_yes == 1){
					//printf("what\n");
					//printf("\n");
				printf("%21s %7d %20s %20s %30s %8s %20s \n" ,"---" ,pt->line_no ,tokenName[sym] ,pt->value,tokenName[pt->parent->symbol], "yes", "---" );
				}
			}
			else
			{
				//fprintf(filep ,"Current Node = %s\n",tokenName[pt->symbol]);
				if (print_yes == 1)
				printf("%21s %7d %20s %20s %30s %8s %20s \n" , pt->value ,pt->line_no ,tokenName[sym] ,"---",tokenName[pt->parent->symbol], "yes", "---" );
			}
		}
		else if(sym==60)
		{
			//fprintf(filep ,"Current Node = %s\n",tokenName[pt->symbol]);
			if (print_yes == 1)
			printf("%21s %7s %20s %20s %30s %8s %20s \n" , "---" , "---" ,"---" ,"---","ROOT", "no", tokenName[sym] );
		}
		else
		{
			if (print_yes == 1)
			printf("%21s %7s %20s %20s %30s %8s %20s \n" , "---" , "---" ,"---" ,"---",tokenName[pt->parent->symbol], "no", tokenName[sym] );
		}

		while(i<pt->childnum)
		{
			dfs(pt->child[i],filep,print_yes,nodes,mem);
			i++;
		}
	}

}

void printParseTree(parseTree  pt, char *outfile,int print_yes,int *nodes,int *mem)
{
	FILE* fp = NULL;
	//FILE *fp = fopen(outfile,"w+");
	//printf("entered print block:\n");
	if (print_yes == 1)
	printf("%21s %7s %20s %20s %30s %8s %20s \n\n" ,"LEXEMECURRENTNODE" ,"LINENO" ,"TOKEN" ,"VALUEIFNUMBER","PARENTNODESYMBOL", "LEAF(Y/N)", "NODESYMBOL" );
	dfs(pt,fp,print_yes,nodes,mem);
	//fclose(fp);

}

abstractSyntaxTree initialise_AST()
{
	abstractSyntaxTree ast;
	ast = NULL;
	return ast;
}

abstractSyntaxTree copyContents(abstractSyntaxTree ptr1,abstractSyntaxTree ptr2)
{
	ptr1->symbol = ptr2->symbol;
	ptr1->parent = ptr2->parent;//ptr ka parent does not change
	int i1;
	for (i1=0;i1<10;i1++)
	{
		if (ptr2->child[i1] != NULL)
		{
			ptr1->child[i1] = ptr2->child[i1];
		}
		else
		{
			ptr1->child[i1] = NULL;
		}

	}
	ptr1->childnum = ptr2->childnum;
	ptr1->counterChild = ptr2->counterChild;
	ptr1->ruleNo = ptr2-> ruleNo;
	strcpy(ptr1->value,ptr2->value);
	ptr1->line_no = ptr2->line_no;

}
abstractSyntaxTree pullContents(abstractSyntaxTree ptr1,abstractSyntaxTree ptr2)
{
	ptr1->symbol = ptr2->symbol;
	ptr1->parent = ptr1->parent;//ptr ka parent does not change
	int i1;
	for (i1=0;i1<10;i1++)
	{
		if (ptr2->child[i1] != NULL)
		{
			ptr1->child[i1] = ptr2->child[i1];
		}
		else
		{
			ptr1->child[i1] = NULL;
		}

	}
	ptr1->childnum = ptr2->childnum;
	ptr1->counterChild = ptr2->counterChild;
	ptr1->ruleNo = ptr2-> ruleNo;
	strcpy(ptr1->value,ptr2->value);
	ptr1->line_no = ptr2->line_no;

}
abstractSyntaxTree add_to_AST(abstractSyntaxTree ast, parseTree pt)
{
	int i=0;
	abstractSyntaxTree temp;
	temp= (abstractSyntaxTree )malloc(sizeof(struct _ParseTree));
	//printf("Entering function create() \n");
	if(ast==NULL)
	{
		temp->parent = NULL;
		temp->symbol = pt->symbol;
		for(i=0;i<10;i++)
				temp->child[i] = NULL;
		temp->childnum = 0;
		temp->counterChild = -1;
		temp->ruleNo = pt->ruleNo;
		strcpy(temp->value, pt->value);
		temp->line_no = pt->line_no;
		return temp;
	}
	else
	{
		temp->symbol = pt->symbol;
		for(i=0;i<10;i++)
				temp->child[i] = NULL;
		temp->childnum = 0;
		temp->counterChild = -1;
		temp->parent = ast;
		ast->child[ast->childnum] = temp;
		ast->ruleNo = (pt->parent)->ruleNo;
		ast->childnum++;
		strcpy(temp->value, pt->value);
		temp->line_no = pt->line_no;
		temp->ruleNo = pt->ruleNo;
		//printf("Current node is is %s \n",tokenName[temp->symbol]);
		//printf("Exiting function create() non-root \n");
			//printf("Current Rule No = %d",currentRuleNo);
		return temp;
		}
		//return ast;
}

abstractSyntaxTree reduction(abstractSyntaxTree ast, parseTree pt)
{
	
	int ruleNo = pt->ruleNo ;

	if (ruleNo == 600)
	{
		//printf("entered reduction for <program>\n");
		/*printf("hello sourabh\n");
		ast->child[0] = pt->child[0];
		ast->child[1] = pt->child[1];*/
	}
	else if (ruleNo == 610 || ruleNo == 660 || ruleNo == 680 || ruleNo == 681 || ruleNo == 690
		||ruleNo == 700 || ruleNo == 701 ||ruleNo == 720 || ruleNo == 730 || ruleNo == 740 || ruleNo == 790
		|| ruleNo == 800
		|| ruleNo == 820 || ruleNo == 821 || ruleNo == 822 || ruleNo == 823|| ruleNo == 824|| ruleNo == 890
		|| ruleNo == 910|| ruleNo == 951|| ruleNo == 960|| ruleNo == 961|| ruleNo == 970|| ruleNo == 971
		|| ruleNo == 972|| ruleNo == 981|| ruleNo == 982|| ruleNo == 990|| ruleNo == 1000|| ruleNo == 1010
		|| ruleNo == 1011|| ruleNo == 1030|| ruleNo == 1031|| ruleNo == 1041|| ruleNo == 1050|| ruleNo == 1060
		|| ruleNo == 1061|| ruleNo == 1070|| ruleNo == 1071|| ruleNo == 1072|| ruleNo == 1073|| ruleNo == 1074
		|| ruleNo == 1075|| ruleNo == 1080|| ruleNo == 1090|| ruleNo == 1100)
	{
		//printf("otherfunctions gives epsilon\n");
		pullContents(ast,ast->child[0]);
	}

	else if (ruleNo == 611)
	{
		/*ast->child[0] = pt->child[0];
		ast->child[1] = pt->child[1];*/
	}
	else if (ruleNo == 620)
	{
		//printf("\n\n\nreduction called on node main function\n\n\n");
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[1]);

		pullContents(ast,ast->child[0]);
		ast->childnum++;
		ast->child[0]=ta;

		/*ast = pt->child[0];
		ast->child[0] = pt->child[1];
		free(pt->child[2]);
		free(pt->child[0]);*/
	}
	else if (ruleNo == 630)
	{
		//printf("entered reduction for <stmts>\n");
		/*ast->child[0] = pt->child[0];
		ast->child[1] = pt->child[1];
		ast->child[2] = pt->child[2];
		ast->child[3] = pt->child[3];*/
	}
	else if (ruleNo == 640)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[1]);
		abstractSyntaxTree tb=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tb,ast->child[2]);
		abstractSyntaxTree tc=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tc,ast->child[4]);

		pullContents(ast,ast->child[0]);
		ast->childnum=3;
		ast->child[0]=ta;
		ast->child[1]=tb;
		ast->child[2]=tc;
		
	}
	else if (ruleNo == 650)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[4]);
		

		pullContents(ast,ast->child[0]);
		ast->childnum=1;
		ast->child[0]=ta;
		

	}
	
	else if (ruleNo == 661)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[4]);
		

		pullContents(ast,ast->child[0]);
		ast->childnum=1;
		ast->child[0]=ta;
		
	}

	else if (ruleNo == 670)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[0]);
		abstractSyntaxTree tb=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tb,ast->child[2]);
		

		pullContents(ast,ast->child[1]);
		ast->childnum=2;
		ast->child[0]=ta;
		ast->child[1]=tb;

		/*
		ast = pt->child[1];
		ast->child[0] = pt->child[0];
		ast->child[1] = pt->child[2];
		free(pt->child[0]);*/
	}
	else if (ruleNo == 691)
	{
		pullContents(ast,ast->child[1]);
	}
	
	else if (ruleNo == 710)
	{
		pullContents(ast,ast->child[1]);
	}
	else if (ruleNo == 720)
	{
		pullContents(ast,ast->child[0]);
	}

	else if (ruleNo == 721)
	{
		
	}
	
	else if (ruleNo == 731)
	{
		
	}
	

	else if (ruleNo == 741)
	{
		
	}
	else if (ruleNo == 750)
	{
		//printf("hello saurabh\n");
		//remember to free nodes
		ast =pullContents(ast,ast->child[1]);
		//printf(" i have changed the node")
		/*ast = pt->child[1];
		free(pt->child[0]);
		free(pt->child[1]);
		free(pt->child[2]);*/
	}
	else if (ruleNo == 760)
	{

		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[2]);
		

		pullContents(ast,ast->child[1]);
		ast->childnum=1;
		ast->child[0]=ta;
	}
	else if (ruleNo == 770)
	{
		
	}
	else if (ruleNo == 780)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[1]);

		pullContents(ast,ast->child[3]);
		ast->childnum=1;
		ast->child[0]=ta;

		/*ast = pt->child[3];
		ast->child[0] = pt->child[1];
		free(pt->child[0]);
		free(pt->child[2]);
		free(pt->child[3]);
		free(pt->child[4]);*/
	}

	else if (ruleNo == 791)
	{
		/*ast->child[0] = pt->child[0];
		ast->child[1] = pt->child[1];*/
	}
	

	else if (ruleNo == 801)
	{


		pullContents(ast,ast->child[1]);
		

	}
	else if (ruleNo == 810)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[1]);
		abstractSyntaxTree tb=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tb,ast->child[4]);

		pullContents(ast,ast->child[3]);
		ast->childnum=2;
		ast->child[0]=ta;
		ast->child[1]=tb;
		/*
		ast = pt->child[3];
		ast->child[0] = pt->child[1];
		ast->child[1] = pt->child[4];*/

	}
	
	
	else if (ruleNo == 830)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[0]);
		abstractSyntaxTree tb=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tb,ast->child[2]);

		pullContents(ast,ast->child[1]);
		ast->childnum=2;
		ast->child[0]=ta;
		ast->child[1]=tb;
		
	}
	else if (ruleNo == 840)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[2]);
		abstractSyntaxTree tb=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tb,ast->child[4]);
		abstractSyntaxTree tc=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tc,ast->child[5]);

		pullContents(ast,ast->child[0]);
		ast->childnum=3;
		ast->child[0]=ta;
		ast->child[1]=tb;
		ast->child[2]=tc;
		/*
		ast->child[0] = pt->child[2];
		ast->child[1] = pt->child[4];
		ast->child[2] = pt->child[5];
		free(pt->child[0]);
		free(pt->child[1]);
		free(pt->child[3]);
		free(pt->child[6]);*/
	}
	else if (ruleNo == 850)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[2]);
		abstractSyntaxTree tb=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tb,ast->child[5]);
		abstractSyntaxTree tc=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tc,ast->child[6]);
		abstractSyntaxTree td=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(td,ast->child[7]);

		pullContents(ast,ast->child[0]);

		
		ast->childnum=4;
		ast->child[0]=ta;
		ast->child[1]=tb;
		ast->child[2]=tc;
		ast->child[3]=td;		
	}
	else if (ruleNo == 860)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[2]);
		

		pullContents(ast,ast->child[0]);
		ast->childnum=1;
		ast->child[0]=ta;
	
	}
	else if (ruleNo == 861)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[2]);
		

		pullContents(ast,ast->child[0]);
		ast->childnum=1;
		ast->child[0]=ta;
	}
	else if (ruleNo == 870)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[0]);
		abstractSyntaxTree tb=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tb,ast->child[5]);

		pullContents(ast,ast->child[2]);
		ast->childnum=2;
		ast->child[0]=ta;
		ast->child[1]=tb;
		/*ast = pt->child[2];
		ast->child[0] = pt->child[0];
		ast->child[1] = pt->child[5];
		free(pt->child[1]);
		free(pt->child[2]);
		free(pt->child[3]);
		free(pt->child[4]);
		free(pt->child[6]);*/
	}
	else if (ruleNo == 880)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[1]);
		

		pullContents(ast,ast->child[0]);
		ast->childnum=1;
		ast->child[0]=ta;
		/*
		ast = pt->child[0];
		ast->child[0] = pt->child[1];
		free(pt->child[0]);*/
	}
	
	else if (ruleNo == 891)
	{
		pullContents(ast,ast->child[1]);
	}
	else if (ruleNo == 900)
	{
		/*abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[0]);
		

		pullContents(ast,ast->child[1]);
		ast->childnum=1;
		ast->child[0]=ta;*/
		/*ast = pt->child[1];
		ast->child[0] = pt->child[0];*/
	}
	

	else if (ruleNo == 911)
	{
		pullContents(ast,ast->child[1]);
		/*ast = pt->child[1];
		free(pt->child[0]);
		free(pt->child[1]);
		free(pt->child[2]);
		free(pt->child[3]);*/

	}
	else if (ruleNo == 920)
	{
		pullContents(ast,ast->child[1]);
		/*ast = pt->child[1];
		free(pt->child[0]);
		free(pt->child[1]);
		free(pt->child[2]);*/
	}
	else if (ruleNo == 930)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[1]);
		

		pullContents(ast,ast->child[0]);
		ast->childnum=1;
		ast->child[0]=ta;
		/*ast = pt->child[0];
		ast->child[0] = pt->child[1];
		free(pt->child[0]);*/
	}
	else if (ruleNo == 940)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[1]);
		abstractSyntaxTree tb=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tb,ast->child[5]);

		pullContents(ast,ast->child[3]);
		ast->childnum=2;
		ast->child[0]=ta;
		ast->child[1]=tb;
		/*ast = pt->child[3];
		ast->child[0] = pt->child[1];
		ast->child[1] = pt->child[5];
		free(pt->child[0]);
		free(pt->child[2]);
		free(pt->child[3]);
		free(pt->child[4]);
		free(pt->child[6]);*/
	}
	else if (ruleNo == 941)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[0]);
		abstractSyntaxTree tb=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tb,ast->child[2]);

		pullContents(ast,ast->child[1]);
		ast->childnum=2;
		ast->child[0]=ta;
		ast->child[1]=tb;
		/*ast = pt->child[1];
		ast->child[0] = pt->child[0];
		ast->child[1] = pt->child[2];
		free(pt->child[1]);*/
	}
	else if (ruleNo == 942)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[2]);
		

		pullContents(ast,ast->child[0]);
		ast->childnum=1;
		ast->child[0]=ta;
		
	}
	else if (ruleNo == 950)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[1]);
		abstractSyntaxTree tb=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tb,ast->child[2]);

		pullContents(ast,ast->child[0]);
		ast->childnum=2;
		ast->child[0]=ta;
		ast->child[1]=tb;
		/*ast->child[0] = pt->child[1];
		ast->child[1] = pt->child[2];
		free(pt->child[0]);
		free(pt->child[3]);*/
	}	
	
	else if (ruleNo == 962)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[1]);
		

		pullContents(ast,ast->child[0]);
		ast->childnum=1;
		ast->child[0]=ta;
	}
	
	
	else if (ruleNo == 980)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[1]);
		pullContents(ast,ast->child[0]);
		ast->childnum=1;
		ast->child[0]=ta;
	}
	
	
	else if (ruleNo == 991)
	{
		pullContents(ast,ast->child[1]);
	}
	

	else if (ruleNo == 1001)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[1]);
		abstractSyntaxTree tb=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tb,ast->child[2]);

		pullContents(ast,ast->child[0]);
		ast->childnum=2;
		ast->child[0]=ta;
		ast->child[1]=tb;
		/*ast = pt->child[0];
		ast->child[0] = pt->child[1];
		ast->child[1] = pt->child[2];
		free(pt->child[0]);*/
	}
	
	else if (ruleNo == 1020)
	{
		/*abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[0]);
		pullContents(ast,ast->child[1]);
		ast->childnum=1;
		ast->child[0]=ta;*/
		

		/*ast->child[0] = pt->child[0];
		ast->child[1] = pt->child[1];*/
	}
	
	else if (ruleNo == 1040)
	{
		pullContents(ast,ast->child[1]);
	}
	
	

	else if (ruleNo == 1051)
	{
		abstractSyntaxTree ta=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(ta,ast->child[1]);
		abstractSyntaxTree tb=(abstractSyntaxTree)malloc(sizeof(struct _ParseTree));
		copyContents(tb,ast->child[2]);

		pullContents(ast,ast->child[0]);
		ast->childnum=2;
		ast->child[0]=ta;
		ast->child[1]=tb;
	}	

	else if (ruleNo == 1081)
	{
		pullContents(ast,ast->child[1]);
	}
	

	else if (ruleNo == 1091)
	{
		pullContents(ast,ast->child[1]);
	}
	

	else if (ruleNo == 1101)
	{
		pullContents(ast,ast->child[1]);
	}
	else
	{
		//printf("reduction function called for %s\n",tokenName[ast->symbol]);
	}

	return ast;
}

abstractSyntaxTree generateAST(abstractSyntaxTree ast ,parseTree pt)
{
	int i=0 ;
	parseTree temp;
	temp = pt->parent;
	if (pt == NULL)
	{
		printf("the parse tree is not created yet\n");
	}
	//else if(pt->child==NULL && temp->counterChild < temp->childnum)//print some message
	/*else if((pt->child == NULL && temp!= NULL && temp->counterChild < temp->childnum))
	//print some message //shouldn't this be childnum =0 , 
	//temp should be checked for NULL before checking the other condition
	{
		//||(pt->childnum==1 && pt->child[0]->symbol == TK_EPSILON)
		
	}*/

	else
	{
		if (ast != NULL)
		{
			//printf("add ast function called for %s and %s\n",tokenName[ast->symbol],tokenName[pt->symbol]);			
		}
		
		else
		{
			//printf("ast is null \n");
		}
		ast = add_to_AST(ast ,pt);



		//printf("pt is %s and child num is %d\n",tokenName[pt->symbol],pt->childnum);
		while(i<(pt->childnum))
		{
			pt->counterChild = i;
			if (ast!=NULL);
			//printf("generate ast function called for %s and %s\n",tokenName[ast->symbol],tokenName[pt->symbol]);
			else
			{
			//printf("ast is null \n");
			}
	
			ast = generateAST(ast ,pt->child[i]);

			i++;
		}
		ast = reduction(ast,pt);
		//check if this condition is correct otherwise root ka parent NULL return ho jayega
		if (ast-> parent != NULL)
		ast = ast->parent;
	}
	return ast;
}
