/*******************************************************************************************************************************/

											/*LINKED LIST IMPLEMENTATION OF STACK*/

/********************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

# define max 80
/*

Assuming : 34e11=34*(10^11)

The program works for all numbers with any number of digits or decimal places allowed for float. 

As inbuilt functions pow() and isdigit() didn't run in the compilor used, these functions have been defined explicitly

*/

/************************************************************************************************************************/

struct stack1		//self referential structure
{
	float item;
	struct stack1 *label;
};

float power(float a, float b)		//to find power of 10^b or a^b
{
	float i;
	float pro=a;
	for(i=1;i<b;i++)
		pro*=a;
	return pro;
}

int ifdigit(char c)		//to check if c is a digit
{
	//printf("\n\nReached ifdigit\n");
	if(0<=(c-'0') && (c-'0')<=9)
		return 1;
	return 0;
}

float number(char expr[],int *pos)	//To find the number in case of more that one digit
{
	int i=*pos;
	float val=0;
	do
	{
		//printf("val : %f",val);
		val=val*10+(float)(expr[i]-'0');
		i++;

	}while(ifdigit(expr[i]));
	*pos=i-1;
	return val;
}

float decimal(char expr[],int *pos)		//to find the decimal part of the number
{
	float pro=10.0;
	int i=*pos;
	float val=0;
	do
	{
		val=val + (float)(expr[i]-'0')/pro;
		i++;
		pro*=10;

	}while(ifdigit(expr[i]));
	*pos=i-1;
	return val;
}


float pop(struct stack1 **top)		//to pop
{
	if((*top)==NULL)
	{
		printf("\nStack Underflow!\n");
		exit(1);
	}
	float val=(*top)->item;
	//printf("top->item : %f\n",val );
	//struct stack1 *ps=(struct stack1 *)malloc(sizeof(struct stack1));
	struct stack1 *ps;
	ps=*top;
	(*top)=(*top)->label;
	free(ps);
	//printf ("\nPopped value is: %f ",val);
	return(val);
}

float push(struct stack1 **top, float x)		//to push
{
	struct stack1 *ps=(struct stack1 *)malloc(sizeof(struct stack1));
	ps->item=x;
	if((*top)==NULL)
	{
		ps->label=NULL;
		*top=ps;
	}
	else
	{
		ps->label=*top;
		*top=ps;
	}
	
	//printf("\n Pushed value : %f",(*top)->item);
	return(0);
}

float error(char expr[],int *pos,int position)	// Common function used to check error after e (if present) after a number and evaluate the digits after e
{
		int i,flag=0,r;
		int t=*pos;
		float val;
		if(expr[t]!='e' && expr[t]!=' ' && expr[t]!='\n' && expr[i]!='\0')
		{
			printf("\nInvalid expr. Illegal tag: ");
			for(i=position; expr[i]!=' ' && expr[i]!='\n' && expr[i]!='\0';i++)
			printf("%c",expr[i]);
			exit(0);
		}
		else if(expr[t]=='e')
		{	
			for(i=t+1;expr[i]!=' ' && expr[i]!='\n' && expr[i]!='\0';i++)
				if((r=ifdigit(expr[i]))!=1)
				{
					flag=1;
				}					
					
			if(flag==1)
			{
				printf("\nInvalid expr. Illegal tag: ");
				for(i=position;expr[i]!=' ' && expr[i]!='\n'  && expr[i]!='\0';i++)
				printf("%c",expr[i]);
				exit(0);
			}
			else
			{	
				t++;
				*pos=t;
				val=number(expr,pos);
				return(val);
			}
		
		}
	return(0);
}


float oper(char c, float op1, float op2)		//to carry out the operation after popping the numbers
{
	int flag = 0;
	switch(c)
	{
		case'+':return(op1 + op2);
		case'-':return(op1 - op2);
		case'*':return(op1 * op2);
		case'/':return(op1 / op2);
		default : flag=1;
	}
	if(flag==1)
	{
		printf("\nThe operator not recognised!! Terminating!!");
		exit(0);
	}	
	return(0);
}

