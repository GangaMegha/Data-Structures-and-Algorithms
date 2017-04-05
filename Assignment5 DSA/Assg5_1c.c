
#include <stdio.h>

#define R 10
#define C 10
 


int isValid(int row, int col)		// check whether given indices (row, col) are valid or not.
{
    if((row >= 0) && (row < R) && (col >= 0) && (col < C))	// return 1 if row number and column number is in range
    	return 1;
    return 0;
}

void findpath(int tc[R][C],int n1, int n2, int m1, int m2)		//backtracking from the destination to the beginning
{
	
	int flag,temp2;			//temp2 holds the value of the cell to the alternate path
    int path[R][C][10]={0};     //1 denotes the shortest path

    int i,j,l,m=0;		//m iterates through the different paths, i&j holds the row and column index of a cell
	
    int x,y;		//x&y holds the row and column index of a cell

    int rowNum[] = {-1, 0, 0, 1};       //for accessing the adjascent elements
    int colNum[] = {0, -1, 1, 0};

	do    
	{
	    i=m1;		//reinitialising i and j
	    j=m2;
	    path[i][j][m]=1; //marking stop
	    flag=0;
	    temp2=-1;

	    while(i!=0 || j!=0)
	    {
	        int smallest=1000;
	        
	        for (l = 0; l < 4; l++)
	        {
	            int row = i + rowNum[l];
	            int col = j + colNum[l];

	            // if adjacent element is valid
	            if (isValid(row, col))
	                if(smallest > tc[row][col])		//finding the smallest cost cell around the given i&j
	                {
	                    smallest = tc[row][col];
	                    x = row;
	                    y = col;          
	                }
	                else if(smallest==tc[row][col])		//if there are multiple smallest cost cells around the given cell
		            {
		            	temp2=smallest;
		            }
	        }
	        if(temp2==smallest && i==m1 && j==m2)		//if there are multiple minimum cost, 
	        {
	        	flag=1;
	        	tc[x][y]=5000;		//then block the current beginning so that it wont be accessed in the next iteration
	        }

	        i=x;	//moving to the nxt cell
	        j=y;
	        path[i][j][m]=1;   //marking the path travelled
	        	

	    }

	    printf("\nShortest path %d:\n",m+1);

	    for(i=0;i<n1;i++)       //print shortest path
	     {
	        printf("\n");
	        for(j=0;j<n2;j++)
	        {
	        	 printf("\t%d",path[i][j][m]);
	        }   

	    }
	    
	    m++;		//incrementing the no. of different paths

	}while(flag==1);
}
 
/*function that returns minimum of 4 integers */
int min(int x, int y, int z, int w)
{
	
    if (x <= y && x<=z && x<=w)
      	return x;
    else if(y<=x && y<=z && y<=w)
       return y;
   	else if(z<=x && z<=y && z<=w)
   		return z;
   	else if(w<=x && w<=y && w<=z)
   		return w;
   	//return 0;
}

