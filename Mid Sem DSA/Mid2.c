/********************************************Note******************************************************/
/*	Drop rate has been defined as  : total number of drops *100/ total number of such packets ever arrived
	
	hence, drop rate at time = i is the  drop percentage till ith second
*/
/************************************Question 2********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 1000000		//Maximum size of queue

typedef struct 
{
	char id[10];	//http or video packet
	float t0;	//Arrival time of packet in millisecond
	float delay;	//Holds the packet delay
	int L;	//size of packet in bytes
	float key;	//key for forming heap
	char *contents;	//pointer to contents of packet
}PACKET;

PACKET Heap[N];

void swap(int i, int j)
{
	PACKET temp;
	strcpy((temp.id),(Heap[i].id));
	(temp.t0)=(Heap[i].t0);
	(temp.L)=(Heap[i].L);
	(temp.delay)=(Heap[i].delay);
	(temp.key)=(Heap[i].key);
	(temp.contents)=(Heap[i].contents);

	strcpy((Heap[i].id),(Heap[j].id));
	(Heap[i].t0)=(Heap[j].t0);
	(Heap[i].L)=(Heap[j].L);
	(Heap[i].delay)=(Heap[j].delay);
	(Heap[i].key)=(Heap[j].key);
	(Heap[i].contents)=(Heap[j].contents);

	strcpy(Heap[j].id,temp.id);
	Heap[j].t0=temp.t0;
	Heap[j].L=temp.L;
	Heap[j].delay=temp.delay;
	Heap[j].key=temp.key;
	Heap[j].contents=temp.contents;
}

void heapify(int n, int i)
{
    int largest = i;  // Initialize largest as root
    int l = 2*i + 1;  // left = 2*i + 1
    int r = 2*i + 2;  // right = 2*i + 2

    // If left child is larger than root
    if (l < n && Heap[l].key > Heap[largest].key)
        largest = l;

    // If right child is larger than largest so far
    if (r < n && Heap[r].key > Heap[largest].key)
        largest = r;

    // If largest is not root
    if (largest != i)
    {
        swap(i,largest);

        // Recursively heapify the affected sub-tree
        heapify(n,largest);
    }
}

void build_heap(int n)
{
	int i;
	// Build heap (rearrange array)
    for (i = n/ 2-1 ; i >= 0; i--)
        heapify(n,i);
}

int main()
{
	FILE *fout,*fin;
	fout = fopen("Output.dat","w");	//output file
	fin = fopen("HomeAssg_input.dat","r");	//input file

	if(fout==NULL || fin==NULL)
	{
		printf("\nError opening files!!");
		return(1);
	}

	int i;	//i counts the seconds passed
	float t_arrival=0;	//used while inserting the packets per second
	fscanf(fin,"%f",&t_arrival); //accepting the time of arrival of first packet
	int flag_v=0, flag_h=0;		//to avoid division by zero error
	long int size;	//to send out 64KB 
	long int dh=0,dv=0;	//http and video drop counter
	float time=0; //to keep track of the time when the packets get dropped or sent

	printf("\nTime(s)   Queue length(KB)   HTTP Q(KB)   Video Q(KB)   HTTP Drops  Video Drops");

	fprintf(fout, "\n\nTime\tType\tAction\n");
	
	long int v_length=0,h_length=0,total_length_h=0,total_length_v=0;	//v_length, h_lengh holds the current no. of http and video packets in the queue
																//total_length_h,total_length_v holds the total no. of http and video packets queued uptil now.
	long int n=0; //	total no.of elements in heap
	i=0;

	while(!feof(fin))
	{
		printf("\n\n   %d",i);

		printf("\t   %f\t   %f\t   %f",(h_length*80.0+v_length*400.0)/1000,h_length*80.0/1000,v_length*400.0/1000);	//length in KB

		if(total_length_v==0)		//to avoid division by zero
		{
			flag_v=1;
			total_length_v=1;
		}
		if(total_length_h==0)		//to avoid division by zero
		{
			flag_h=1;
			total_length_h=1;
		}
		printf("\t%f    %f",(float)dh*100/total_length_h,(float)dv*100/total_length_v); //drops in percentage
		if(flag_h==1)
			total_length_h=0;
		if(flag_v==1)
			total_length_v=0;
		
		flag_v=0;
		flag_h=0;
/**********************************inserting  video packets for a period of 1second*****************************/

		while(t_arrival<i+1)	//in 1 second, 32kB = 80 video packets(each of 400bytes) inserted into video queue along with initial https
		{			
			Heap[n].t0 = t_arrival;
			fscanf(fin," %s",Heap[n].id);	//reading from file(http or video)
			
			if(!strcmp(Heap[n].id,"http"))	//if the element to be queued is http
			{
				Heap[n].L=80;
				h_length++;
				total_length_h++;
			}	
			else							//if the element to be queued is video
			{
				Heap[n].L=400;
				v_length++;
				total_length_v++;
			}	
			Heap[n].delay=0;
			Heap[n].contents=NULL;	//can be initialised using malloc 
			Heap[n].key=Heap[n].L+340/(Heap[n].t0+1.0);		//key for heap
			
			n++;
			if(!feof(fin))
				fscanf(fin,"%f",&t_arrival);	//reading from file ( next packet's time of arrival)
			else break;
		}

		build_heap	(n);	//	forming the heap