float eval(char expr[])		//To read string , push new numbers to stack and pop when encountering a valid operator and to print out errors
{
	char c;
	int pos,i,j,r,position;
	float op1,op2,value,temp,temp1,temp2,temp3;
	struct stack1 *top=(struct stack1 *)malloc(sizeof(struct stack1));
	top=NULL;
	for(pos=0;(c=expr[pos])!='\0';pos++)
	{	//printf("\n\n\texpr(pos) : %c ",expr[pos]);
		position=pos;
		switch(c)		//checking for operators
		{
			case '+' :
			case '-' :
			case '*' :
			case '/' :	if(expr[pos+1]==' ' || expr[pos+1]=='\0'  || expr[pos+1]=='\n')
						{	
							if(top==NULL || top->label==NULL)
							{
								printf("Invalid expr. “%c” does not have enough operands",c);
								exit(0);
							}
							else
							{
								op2=pop(&top);
								op1=pop(&top);
								value=oper(c,op1,op2);
								push(&top,value);
							}
						}
						else if((c=='-' || c=='+') && (r=ifdigit(expr[pos+1]))==1)	//Checking for positive and negative numbers
						{	
							if(r==1)
							{
								pos++;
								temp1=number(expr,&pos);			//temp1 holds the number
								//printf("\nTemp1 : %f",temp1);
							}
							if(expr[pos+1]==' ' || expr[pos+1]=='\0'  || expr[pos+1]=='\n')
							{
								if(c=='-')
									push(&top,(float)((-1)*temp1));
								else
									push(&top,(float)(temp1));
								
							}	

							else if(expr[pos+1]=='.' && (r=ifdigit(expr[pos+2]))==1)
							{
								if(r==1)
								{
									pos=pos+2;
									temp2=decimal(expr,&pos);		//temp2 holds the decimal part
								}

								if(expr[pos+1]==' ' || expr[pos+1]=='\0' || expr[pos+1]=='\n')
								{
									if(c=='-')
										push(&top,(float)((-1)*(temp1+temp2)));
									else
										push(&top,(float)(temp1+temp2));
										
								}
								
								else if(expr[pos+1]!=' ' && expr[pos+1]!='\0'  && expr[pos+1]!='\n')
								{	
									//j=pos-position+1;
									//r=error(expr,position,j);
									pos++;
									temp3=error(expr,&pos,position);		//temp3 holds the natural number after e
									if(temp3!=0)
									{	
										temp=(power(10,temp3));
										if(c=='-')
											push(&top,(float)((-1)*(temp1+temp2)*temp));
										else
											push(&top,(float)(temp1+temp2)*temp);
									
									}
								}
							}
							else //if decimal part not present
							{	
								//j=2;
								//r=error(expr,pos,j);
								pos++;
								temp3=error(expr,&pos,position);
								if(temp3!=0)			//temp3 holds the part after e
								{	
									temp=(float)(power(10,temp3));
									if(c=='-')
										push(&top,-1*temp1*temp);
									else
										push(&top,temp1*temp);
									
								}
							}

						}
						break;
							
			default : 	if(ifdigit(c)==1)
						{	
							temp1=number(expr,&pos);			//temp1 holds the number
							
							if(expr[pos+1]==' ' || expr[pos+1]=='\0' || expr[pos+1]=='\n')
							{
								push(&top,(float)(temp1));
								//printf("\n Pushed value : %f",(float)(c-'0'));
							}

										
							else if(expr[pos+1]=='.' && (r=ifdigit(expr[pos+2]))==1)
							{
								if(r==1)
								{
									pos=pos+2;
									temp2=decimal(expr,&pos);			//temp2 holds the decimal part
								}

								if(expr[pos+1]==' ' || expr[pos+1]=='\0' || expr[pos+1]=='\n')
								{
									push(&top,(float)(temp1+temp2));	
								}
								
								
								else if(expr[pos+1]!=' ' && expr[pos+1]!='\0' && expr[pos+1]!='\n')
								{	
									//j=3;
									//r=error(expr,pos,j);
									pos++;
									temp3=error(expr,&pos,position);		//temp3 holds the part after e

									if(temp3!=0)
									{	
										temp=(power(10,temp3));
										push(&top,(float)(temp1+temp2)*temp);
									
									}
								}
								
							}
							else //if decimal part not present
							{	
								//j=1;
								//r=error(expr,pos,j);
								pos++;
								temp3=error(expr,&pos,position);		//temp3 holds the part after e
								if(temp3!=0)
								{	
									temp=(float)(power(10,temp3));
									push(&top,temp1*temp);
								}
							}
							
						}
						else if(c!=' ' && c!='\0'  && c!='\n')		//if an unwanted character is present abort
						{
							printf("\nInvalid character %c present in input!!",c);
							exit(0);
						}
						break;
			
		}
	}
	if(expr[pos]=='\0')		//string traversed
	{
		if((top->label)==NULL && top!=NULL)		//only one element left in stack
			return(pop(&top));
		else
		{
			printf("\nInvalid expr. The number of operands is too many compared to the operators!!");
			exit(0);
		}
	}

		return(0);
}



int main()
{
	char expr[max];
	int position=0;
	//while((expr[position++]=getchar())!='\n');
	//position--;
	do
	{
		expr[position]=getchar();
		position++;

	}while(expr[position-1]!='\n');
	position--;
	expr[position]='\0';
	printf("%s%s","The original postfix expression is ",expr);
	printf("\n\n%f\n",eval(expr));
	return 0;
}