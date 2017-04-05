#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define N 10000	//Maximum size of the tree and heap

typedef struct 
{
	int id;	//packet id
	int t0;	//Arrival time of packet
	int L;	//size of packet
	char *contents;	//pointer to contents of packet
}PACKET;
	
PACKET Tree[N]={0};
//PACKET Queue[N];	// to store the lambda packets that arrive
PACKET Heap[N]={0};

int n=0;	//current no.of elements in the tree
int n_h=-1;	//index of last element in heap

float nextTime(float rateParameter)
{ 
	return -logf(1.0f - (float) random() / (RAND_MAX + 1.0)) / rateParameter;
}

/***********************Function to insert arrived packets into the tree*************************************/

int check(int temp,int i)	//to find the index where the new packet is to be added
{
	int j;
	if(temp<=(Tree[i].L))
	{
		j=2*i+1;			//left child
		if((Tree[j].L)==0)
		{
			return j;
			if(j>n)
				n=j;
		}
		else
			j=check(temp,j);		
	}
	else
	{
		j=2*i+2;		//rightchild
		if((Tree[j].L)==0)
		{
			return j;
			if(j>n)
				n=j;
		}
		else
			j=check(temp,j);	
	}
	return j;
}

void add(int index, PACKET temp)		//function to add a new packet
{
	(Tree[index].id)=temp.id;
	(Tree[index].t0)=temp.t0;
	(Tree[index].L)=temp.L;
	(Tree[index].contents)=temp.contents;
}

int make_binary_tree(PACKET temp)	//function to insert new elements into the tree
{
	int i,index;
	if(Tree[0].L==0)	//if the tree is empty
	{
		add(0,temp);
		n=1;
		return(0);
	}
	else
	{
		index=check(temp.L,0);		//location in tree to be inserted.
		if(index>=N)
			return(-1);
		add(index,temp);		// adding new element
		if(n<index+1)
			n=index+1;
		return(0);
		//printf("\nReached  ");
	}
	//tree full implies drop the rest of the packets
	
}
/******************************************************************************************************************/

/***************************Function to delete smallest packets from the tree as they go out***********************/
void move(int i, int j)		//to move the contents from one index to another
{
	(Tree[i].id)=(Tree[j].id);
	(Tree[i].t0)=(Tree[j].t0);
	(Tree[i].L)=(Tree[j].L);
	(Tree[i].contents)=(Tree[j].contents);
}

void rearrange(int i, int j)	//function to shift the rightmost child( and its children and so on ) to parent position
{
	static int temp=0;
	
	if((Tree[2*j+1].L)!=0)
	{
		move(2*i+1,2*j+1);
		(Tree[2*j+1].L)=0;
		free((Tree[2*j+1].contents));
		(Tree[2*j+1].contents)=NULL;
		temp=2*i+1;	//during the final recursion, n will hold the index of the last element in the tree.
		if(temp>n)
			n=temp;
		rearrange(2*i+1,2*j+1);
		
	}
	if((Tree[2*j+2].L)!=0)
	{
		move(2*i+2,2*j+2);
		(Tree[2*j+2].L)=0;
		free((Tree[2*j+2].contents));
		(Tree[2*j+2].contents)=NULL;
		
		temp=2*i+2;	//during the final recursion, n will hold the index of the last element in the tree.
		if(temp>n)
			n=temp;
		rearrange(2*i+2,2*j+2);
	}
}

int choose_smallest_packet(int t0, int mu)		//to choose the smallest packet for senting out
{
	int i;
	for(i=0;i<n;i=2*i+1)
		if((Tree[2*i+1].L)==0 && (Tree[i].L)!=0)
		{
			return(i);
		}
	return(-1);
}

void sent_packets_out(int t0, int mu, int L_avg)		//function to sent out packets during the time interval t0
{
	int i,j,index;
	static float extra=0;

	int size=3*sizeof(int)+(L_avg+1)*sizeof(char);
	int no_of_packets = (mu*t0+extra)/size;		//no. of packets fully sent out
	extra = (int)(mu*t0+extra)%size;	//part of a packet sent out(bytes), ex: a packet might not get fully sent out during the time interval
	for(i=0;i<no_of_packets;i++)
	{	
		index=choose_smallest_packet(t0,mu);		//index holds the index value of smallest packet
		if(index==(-1))
			break;
		if((Tree[2*index+1].L)==0 && (Tree[2*index+2].L)!=0)	//if the leftchild of parent(index) is missing and index is the smallest 
		{
			move(index,2*index+2);
			n=0;			//
			rearrange(index,2*index+2);	//move right child(2*index+2) to index along with all its children through rearrange
			n++;
		}
		else 			//if both children of index are missing
		{
			(Tree[index].L)=0;
			free((Tree[index].contents));
			(Tree[index].contents)=NULL;
		}
		
	}
}

