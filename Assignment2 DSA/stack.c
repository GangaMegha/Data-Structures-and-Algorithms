/*******************************************************************************************************************************/

											/*ARRAY IMPLEMENTATION OF STACK*/

/********************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//#include <math.h>

/*

Assuming : 3e4=3*(10^4)

The program works for all numbers with any number of digits or decimal places allowed for float. 

As inbuilt functions pow() and isdigit() didn't run in the compilor used, these functions have been defined explicitly

*/

#define max 80


/************************************************************************************************************************/

struct stack1
{
	int top;
	float items[max];
};

float power(float a, float b)	//to find a^b, here a=10 always
{
	float i;
	float pro=a;
	for(i=1;i<b;i++)
		pro*=a;
	return pro;
}

int ifdigit(char c)			// To check whether c is a digit
{
	//printf("\n\nReached ifdigit\n");
	if(0<=(c-'0') && (c-'0')<=9)
		return 1;
	return 0;
}

float number(char expr[],int *pos)			//To get the natural number if it has more than 1 digit
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

float decimal(char expr[],int *pos)			//To get the decimal part of the number under consideration  
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


float pop(struct stack1 *ps)			//Function to pop
{
	if(ps==NULL)
	{
		printf("\nStack Underflow!\n");
		exit(1);
	}
	float val=ps->items[ps->top];
	//printf ("\nPopped value is: %f ",val);
	(ps->top)--;
	return(val);
}

float push(struct stack1 *ps, float x)			//Function to push
{
	if(ps->top==max-1)
	{
		printf("\nStack Overflow!\n");
		exit(1);
	}
	else
	{
		++(ps->top);
		ps->items[ps->top]=x;
		//printf("\n Pushed value : %f",x);;

	}
	return(0);
}

float error(char expr[],int *pos,int position)			// Common function used to check error after e (if present) after a number and evaluate the digits after e
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


float oper(char c, float op1, float op2)		//To carry out the operation on the popped numbers and operator
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
	struct stack1 opndstk;
	opndstk.top=-1;
	for(pos=0;(c=expr[pos])!='\0';pos++)
	{	//printf("\n\n\texpr(pos) : %c ",expr[pos]);
		position=pos;
		switch(c)					//Checking if c is a valid operator
		{
			case '+' :
			case '-' :
			case '*' :
			case '/' :	if(expr[pos+1]==' ' || expr[pos+1]=='\0'  || expr[pos+1]=='\n')
						{	
							if((opndstk.top)<1)			//stack has less than 2 operands
							{
								printf("Invalid expr. “%c” does not have enough operands",c);
								exit(0);
							}
							else
							{
								op2=pop(&opndstk);
								op1=pop(&opndstk);
								value=oper(c,op1,op2);
								push(&opndstk,value);
							}
						}
						else if((c=='-' || c=='+') && (r=ifdigit(expr[pos+1]))==1)	//Checking for positive and negative numbers
						{	
							if(r==1)
							{
								pos++;
								temp1=number(expr,&pos);	//temp1 holds the number
								//printf("\nTemp1 : %f",temp1);
							}
							if(expr[pos+1]==' ' || expr[pos+1]=='\0'  || expr[pos+1]=='\n')
							{
								if(c=='-')
									push(&opndstk,(float)((-1)*temp1));
								else
									push(&opndstk,(float)(temp1));
								
							}	

							else if(expr[pos+1]=='.' && (r=ifdigit(expr[pos+2]))==1)		//checking for decimal digits
							{
								if(r==1)
								{
									pos=pos+2;
									temp2=decimal(expr,&pos);		//temp2 holds the decimal part
								}

								if(expr[pos+1]==' ' || expr[pos+1]=='\0' || expr[pos+1]=='\n')
								{
									if(c=='-')
										push(&opndstk,(float)((-1)*(temp1+temp2)));
									else
										push(&opndstk,(float)(temp1+temp2));
										
								}
								
								else if(expr[pos+1]!=' ' && expr[pos+1]!='\0'  && expr[pos+1]!='\n')
								{	
									//j=pos-position+1;
									//r=error(expr,position,j);
									pos++;
									temp3=error(expr,&pos,position);	//temp3 holds the number after e 
									if(temp3!=0)
									{	
										temp=(power(10,temp3));
										if(c=='-')
											push(&opndstk,(float)((-1)*(temp1+temp2)*temp));
										else
											push(&opndstk,(float)(temp1+temp2)*temp);
									
									}
								}
							}
							else 			//If no decimal part to the number
							{	
								//j=2;
								//r=error(expr,pos,j);
								pos++;
								temp3=error(expr,&pos,position);
								if(temp3!=0)
								{	
									temp=(float)(power(10,temp3));
									if(c=='-')
										push(&opndstk,-1*temp1*temp);
									else
										push(&opndstk,temp1*temp);
									
								}
							}

						}
						break;
							
			default : 	if(ifdigit(c)==1)
						{	
							temp1=number(expr,&pos);		//temp1 holds the number
							
							if(expr[pos+1]==' ' || expr[pos+1]=='\0' || expr[pos+1]=='\n')
							{
								push(&opndstk,(float)(temp1));
								//printf("\n Pushed value : %f",(float)(c-'0'));
							}

										
							else if(expr[pos+1]=='.' && (r=ifdigit(expr[pos+2]))==1)	//checking for decimal part
							{
								if(r==1)
								{
									pos=pos+2;
									temp2=decimal(expr,&pos);	//temp2 holds the decimal part
								}

								if(expr[pos+1]==' ' || expr[pos+1]=='\0' || expr[pos+1]=='\n')
								{
									push(&opndstk,(float)(temp1+temp2));	
								}
								
								
								else if(expr[pos+1]!=' ' && expr[pos+1]!='\0' && expr[pos+1]!='\n')
								{	
									//j=3;
									//r=error(expr,pos,j);
									pos++;
									temp3=error(expr,&pos,position);	//temp3 holds the number after e

									if(temp3!=0)
									{	
										temp=(power(10,temp3));
										push(&opndstk,(float)(temp1+temp2)*temp);
									
									}
								}
								
							}
							else 	//If no decimal part, then checking for e
							{	
								//j=1;
								//r=error(expr,pos,j);
								pos++;
								temp3=error(expr,&pos,position);		//temp3 holds the part after e
								if(temp3!=0)
								{	
									temp=(float)(power(10,temp3));
									push(&opndstk,temp1*temp);
								}
							}
							
						}
						else if(c!=' ' && c!='\0'  && c!='\n')		//if some other character present
						{
							printf("\nInvalid character %c present in input!!",c);
							exit(0);
						}
						break;
			
		}
	}
	if(expr[pos]=='\0')		//String traversed
	{
		if((opndstk.top)==0)	//only one final element left
			return(pop(&opndstk));
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