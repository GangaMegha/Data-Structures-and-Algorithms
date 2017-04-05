/*******************************************************Question 3*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define N 50	//no. of individuals
#define no_of_children_max 10	//maximum no. of children per individual

typedef struct 
{
	char name[15];
	char death_age[3];
}Person;

Person P[N];	//stores required information about each individual

int g[50]={0};	//no.of great grandkids 

int c[N][no_of_children_max],edge[N][no_of_children_max]; //c[i][j] stores the index of jth child and edge stores the age of i when jth child was born
int p[N][2],cno[N];	// p[i][] stores the two parents(if there) of node i
int Count;	//used to print out the no. of descendents of each individual


/**********************************function to find the ancestors*****************************************/ 

void find_ancestor()		//3a answer. To find the ancestors
{
	int i;
	printf("\nThe ancestors are : ");
	for(i=0;i<N;i++)
		if(p[i][0]==-1 && p[i][1]==-1)	//if they have no parents at all, then they are ancestors.
		{
			printf("\n\t%s",P[i].name);		
			if(cno[i]==0)					//if isolated node
				printf("  (but no descendants)");
		}
}


/**********************************function to find the no. of descendants of each individual*****************************************/ 

void find(int pos)		//recursive function for finding the no. of descendents of a node 
{
	int i;
	if(cno[pos]!=0)		// if the node at pos has children
	{
		for(i=0;i<cno[pos];i++)
		{
			Count++;	//children of node at pos counted
			find(c[pos][i]);	//recursive function call to calculate the no. of rest of the descendents of node at c[pos][i]
		}
	}
}

void no_of_decendants()		//function used to calculate the no. of descendents of each node.
{
	int i,j;
	printf("\n\nPerson  No. of descendants");
	for(i=0;i<N;i++)
	{
		Count=0;	//initialising no. of descendents as 0
		for(j=0;j<cno[i];j++)	//accessing all the children of the given node
		{
			Count++;		//children of considered node counted
			find(c[i][j]);	//recursive function to calculate the no. of rest of the descendents
		}
		printf("\n%s  :  %d",P[i].name,Count);	//display individual's name and no. of descendents
	}
}


/**********************************functions to find the great grandchildren of each individual*****************************************/ 

void fun(int count, int pos, int gpos)	//pos is the current node and gpos is the index of the great grandparent
{
	int i;

	count--;
	if(count==1)	//if great grandchild
	{		
		printf("\t\t%s\n",P[pos].name);		//print out name of great grandchild of node at position gpos 
		g[gpos]++;		//incrementing the no. of great grandchildren of node at gpos
	}
	else if(cno[pos]!=0)	//not great grandchildren but further children of node at pos exists
		for(i=0;i<cno[pos];i++)	//iterative loop through the children of node at pos to check for great grandchildren of node at gpos
			fun(count,c[pos][i],gpos);
}

void great_grandkids()
{
	int i,j,k;
	for(i=0;i<N;i++)		//initislising no. of great grandchildren to 0
		g[i]=0;
	printf("\n\nEverybody's great grandkids : \n");
	printf("\nAncestor\tGreat-grandkids\n");
	printf("------------------------------------------");

	for(i=0;i<N;i++)
	{
		printf("\n\n%s",P[i].name);		//print the name of the node
		for(k=0;k<cno[i];k++)	//iterating through all the children of node i
			fun(4,c[i][k],i);	//recursive function to find the no. of great grandchildren of node at i
		if(g[i]==0)			//if there are no great grandchildren
			printf("\n\t\t-----");
	}

	printf("\nNo. of great grandkids for each individual : ");
	for(i=0;i<N;i++)
		printf(" %s : %d   ,",P[i].name,g[i]);	
}



/**********************************functions to find the individuals who lived to see their great grandchildren*****************************************/ 

