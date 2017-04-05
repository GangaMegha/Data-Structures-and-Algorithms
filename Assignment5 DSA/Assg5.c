/*******************************Question 2*************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
//#include<graphics.h>

#define  N 50 //maximum allowed no. of vertices

typedef struct
{
    int x;
    int y;
}Coordinates;

int accept(Coordinates P[N],float D[N][N],int *n)
{
    FILE *fin;
	fin = fopen("Input.dat","r");	//input file

	if(fin==NULL)
	{
		printf("\nError opening input file!!");
		return(1);
	}
    fscanf(fin,"%d",&(*n)); //accept the no.of verties
    int i,j;
    for(i=0;i<*n;i++) 
        fscanf(fin,"%d %d",&P[i].x,&P[i].y);		//accept the coordinate points of the vertices
        
    for(i=0;i<*n;i++)       //accept the distance matrix row-wise
        for(j=0;j<*n;j++)
            if(abs(i-j)==1 ||abs(i-j)==*n-1)
            	D[i][j]=0;
            else
            	D[i][j]=sqrt((P[i].x - P[j].x)*(P[i].x - P[j].x) + (P[i].y - P[j].y)*(P[i].y - P[j].y));	//computing the distance between the nodes
   /* printf("\nD :");                        
    for(i=0;i<*n;i++)       //accept the distance matrix row-wise
     {
        printf("\n");
        for(j=0;j<*n;j++)
            printf("\t%f",D[i][j]);
    }
*/
    return(0);
}

void compute_cost(float C[N][N],float D[N][N],int V[N][N], int *n)
{
    int i,j,k,x,y,p,flag;
    float smallest,temp;
    int m=*n;
    for(i=0;i<m;i++)
        for(j=0;j<4;j++)    //j=0 is redundant
        {
            C[i][j]=0;      //j<4 , C[i][j]=0 as given in the text
            
            if(j!=3)
                V[i][j]=0;
            else
                V[i][j]=(i+j-1)%m;
        }   
  
    for(j=4;j<=m;j++)
        for(i=m-1;i>=0;i--)    //computing lowest cost for j>=4 for all i
        {
            x=(i+1)%m;           
            y=(i+j-1)%m;
                        
            smallest=C[i][2]+C[x][j-1]+D[i][x]+D[x][y];
            V[i][j]=1;

            for(k=2;k<j-1;k++)
            {
                x=(i+k)%m;
                y=(i+j-1)%m;
                
                temp=C[i][k+1]+C[x][j-k]+D[i][x]+D[x][y];		//basis for the equation explained in Aho
                if(temp<smallest)
                 {
                    smallest=temp;
                    V[i][j]=k;                     
                 }  
            }

            C[i][j]=smallest;

    
       }
    /*   printf("\nV :\n"); 		//display k values 
   for(i=0;i<m;i++)
    {
        printf("\n");
        for(j=m;j>=4;j--)
            printf("\tV%d%d = %d",i,j,V[i][j]);          
    }*/

}

int table(float C[N][N], int *n)		//to print the cost table to the output file
{
    FILE *fout;
	fout = fopen("Output.dat","w");	//output file

	if(fout==NULL)
	{
		printf("\nError opening output file!!");
		return(1);
	}

    int i,j,m=*n;
    for(i=0;i<m;i++)        //print the table to the output file
    {
        fprintf(fout,"\n");
        for(j=m;j>=4;j--)
            fprintf(fout,"\tC%d%d = %f",i,j,C[i][j]);          
    }
    for(i=0;i<m;i++)			//print it on standard output
    {
        printf("\n");
        for(j=m;j>=4;j--)
            printf("\tC%d%d = %f",i,j,C[i][j]);          
    }
    return(0);
}

int count=-1;
int find_chords(int V[N][N], int chords[N][2], int i, int j, int m)		//find the chords from the V matrix using k values and backtracking
{
	int k;
	int x,y;
	if(j<4)
		return 0;
	else
	{
		k=V[i][j];			
		x=(i+k)%m;
		y=(i+j-1)%m;
		if(k!=1)
		{
			count++;
			chords[count][0]=i;		// explained in text
			chords[count][1]=x;
		}
		if(k!=j-2)
		{
			count++;
			chords[count][0]=x;
			chords[count][1]=y;
		}
		find_chords(V,chords,i,k+1,m);
		find_chords(V,chords,x,j-k,m);
		return 1;
	}
}

void display_chords(int chords[N][2], Coordinates P[N])	//display the pionts to be joined as chords
{
	int i,j;
	for(i=0;i<=count;i++)
			printf("\nV%dV%d : (%d,%d) & (%d,%d)",chords[i][0],chords[i][1],P[chords[i][0]].x,P[chords[i][0]].y,P[chords[i][1]].x,P[chords[i][1]].y);
}

int main()
{    
    Coordinates P[N];   //stores the coordinates of the vertices of the polygon
    float D[N][N]={0};   //stores the distance between the vertices
                            //Note : distance between adjacent vertices are defined to be zero as given in the text
    
    float C[N][N];   //stores the minimum cost for each sub polygon
                        //Note : C[i][j] = 0 if j<4 asgiven in the text.

    int V[N][N]={0};    //stores the k values at each i,j

    int chords[N][2];	//stores the vertices that are to be joined as chords

    int n=0;  //stores the no.of vertices

    if(accept(P,D,&n)==1)   //input and calculate distance
        return 1;

    compute_cost(C,D,V,&n); //computes the cost C[i][j] and stores the mid vertex in V

    if(table(C,&n)==1)        //prints out the cost table
        return 1;

    find_chords(V,chords,0,n,n);	//find the chords from the V matrix using k values and backtracking
    display_chords(chords,P);		//display the pionts to be joined as chords
    
    return 0;
}
