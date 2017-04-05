/*******************************************************Question 3 - DP implementation*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define N 50	//no. of individuals
#define no_of_children_max 10	//maximum no. of children per individual

typedef struct //node
{
	char name[15];
	char death_age[3];
}Person;

Person P[N];	//stores required information about each individual

int c[N][no_of_children_max],edge[N][no_of_children_max]; //c[i][j] stores the index of jth child and edge stores the age of i when jth child was born
int p[N][2],cno[N];	// p[i][] stores the two parents(if there) of node i , cno[i] hold the no. of children of node i
int lev[10];	//assume maximum no. of ancestors is 10, lev[i] holds the heirarchial level of each node in the graph
int max=-1 , dp[N][2]={0};	//max is a utility variable used to find one of the oldest ancestors
						//dp[i][j] stores the heirarchial level of node i at j=1 and no. of descendants at j=0

int g2[N][N]={0};	//g2[i][0] holds the no. of great grandchildren of i and g2[i][] holds the indices
int g3[N][N]={0};	//g3[i][0] holds the no. of great grandchildren seen before death by node at i and g3[i][] holds the indices

/**********************************function to find the ancestors*****************************************/ 

void find_ancestor()		//3a answer. To find the ancestors
{
	int i,j=0;
	printf("\nThe ancestors are : ");
	for(i=0;i<N;i++)
	{
		if(i<10)
			lev[i]=-1;	//initialise lev[] to -1
		
		if(p[i][0]==-1 && p[i][1]==-1)	//if they have no parents at all, then they are ancestors.
		{
			printf("\n\t%s",P[i].name);		//print the name of the ancestors
			if(cno[i]==0)					//if isolated node
				printf("  (but no descendants)");
			
			lev[j]=i;	//store the indices of ancestors in lev[]
			j++;	//increment j as one more ancestor index has been stored
			
		}
	}
}

/**********************************functions to find the no. of descendants of each individual*****************************************/ 

void find2(int pos, int cnt)		//recursive function for finding the lowest descendant count
{
	int i;
	if(cno[pos]!=0)		// if the node at pos has children
	{
		cnt++;		//level counted
		for(i=0;i<cno[pos];i++)
		{
			find2(c[pos][i],cnt);	//recursive function call to calculate the lowest descendents level
			if(cnt>max)
				max=cnt;	//to get the maximum depth of a descendant from the oldest ancestor inorder to decide the heirarchial levels
		}
	}
}
void assign(int pos,int cnt)	//recursive function to assign levels to the descendant nodes of node at pos
{
	int i;
	
	if(cno[pos]!=0)		// if the node at pos has children
	{
		cnt--;		//level counted
		for(i=0;i<cno[pos];i++)
		{
			dp[c[pos][i]][1]=cnt;	//assign level to the child node (oldest ancestor at the heighest level)
			assign(c[pos][i],cnt);	//recursive function call to assign levels to the children nodes
		}
	}

}