void fun1(int count, int pos, int age, int gpos)	//pos is the current node and gpos is the index of the great grandparent and age holds the cumulative sum along the edges considered
{
	int i;

	count--;	//decrementing counter (started from 4)
	
	if(count==1)	//if great grandchild
	{
		if(age<=atoi(P[gpos].death_age))	//if great grandparent was alive when great grandchild was born. 
		{
			printf("\t\t%s\n",P[pos].name);	//print out name of great grandchild
			g[gpos]++;	//increment the no. of great grandchildren
		}
	}
	else if(cno[pos]!=0)	//if not great grandchild and further children of node at pos exists 
		for(i=0;i<cno[pos];i++)
			fun1(count,c[pos][i],age+edge[pos][i],gpos);	//recursive call to find the great grandchildren
}

void see_great_grandkids()
{
	int i,j,count1=0,k;
	
	for(i=0;i<N;i++)
		g[i]=0;
	printf("\n\nAncestor\tGreat-grandkids seen");
	printf("\n------------------------------------------");
	for(i=0;i<N;i++)
	{
		printf("\n\n%s",P[i].name);		//print out the name of considered node
		for(k=0;k<cno[i];k++)	//iterate through children of considered node
			fun1(4,c[i][k],edge[i][k],i);	//recursive function to find the great grandchildren and check if the great grandparent had lived to see them
		if(g[i]==0)		//if the great grandparent died before their birth or if they didn't have any children
			printf("\n\t\t-----");
	}
	for(i=0;i<N;i++)	//to calculate the no. of individuals who had lived to see their great grandchildren
		if(g[i]!=0)
		{
			count1++;	//increment counter if no .of great grandchildren seen is non-zero
		}
	printf("\n\n%d individuals lived to see their great grandkids : ",count1);
	for(i=0;i<N;i++)
		if(g[i]!=0)
			printf("    %s : %d   ",P[i].name,g[i]);	//printing out names of individuals who lived to see their great grandkids

		
}
 
int main() 
{

    FILE *fin;
    fin = fopen("input3.dat", "r");	//opening file using file pointer fin
    
    if(fin==NULL)
    {
    	printf("\nError opening input file!!");
    	return(-1);
    }   

    char *str1,*str2;	//to read in names of individuals
    int E,index_p,index_c,i;//E holds the read edge value, index_p and index_cholds the indices of parent and child whose names were read

    str1 = (char *)malloc(80*sizeof(char));	//allocate memory
    
	for(i=0;i<4;i++)		//just to ignore the first 4 lines in the input file
		fgets(str1,80,fin);
	
	for(i=0;i<N;i++)	//reading in the name anddeath age of each individual from the file
	{
		fscanf(fin,"%s",P[i].name);
		fscanf(fin,"%s",P[i].death_age);
		p[i][0]=-1;		//initialising paret indices as -1
		p[i][1]=-1;
		cno[i]=0;	//initialise no. of children as 0
	}

	str1 = (char *)malloc(15*sizeof(char));	//allocate memory
	str2 = (char *)malloc(15*sizeof(char));

	while(!feof(fin))
	{
		fscanf(fin,"%s",str1);	//read in parent name
		fscanf(fin,"%s",str2);	//read in child name
		fscanf(fin,"%d",&E);	//read in edge value
		for(i=0;i<N;i++)
			if(strcmp(str1,P[i].name)==0)	//find index of parent
				index_p=i;
			else if(strcmp(str2,P[i].name)==0)	//find index of child
				index_c=i;
		c[index_p][cno[index_p]]=index_c;	//storing the index of the child for the parent
		edge[index_p][cno[index_p]]=E;		//storing the edge value to the child from the parent
		(cno[index_p])++;			//increment the no. of children
		if(p[index_c][0]==-1)	//if no parent till now
		{
			p[index_c][0] = index_p;	//parent index stored
		}
		else				//if one parent was already found before
			p[index_c][1] = index_p;				
	}

	find_ancestor();	//Q3a finding the ancestors
	no_of_decendants();	//Q3b finding the no .of descendents for each node
	great_grandkids();	//Q3c finding the great grandchildren
	see_great_grandkids();	//the individuals who lived to see their great grandchildren
    
	free(str1);	//deallocate the memory previously allocated
	free(str2);	//deallocate the memory previously allocated

    fclose(fin);

    return 0;

}