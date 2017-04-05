#include <stdio.h>


int N,w[100],count=0;

int knapsack(int W, int i)			//knapsack function module
{
	count++;
	if(W==0)
		return(1);
	else if( W<0 || i>N )
		return(0);
	else if(knapsack(W-w[i],i+1))
	{
		printf("%d ",w[i]);
		return(1);
	}
	else
		return(knapsack(W,i+1));
}
int main()
{
	int W,i;
		
	printf("\nEnter W : ");
	scanf("%d",&W);
	printf("\nEnter the number of array elements : ");
	scanf("%d",&N);
	printf("\nEnter the elements of the array : ");
	for(i=0 ; i<N;i++)
		scanf("%d",&w[i]);
	knapsack(W,0);
	printf("\n Count : %d",count);
	return(0);
}