/****************************************************************************************************************************/

/***********************Function to insert and delete packets in the heap*************************************/
void swap(int i, int j)
{
	PACKET temp;
	(temp.id)=(Heap[i].id);
	(temp.t0)=(Heap[i].t0);
	(temp.L)=(Heap[i].L);
	(temp.contents)=(Heap[i].contents);

	(Heap[i].id)=(Heap[j].id);
	(Heap[i].t0)=(Heap[j].t0);
	(Heap[i].L)=(Heap[j].L);
	(Heap[i].contents)=(Heap[j].contents);

	Heap[j].id=temp.id;
	Heap[j].t0=temp.t0;
	Heap[j].L=temp.L;
	Heap[j].contents=temp.contents;
}

void heapify(int n_h, int i)		//implements recursive swapping from top to bottom
{
	int l,r,smallest;
	l=2*i+1;
	r=2*i+2;
	smallest=i;

	if(l<n_h && Heap[smallest].L>=Heap[l].L)
		smallest=l;
	if(r<n_h && Heap[smallest].L>=Heap[r].L)
		smallest=r;

	if (smallest!=i)
	{
		swap(i,smallest);
		heapify(n_h,smallest);
	}
}

int make_heap(PACKET temp)		//to form the heap and to insert into the heap 
{
	int smallest,i;
	if(n_h==N-1)
		return (-1);
	n_h++;
	Heap[n_h]=temp;
	smallest=(n_h-1)/2;
	while(Heap[smallest].L >= Heap[n_h].L && n_h>0)
	{
		swap(smallest,n_h);
		n_h=smallest;
		smallest=(n_h-1)/2;
	}
	
	return 0;
}

int pop_heap(int t0, int mu, int L_avg)			//removing packets from heap
{
	int i,j;
	static float extra_h=0;

	int size=3*sizeof(int)+(L_avg+1)*sizeof(char);
	int no_of_packets = (mu*t0+extra_h)/size;		//no. of packets fully sent out
	extra_h = (int)(mu*t0+extra_h)%size;	//part of a packet sent out(bytes), ex: a packet might not get fully sent out during the time interval
	for(i=0;i<no_of_packets;i++)
	{
		swap(0,n_h);
		heapify(n_h-1,0);

		(Heap[n_h].L)=0;
		free((Heap[n_h].contents));
		(Heap[n_h].contents)=NULL;
		
		(n_h)--;
		if(n_h<0)
			return (-1);
	}
	return(0);
}

/****************************************************************************************************************************/