int minCost(int Matrix[R][C], int m, int n, int m1, int m2)
{
    int i, j,k,l;
    int flag,flag123;

    int tc[R][C];  
    int mat2[R][C];

    for(i=0;i<m;i++)
        for(j=0;j<n;j++)
            tc[i][j]=1000;      //initialise to a large value initially to get minimum cost during computation
    
    tc[0][0] = Matrix[0][0];        //initialise the first element
    
    for(l=0;l<10;l++)       //to find the proper cost matrix
    {
        flag123=0;
        for (i = 1; i <=(m-1)*2; i++)
        {
            if(i<=m-1)  //matrix is traversed diagonally. Hence, the conditions when the diagonal exceeds the main diagonal
            {
                flag=i;
                k=i;
                j=0;
            }
            else
            {
                flag=n-1;
                k=m-1;
                j=i-(n-1);
            }
            
            while(j<=flag)   //cost at a cell= value in that cell + minimum of the cost of the adjascent cells. In certain cases, adjascent cells are not present, hence the conditions
            {
                if(j-1>=0 && j+1<=n-1 && k-1>=0 && k+1<=m-1)
                    tc[k][j] = min(tc[k-1][j],tc[k][j-1],tc[k+1][j],tc[k][j+1]) + Matrix[k][j];
                else if(j+1>n-1)
                {
                    if(k-1<0)
                        tc[k][j] = min(9000,tc[k][j-1],tc[k+1][j],9000) + Matrix[k][j];
                    else if(k+1>n-1)
                        tc[k][j] = min(tc[k-1][j],tc[k][j-1],9000,9000) + Matrix[k][j];
                    else
                        tc[k][j] = min(tc[k-1][j],tc[k][j-1],tc[k+1][j],9000) + Matrix[k][j];
                }
                else if(j-1<0)
                {
                    if(k+1>m-1)
                        tc[k][j] = min(tc[k-1][j],9000,9000,tc[k][j+1]) + Matrix[k][j];
                    else if(k-1<0)
                        tc[k][j] = min(9000,9000,tc[k+1][j],tc[k][j+1]) + Matrix[k][j];
                    else
                        tc[k][j] = min(tc[k-1][j],9000,tc[k+1][j],tc[k][j+1]) + Matrix[k][j];
                }
                else if(k-1<0 && j+1<=n-1 && j-1>=0)
                    tc[k][j] = min(9000,tc[k][j-1],tc[k+1][j],tc[k][j+1]) + Matrix[k][j];
                else if(k+1>m-1 && j-1>=0 && j+1<=n-1)
                    tc[k][j] = min(tc[k-1][j],tc[k][j-1],9000,tc[k][j+1]) + Matrix[k][j];


                k--;
                j++;
            }

        }
         for(i=0;i<m;i++)                    //if the matrix remains the same then break out of the iterations for computing cost
            for(j=0;j<n;j++)
                if(mat2[i][j]!=tc[i][j])
                {
                    flag123=1;
                    break;
                }
        if(flag123==0)      //if the cost matrix remains the same, break out of the loop to find the optimum cost matrix, ie, the l loop
            break;

        for(i=0;i<m;i++)                    
            for(j=0;j<n;j++)
                mat2[i][j]=tc[i][j];
    }

    printf("\nDP Matrix : \n");
    for(i=0;i<10;i++)        //Print out the cost matrix
     {
        printf("\n");
        for(j=0;j<10;j++)
            printf("\t%d",tc[i][j]);
    }

    findpath(tc,m,n,m1,m2);
    return tc[m1][m2];
}



int main()
{
    int i,j,m1, m2,m,n;
    /*     FILE *fin;
    fin = fopen("Input.dat","r");   //input file

    if(fin==NULL)
    {
        printf("\nError opening input file!!");
        return(1);
    }*/
  
    int Matrix[R][C]={  {0,1,1,1,1,1,1,1,1,1},				//assumed that 10 units of time to attemp to go through a wall + 1 unit of time to come out of the wall=11
                        {1,11,11,11,11,1,11,11,11,1},
                        {1,11,1,11,1,1,1,1,11,11},
                        {1,1,1,11,1,11,1,1,11,0},
                        {1,11,11,11,1,11,11,11,11,1},
                        {1,11,1,1,1,11,1,1,11,1},
                        {1,11,1,11,1,11,1,1,1,1},
                        {1,11,1,11,1,1,1,1,11,1},
                        {1,11,1,11,11,11,1,11,11,1},
                        {1,1,1,1,1,1,1,1,1,1}           };
    /*for(i=0;i<m;i++)       //accept the distance matrix row-wise
        for(j=0;j<n;j++)
        {
            fscanf(fin,"%c",&c);
            printf("%c",c );
            if(c=='0')
                cost[i][j] = 1;
            else if(c=='x')
                cost[i][j] = 10;
            else if(c=='S')
                cost[i][j] = 0;
            else if(c=='E')
            {       
                    m1 = i;
                    m2 = j;
                    cost[i][j] = 0;
            }
        }*/
    printf("\nInitial Matrix :");                        
    for(i=0;i<10;i++)       //print out the initial matrix or maze
     {
        printf("\n");
        for(j=0;j<10;j++)
            printf("\t%d",Matrix[i][j]);
    }
   printf("\n\n Minimum cost :%d ", minCost(Matrix, 10, 10, 3, 9));
  
   return 0;
}