#include <stdio.h>

int sum(int coloured[30], int n)
{
	int i,Sum=0;
	for(i=0;i<n;i++)
	{
		Sum+=coloured[i];
		if(Sum>0)
			return Sum;
	}
	return Sum;
}

void main()
{
	char node[30][2];			//Contains the nodes.
	char new_clr[60];			//Contains a particular colour group.
	int inced_matrix[30][30]={0},i,j,n;		//n denotes the number of nodes , inced_matrix contains the incidence matrix , i&j used for iterations
	int coloured[30]={0};				//Used to denote the nodes already grouped. 
	int group[30][60],extra[30][60]; 		/*group contains the nodes in a particular colour group in each row (turns)
								  extra contains the extra nodes that are valid for the particular colour group in each row*/
	int indices[30][30];			//To hold the indices og the group elements
	int num_nodes[30][2];		//Contains number of nodes in group and extra respectively

	char colour[15][15] = {"Blue","Red","Green","Yellow","Black","Brown","Orange","Violet","Grey","White","Pink","Magenta","Golden","Silver","Cream"};


/****************************************************************************************************************************************************************/	

//Accepting the inputs from the input file specified along with ./a.out < input.file
	scanf("%d",&n);
	
	for(i=0;i<n;i++)
	{
			coloured[i]=1;		//0 means assigned a colour. 
			scanf("%s",node[i]);
	}

	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			scanf("%d",&inced_matrix[i][j] );
	
	//Initialising coloured to 1
	for(i=0;i<n;i++)
		coloured[i]=1;	//1 denotes , not assigned a colour
			

	//Program logic inside function greedy

	int s=sum(coloured,n);	//keeps track of the node list, such that all the nodes have been assinged to a group.
	int count=0;		//keeps track of the no. of groups
	int track=0;			//to keep track of the first member position in the current group considered, so as to minimise the number of iterations(computations).
	int found;

	
/***************************************************************************************************************************************************************/

//Loop to form the groups or turns

	while(s>0)			
	{					
		for(i=track;i<n;i++)			//loop to assign the first member of the group[count]
			if(coloured[i]==1)
			{
				group[count][0]=node[i][0];
				group[count][1]=node[i][1];
				indices[count][0]=i;
				coloured[i]=0;
				track=i+1;
				num_nodes[count][0]=2;
				break;
			}
		for(i=track;i<n;i++)			//loop to assign the rest of the group members in group[count]
			if(coloured[i]==1)
			{
				found=0;
				for(j=0;j<num_nodes[count][0];j=j+2)
					if(inced_matrix[indices[count][j/2]][i]==1)
					{
						found=1;
						break;
					}
				if(found==0)
					{
						group[count][num_nodes[count][0]]=node[i][0];
						group[count][num_nodes[count][0]+1]=node[i][1];
						indices[count][num_nodes[count][0]/2]=i;
						coloured[i]=0;
						num_nodes[count][0]+=2;
					}
			}

		//Note : num_nodes[count][0] / 2 gives the number of nodes in group[count]

		count++;
		s=sum(coloured,n);
	}

	int k,t;

/************************************************************************************************************************************************************/

//Loop to find out the extra possibilities

	for(i=0;i<count;i++)	
	{
		t=0;
		num_nodes[i][1]=t;		//Number of nodes in extra[i][] = num_nodes[i][1] / 2
		for(j=0;j<n;j++)		//loop that runs through the entire set of nodes
		{
			found=0;
			for(k=0;k<num_nodes[i][0];k=k+2)		//loop that iterates through 'i'th row of group[][] to check for equal nodes
				if(node[j][0]==group[i][k] && node[j][1]==group[i][k+1])
				{
					found=1;	//the node[j] is already present in group[i] and hence , not required in extra[i]
					break;
				}

			if(found==0)
			{	
				for(k=0;k<num_nodes[i][0];k=k+2)			//loop that iterates through 'i'th row of group[][] to check for edge
					if(inced_matrix[indices[i][k/2]][j]==1)
					{
						found=1;
						break;
					}					//edge found, implies node[j] shouldn't be added to extra[i][] 
				if(found==0)			//edge not found, implies, add node[j] to extra[i][]
				{
					for(k=0;k<num_nodes[i][1];k=k+2)			//loop that iterates through 'i'th row of extra[][] to check for edge
						if(inced_matrix[indices[i][k/2]][j]==1)
						{
							found=1;
							break;
						}					//edge found, implies node[j] shouldn't be added to extra[i][] 
					if(found==0)		//edge not found, implies, add node[j] to extra[i][]
					{
						extra[i][t]=node[j][0];
						extra[i][t+1]=node[j][1];
						t+=2;
						num_nodes[i][1]=t;		//Number of nodes in extra[i][] = num_nodes[i][1] / 2
					}
				}

			}


		}
		

	}

/*******************************************************************************************************************************************************************/

//Printing the table on to the output file specified with ./a.out < input.file > output.file

	printf("\n\n\tColor\t\t\t\tTurns\t\t\t\tExtras\n---------------------------------------------------------------------------\n");
	for(i=0;i<count;i++)
	{	
		printf("\n\t%s\t\t\t",colour[i]);
		
		for(j=0;j<num_nodes[i][0]-2;j+=2)
			printf("%c%c,",group[i][j],group[i][j+1]);
		printf( "%c%c\t\t\t\t", group[i][num_nodes[i][0]-2] , group[i][num_nodes[i][0]-1] );
		
		for(j=0;j<num_nodes[i][1]-2;j+=2)
			printf("%c%c,",extra[i][j],extra[i][j+1]);
		printf( "%c%c\t", extra[i][num_nodes[i][1]-2] , extra[i][num_nodes[i][1]-1] );
			
	}

	
}


//The time complexity of the algorithm will be O(n^2). The rest of the algoritm has iterations less than that.