int main()
{
	int i,j,k;
	time_t t;
	int delta_t;	//time interval between the packets arrival
	int avg=0,calc_avg=0;	//average rate of input data

	int L1=80,L2=1550;	//Lower and Upper bound on packet length
	int lambda,mu;	//Time denotes the time of arrival of packet
	float Time=0;					//lambda is the no. of packets arriving per second.
						//mu denotes the no. of bytes sent out per second.
	float prog;					//prog is the no. of times the program runs...ie, the no. of inputs
	
	int L_av,count=0,L_avg=(L2-L1)/2;
	float av=0;		//calc_avg=av/Time

	int flag_binary, flag_heap;		//to check if incoming packets are to be dropped
	PACKET temp;
	
	double t_b=0,t_h=0;	//holds the time elapsed during insertion and deletion for binary tree and heap for the entire program

	printf("\n Enter time for which program is to be run (in seconds): ");
	scanf("%f",&prog);
	printf("\n Enter the no. of packets arriving per second , lambda : ");
	scanf("%d",&lambda);
	printf("\n Enter the no. of bytes sent out per second , mu : ");
	scanf("%d",&mu);
		
	static float fraction=0;

//Loop providing the lamda packets and senting out the smallest packets as time goes by.
	while(Time<prog)  
	{
		srand((unsigned) time(&t));	//providing a seed to rand
		float delta_t=nextTime(lambda);
		count++;
		Time+=delta_t;
		//printf("Time : %f\n",Time);
		int Lamda = lambda*delta_t+fraction;	//Lamda holds the packets arriving during time delta_t
		fraction = (lambda*delta_t+fraction)-Lamda;
		L_av=0;

		flag_binary=0;
		flag_heap=0;

		clock_t start_b1 = clock();
		sent_packets_out(delta_t,mu,L_avg);		// sent out packets during the time interval are removed form the tree
		clock_t stop_b1 = clock();

		clock_t start_h1 = clock();
		pop_heap(delta_t,mu,L_avg);		// sent out packets during the time interval are removed form the heap
		clock_t stop_h1 = clock();
		//printf("\nReached\n");

		for (i=0;i<Lamda;i++)
		{
			temp.L = (rand() % (L2 + 1 - L1)) + L1;	//random allocation of length
			temp.t0=Time;	//time of packet arrival
			temp.contents = (char *) malloc((temp.L+1)*sizeof(char));
			for(j=0;j<temp.L;j++)
				*(temp.contents+j)=(char)('a'+j);	//creating content simply, can be accepted from user if we wish to do so too.
			L_av+=temp.L;
			//printf("L_avg :%d\n",L_av);
			
			clock_t temp1 = clock();
			if(flag_binary==0)		//ignore or drop the incoming packets if flag_binary=-1 since the first packet wasnt inserted
			{
				flag_binary=make_binary_tree(temp);	//function to insert into the tree
			}
			clock_t temp2 = clock();
			if(flag_heap==0)		//ignore or drop the incoming packets if flag_binary=-1 since the first packet wasnt inserted
			{
				flag_heap=make_heap(temp);		//Inserting the new packet into heap
			}
			clock_t temp3 = clock();

			t_b+=(double)(temp2-temp1);
			t_h+=(double)(temp3-temp2);

			free(temp.contents);
			if(flag_heap!=0 && flag_binary!=0)	//no further elements are inserted to the heap nor tree for that time interval
				break;
		}
		//L_avg+=L_av/Lamda;		
		t_b+=(double)(stop_b1-start_b1);
		t_h+=(double)(stop_h1-start_h1);

		av+=(Lamda*3*sizeof(int)+(L_av+Lamda)*sizeof(char));
	}
	//L_avg/=count;
	av=av+ fraction*(3*sizeof(int)+(L_av+1)*sizeof(char));

	//avg=lambda*(3*sizeof(int)+(L_avg+1)*sizeof(char));	//average rate of input in bytes per second from program
	calc_avg=lambda*(3*sizeof(int)+(L_avg+1)*sizeof(char));	//manually calculated
	avg=(int)(av/Time);	//average rate of input in bytes per second from program

	t_b = t_b * 1000.0 / CLOCKS_PER_SEC;	//in milliseconds
	t_h = t_h * 1000.0 / CLOCKS_PER_SEC;
	printf("\n1.Calculated average rate of input data in bytes per second : %d\n", calc_avg);
	printf("\n2.Program via calculated average rate of input data in bytes per second : %d\n", avg);
	printf("\n\nProgram determines the next “event”(either arrival of packet), and jumps to that time and services it\n");
	printf("\nTime taken by binary tree : %f",t_b);
	printf("\nTime taken by heap : %f",t_h);

	return 0;
}

/***************************OUTPUT******************************************/

/*

Enter time for which program is to be run (in seconds): 120

Enter the no. of packets arriving per second , lambda : 100

Enter the no. of bytes sent out per second , mu : 500

1.Calculated average rate of input data in bytes per second : 74800

2.Program via calculated average rate of input data in bytes per second : 77724


Program determines the next “event”(either arrival of packet), and jumps to that time and services it

Time taken by binary tree : 11.997000
Time taken by heap : 11.029000

*/

/******************************Inference*************************************************/
/*

Here, the heap sort hasn't been used as it is not required. A regular heap with smallest element on top would work.

The heap insert and binary insert have been implemented as efficient as I can, though I wasn't able to balance the tree, 
the heap uses memory very efficiently.

According to the above program, heap would be a better way to implement the  program as it appears to be faster in this case.
But if heapsort is used, then binary tree would be faster.

In the above code, heap is more efficient in terms of both memory and speed compared to that of binary tree.

*/