void assign_p(int pos,int cnt)	//function to assign levels to parent nodes that have children with nodes that are part of the biggest tree : connected trees and hence referred to as graphs
{
	int i;
	cnt++;	//increment level as compared to the child
	
	if(p[pos][0]!=-1 && dp[p[pos][0]][1]==0 )	//for parent 1, if the level has not been yet assigned as it wasn't part of the largest tree
	{
		dp[p[pos][0]][1]=cnt;	//assign level to the parent node
		assign_p(p[pos][0],cnt);	//recursive function call to assign levels to further parent nodes
	}
	if(p[pos][1]!=-1 && dp[p[pos][1]][1]==0)	//for parent 2, if the level has not been yet assigned as it wasn't part of the largest tree
	{
		dp[p[pos][1]][1]=cnt;	//assign level to the parent node
		assign_p(p[pos][1],cnt);	//recursive function call to assign levels to further parent nodes
	}
	
}
int max_level()	
{
	int i=0,index=-1,level_max=-1;	//level_max holds the deepest depth from the ancestor at index 
	
	while(lev[i]!=-1)	//loop to calculate the oldest ancestor level from all the found ancestors
	{	
		find2(lev[i],0);	
		if(max>level_max)	//max is a global variable, check if the depth= max found is greater than the one previously found
		{
			level_max=max;	//new maximum depth is stored in level_max
			index=lev[i];	//index of the oldest ancestor found as of now is stored in index
		}
		i++;	//increment i to access the next ancestor
	}
	dp[index][1]=level_max+1;	//assign the highest level no. to one of the oldest ancestors (in case more than one ancestor holds the same heighest heirarchial level)
	assign(index,level_max+1);	//assign levels to all the descendants of the oldest ancestor

	for(i=0;i<N;i++)	//loop to assign levels upto the rest of the other ancestors such that each ancestor receives their heirarchial location with respact to one another
	{
		if(dp[i][1]!=0)	//node that has been assigned a level
		{
			assign_p(i,dp[i][1]);	//if parents have not been assigned levels, then does a recursive loop to do so. This is in case of common children between people belonging to different geneology
		}	

	}

	i=0;
	while(lev[i]!=-1)	//loop to assign levels to the rest of the ancestors and their descendants (connected or disconnected from the main graph)
	{
		if(lev[i]!=index && dp[lev[i]][1]!=0)	//if the ancestor tree is connected with the other ancestor trees via some marriage and common descendant
			assign(lev[i],dp[lev[i]][1]);		//assign heirarchial levels to all descendants of such trees
		else if(lev[i]!=index && dp[lev[i]][1]==0)	//if the ancestor tree is disconnected with the other ancestor trees
		{
			dp[lev[i]][1]=level_max+1;		//assign  the maximum level to ancestor
			assign(lev[i],dp[lev[i]][1]);	//assign descending levels to the descenants of the ancestor
		}

		i++;	//finding the rest of the ancestors and doing the same
	}
	return(level_max+1);	//return the max level
}

void no_of_decendants2()		//function used to calculate the no. of descendents of each node.
{
	int m = max_level();	//find the maximum level and assign heirarchial levels to each node
	int i,j;
	for(j=1;j<=m;j++)	//levelwise adding the no. of descendants from bottom-up approach (note: ignoring level 1. ie, the bottom-most level as their descendants=0)
	{
		for(i=0;i<N;i++)	//traversing the array to find the required level j
		{
			if(dp[i][1]==j)	//if the level is found, increment the no. of descendants of the parents
			{
				if(p[i][0]!=-1)	//for parent 1
				{
					dp[p[i][0]][0]+=dp[i][0]+1;	//increment the no. of descendants = descendants of current node + the current node
				}
				if(p[i][1]!=-1)	//for parent 2
				{
					dp[p[i][1]][0]+=dp[i][0]+1;	//increment the no. of descendants = descendants of current node + the current node
				}
			}
		}
	}
	
	printf("\n\nPerson  No. of descendants");	//print out the no. of descendants for each individual
	printf("\n------------------------------------");
	for(i=0;i<N;i++)
	{
		printf("\n%s  :  %d",P[i].name,dp[i][0]);	//display individual's name and no. of descendents
	}
}


/**********************************functions to find the great grandchildren of each individual*****************************************/ 

void fun2(int count, int pos0,int pos1, int pos2, int pos3, int ed0, int ed1, int ed2)	//count is a dummy variable used to identify that its a great grandchild
{	//po0 to pos3 holds 4 nodes starting from great grandparent to great grandchild and ed0 to ed2 holds the edge values.
	int i,gpos;
	count--;
	if(count==0)	//if great grandchild
	{		
		gpos=pos0;	//gpos = position of great grandparent
		g2[gpos][0]++;		//incrementing the no. of great grandchildren of node at gpos
		g2[gpos][g2[gpos][0]]=pos3;	//store the great-grand child index in the next storage place after the previous one if any.
		if(ed0+ed1+ed2<=atoi(P[gpos].death_age))	//checks if the great-grandparent lived to see the great grandchild under consideration
		{
			g3[gpos][0]++;	//increment the no. of great-grandchildren seen
			g3[gpos][g3[gpos][0]]=pos3;	//store the index of the great grand child seen
		}	
		if(cno[pos3]!=0)	//further children of great grandchild of node at gpos exists 
		{
			pos0=pos1;	//shift the positions of the heirarchial descendant nodes by eliminating the great grandparent
			pos1=pos2;
			pos2=pos3;

			ed0=ed1;	//shift the edge values under consideration
			ed1=ed2;

			for(i=0;i<cno[pos2];i++)	//iterative loop through the children of node at pos2(great grand child) to check for great grandchildren of node at pos0
			{
				pos3=c[pos2][i];	//pos3 stores the new node ie, child of pos2
				ed2=edge[pos2][i];	//ed3 holds the edge value between pos2 and pos3
				fun2(count+1,pos0,pos1,pos2,pos3,ed0,ed1,ed2);	//recursive call to find further greatgrandchildren to the nodes
			}	
		}	
	}
}

