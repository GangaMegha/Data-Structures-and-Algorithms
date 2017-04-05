#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int N,w[20],count;

int knapsack(int W, int i)			//knapsack function module
{
	count++;
	if(W==0)
		return(1);
	else if( W<0 || i>N )
		return(0);
	else if(knapsack(W-w[i],i+1))
	{
		//printf("%d ",w[i]);
		return(1);
	}
	else
		return(knapsack(W,i+1));
}
int main()
{
	int i,j,W,max_count,min_count;
	time_t t;

	printf("\n\tN\tMin\t Max");
	
	for(N=1;N<21;N++)
	{
		srand((unsigned) time(&t));		//providing a seed to rand

		max_count=0;
		min_count=1000;
		for(j=0;j<10000;j++)
		{
			count=0;

			for(i=0;i<N;i++)
				w[i]=rand()%N;
			W=rand()%((int)(N*N/2)+1);
			if(knapsack(W,0)!=1)
			{
				printf("\nKnapsack unsuccessful!! ABORT!!");
				break;
			}
			if(max_count<count)
				max_count=count;
			if(min_count>count)
				min_count=count;
		}
		printf("\n\t%d\t%d\t%d",N,min_count,max_count);
	}
	return(0);
}

/******************************************OUTPUT*****************************************************************/
/*

	N	Min	 Max
	1	1	1
	2	1	15
	3	1	31
	4	1	63
	5	1	127
	6	1	255
	7	1	511
	8	1	1023
	9	1	2047
	10	1	4095
	11	1	8191
	12	1	16383
	13	1	32767
	14	1	65535
	15	1	131071
	16	1	262143
	17	1	524287
	18	1	1048575
	19	1	2097151
	20	1	4194303

*/


/*   For N>1 , Max=(2^(N+2))-1  

	Order = O(2^N) */
