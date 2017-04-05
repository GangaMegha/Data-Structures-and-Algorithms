
#include <stdio.h>

#define ROW 10
#define COL 10
 
//to store matrix indices as cordinates
typedef struct Point
{
    int x;
    int y;
}Point;
 
typedef struct queueNode		// An Data Structure for queue used in BFS	
{
    Point pt;  // The cordinates of a matrix element
    int dist;  // element's distance of from start
}queueNode;
 

int isValid(int row, int col)		// check whether given indices (row, col) are valid or not.
{
    if((row >= 0) && (row < ROW) && (col >= 0) && (col < COL))	// return 1 if row number and column number is in range
    	return 1;
    return 0;
}
 
// These arrays are used to get row and column numbers of 4 neighbours of a given cell
int rowNum[] = {-1, 0, 0, 1};
int colNum[] = {0, -1, 1, 0};
 

/***************** function to find the shortest path between a given start coordinates and stop coordinates**************************/

int BFS(int mat[][COL], Point start, Point stop)
{
	int f=-1,b=-1;
    // check if the start and stop in the matrix have value 1
    if (!mat[start.x][start.y] || !mat[stop.x][stop.y])
        return -1;
 
    int visited[ROW][COL]={0};	//0 implies the position has not been visited
     
    // Mark the start as visited
    visited[start.x][start.y] = 1;
 
    // Create a queue for BFS
    queueNode q[300];;
     
    // distance of start coordinate is 0
    queueNode s = {start, 0};
 	b++;
 	if(f==-1)
 		f=0;
    q[b]= s;  // Enqueue start
    
    while (b!=-1)		// Do a BFS starting from 'start'
    {
        queueNode curr = q[f];
        Point pt = curr.pt;
 
        // If we have reached the stop coordinates
        if (pt.x == stop.x && pt.y == stop.y)
            return curr.dist;		//distance of stop from the start
 
        // Otherwise dequeue the front element in the queue and enqueue its adjacent elements
        f++;
 
        for (int i = 0; i < 4; i++)
        {
            int row = pt.x + rowNum[i];
            int col = pt.y + colNum[i];
             
            // if adjacent element is valid, has path and not visited yet, enqueue it.
            if (isValid(row, col) && mat[row][col] && !visited[row][col])
            {
                // mark element as visited and enqueue it
                visited[row][col] = 1;
                queueNode Adjcell = { {row, col},
                                      curr.dist + 1 };
                b++;
                q[b] = Adjcell;
            }
        }
    }
 
    //return -1 if stop cannot be reached
    return -1;
}
 

int main()
{
    int mat[ROW][COL] =
    {
        {1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,1,0,0,0,1},
        {1,0,1,0,1,1,1,1,0,0},
        {1,1,1,0,1,0,1,1,0,1},
        {1,0,0,0,1,0,0,0,0,1},
        {1,0,1,1,1,0,1,1,0,1},
        {1,0,1,0,1,0,1,1,1,1},
        {1,0,1,0,1,1,1,1,0,1},
        {1,0,1,0,0,0,1,0,0,1},
        {1,1,1,1,1,1,1,1,1,1}
    };
 
    Point start = {0, 0};
    Point stop = {3, 9};
 
    int cost = BFS(mat, start, stop);
 
    if (cost != -1)
        printf("Shortest Path is %d", cost) ;
    else
        printf("Shortest Path doesn't exist");
 
    return 0;
}