void fun3(int count,int pos[],int ed[], int gpos, int n)	//function to find the great grandchildren of the ancestors
{
	int i;
	count--;	//decrement count and execute the current function till it reaches a great grand child
	if(cno[pos[n]]!=0 && count!=0)	//not great grandchildren but further children of node at pos[n] exists
	{
		n++;	//increment the depth
		for(i=0;i<cno[pos[n-1]];i++)	//iterative loop through the children of node at pos[n-1] to check for great grandchildren of node at gpos
		{
			pos[n]=c[pos[n-1]][i];	//add the current child to the chain of nodes from the ancestor to great grandchild
			ed[n-1]=edge[pos[n-1]][i];	//add the edge between the current node and child considered
			fun3(count,pos,ed,gpos,n);	//recursive call to function 
		}
	}	
	else if(count==0)	//if great grandchild found
		fun2(count+1,pos[0],pos[1],pos[2],pos[3],ed[0],ed[1],ed[2]);	//call fun2 to find the great grand children of all descendants of gpos 
}

void great_grandkids2()	//function to find the great grandkids and also the great grandkids individuals lived to see
{
	int i,j,cnt=0;
	int pos[4]={-1,-1,-1,-1};	//array used to hold a chain of great grandparent to great grandchild
	int ed[3]={-1,-1,-1};		//array to hold the edge values to later calculate if the individual lived to see the great grand child
	
	printf("\n\nGreat grandkids of the individuals (if any) : \n");
	printf("\nIndividual\tGreat-grandkids\n");
	printf("------------------------------------------");

	i=0;
	while(lev[i]!=-1)	//iterate through the ancestors found
	{
		pos[0]=lev[i];	//pos[0] holds the great grand parent index
		fun3(4,pos,ed,lev[i],0);	//recursive function to find the great grandchildren of node at i and also the ones node i lived to see
		i++;					//increment to next ancestor
	}
	
	for(i=0;i<N;i++)	//to print the great grandparent name and all of his/her great grandchildren names if any
	{
		if(g2[i][0]!=0)	//print out only the individuals who have great grandchildren 
		{
			printf("\n%s : %d :",P[i].name,g2[i][0]);	//print the name of the great grand parent
			for(j=1;j<=g2[i][0];j++)
				printf("    %s",P[g2[i][j]].name);	//print out the names of the grandkids
		}
	}
/**********************************functions to find the individuals who lived to see their great grandchildren*****************************************/ 

	printf("\n\nAncestor\tGreat-grandkids seen");
	printf("\n------------------------------------------");
	for(i=0;i<N;i++)	//to print the great grandparent name and all of his/her great grandchildren names (if any) that they lived to see
	{
		if(g3[i][0]!=0)	//print out only the individuals who have seen their great grandchildren 
		{
			cnt++;	//increment the count of great grandparents who lived to see their great grandchildren
			printf("\n%s : %d :",P[i].name,g3[i][0]);	//print the name of the great grand parent
			for(j=1;j<=g3[i][0];j++)
				printf("    %s",P[g3[i][j]].name);	//print out the names of the grandkids they have lived to see
		}
	}
	printf("\n\nTherefore, %d individuals lived to see their great grandchildren\n\n",cnt);
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
	
	for(i=0;i<N;i++)	//reading in the name and death age of each individual from the file
	{
		fscanf(fin,"%s",P[i].name);
		fscanf(fin,"%s",P[i].death_age);
		p[i][0]=-1;		//initialising paret indices as -1
		p[i][1]=-1;
		cno[i]=0;	//initialise no. of children as 0
	}

	str1 = (char *)malloc(15*sizeof(char));	//allocate memory
	str2 = (char *)malloc(15*sizeof(char));

	while(!feof(fin))	//reading in the relations
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

	find_ancestor();		//Q3a finding the ancestors
	no_of_decendants2();	//Q3b finding the no .of descendents for each node
	great_grandkids2();		//Q3c finding the great grandchildren and also the ones individuals lived to see
    
	free(str1);	//deallocate the memory previously allocated
	free(str2);

    fclose(fin);	//close the opened input file

    return 0;

}