/***************************************Senting out packets in 1 second*******************************************/
		
		size=0;		//size denotes bytes, sent out <=64kb(=64000)
		
		while(size<64000 && n!=0)
		{
				
			
			if(Heap[0].t0<time)		//if the packet arrival time is less than the time after previous dequeue
				Heap[0].delay = time-Heap[0].t0;		//provide delay to the first(queued) packet
			else
				time=Heap[0].t0;		//jump to the time instant when the packet actually arrived and send the packet out

			if(!strcmp(Heap[0].id,"video")) 
			{
				if(Heap[0].delay<1)	//sent out the packet since delay is within limits
				{
					size+=Heap[0].L;
					fprintf(fout,"\n%.7f\t%s\t%s",time,Heap[0].id,"Sent");
					time+=0.00625;		//increment the time
				}	
				else				//drop the packet as the delay exceeded
				{
					dv++;
					fprintf(fout,"\n%.7f\t%s\t%s",time,Heap[0].id,"Dropped");
				}
				
				v_length--;
			}
			
			else if(!strcmp(Heap[0].id,"http"))	
			{
				if(Heap[0].delay<15)		//sent out the packet since delay is within limits
				{
					size+=Heap[0].L;
					fprintf(fout,"\n%.7f\t%s\t%s",time,Heap[0].id,"Sent");
					time+=0.00125;
				}	
				else		//drop the packet as the delay exceeded
				{
					dh++; 
					fprintf(fout,"\n%.7f\t%s\t%s",time,Heap[0].id,"Dropped");
				}	
				
				h_length--;
			}
			swap(0,(n)-1);	//shifting element to the nth index
			(n)--;  	//removing element	
			heapify(n,0);	//reorganise the rest of the heap

			//deallocate contents if malloc had been used to allocate memory
			//if memory was allocated :		free((Heap[n_h].contents));
									//		(Heap[n_h].contents)=NULL;	
		}	

		i++;		//incrementing the second's counter
	}
	return (0);
}

/**********************************************OUTPUT*******************************************************

Time(s)   Queue length(KB)   HTTP Q(KB)   Video Q(KB)   HTTP Drops  Video Drops

   0	   0.000000	   		 0.000000	   0.000000		0.000000    0.000000

   1	   491.920000	     491.920000	   0.000000		0.000000    0.000000

   2	   459.920000	     459.920000	   0.000000		0.000000    0.000000

   3	   427.920000	     427.920000	   0.000000		0.000000    0.000000

   4	   395.920000	     395.920000	   0.000000		0.000000    0.000000

   5	   363.920000	     363.920000	   0.000000		0.000000    0.000000

   6	   331.920000	     331.920000	   0.000000		0.000000    0.000000

   7	   299.920000	     299.920000	   0.000000		0.000000    0.000000

   8	   267.920000	     267.920000	   0.000000		0.000000    0.000000

   9	   235.920000	     235.920000	   0.000000		0.000000    0.000000

   10	   203.920000	     203.920000	   0.000000		0.000000    0.000000

   11	   171.920000	     171.920000	   0.000000		0.000000    0.000000

   12	   139.920000	     139.920000	   0.000000		0.000000    0.000000

   13	   107.920000	     107.920000	   0.000000		0.000000    0.000000

   14	   75.920000	     75.920000	   0.000000		0.000000    0.000000

   15	   0.000000	   		 0.000000	   0.000000		14.433933   0.000000

   16	   0.000000	  		 0.000000	   0.000000		14.433933   0.000000g

/****************************************************************************************************************/