/********************************************Note******************************************************/
/*	Drop rate has been defined as  : total number of drops *100/ total number of such packets ever arrived
	
	hence, drop rate at time = i is the  drop percentage till ith second
*/
/************************************Question 1********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 1000000		//Maximum size of queue

typedef struct 
{
	char id[10];	//stores http or video
	float t0;	//Arrival time of packet in seconds
	float delay;	//Holds the packet delay
	int L;	//size of packet in bytes
	char *contents;	//pointer to contents of packet
}PACKET;

PACKET queue[N];

int f=-1,r=-1; //f & r hold the index of first and last element of the circular queue


int main()
{
	FILE *fout,*fin;
	fout = fopen("Output.dat","w");	//output file
	fin = fopen("Input.dat","r");	//input file

	if(fout==NULL || fin==NULL)
	{
		printf("\nError opening files!!");
		return(1);
	}

	int i;	//i counts the seconds passed
	float t_arrival=0;	//used while inserting the packets per second
	fscanf(fin,"%f",&t_arrival); //accepting the time of arrival of first packet
	
	long int size;	//to send out 64KB 
	long int dh=0,dv=0;	//http and video drop counter
	float time=0; //to keep track of the time when the packets get dropped or sent

	printf("\nTime(s)   Queue length(KB)   HTTP Q(KB)   Video Q(KB)   HTTP Drops  Video Drops");

	fprintf(fout, "\n\nTime\tType\tAction\n");
	
	long int v_length=0,h_length=0,total_length_h=0,total_length_v=0;	//v_length, h_lengh holds the current no. of http and video packets in the queue
																		//total_length_h,total_length_v holds the total no. of http and video packets queued uptil now.
	i=0;

/**************************************************MAIN LOOP**********************************************************/
	
	while(!feof(fin))		//run till end of file is reached
	{	
		printf("\n\n  %d",i);		//printing out time in seconds


/*************************************calculate queue length**************************************************/		

		printf("\t\t%ld\t\t%ld\t\t%ld",(h_length*80+v_length*400)/1000,h_length*80/1000,v_length*400/1000);	//http and video in KB
		if(v_length==0)
			v_length=1;
		if(h_length==0)
			h_length=1;
		if(total_length_v==0)
			total_length_v=1;
		if(total_length_h==0)
			total_length_h=1;
		printf("\t%f    %f",(float)dh*100/total_length_h,(float)dv*100/total_length_v); //drops in percentage

/*************************************************************************************************************/

/**********************************inserting packets for a period of 1second*****************************/
		
		while(t_arrival<i+1)	//in 1 second, 32kB = 80 video packets(each of 400bytes) inserted into video queue along with initial https
		{
			if(r==N-1)		//incrementing rear to queue the new element
				r=0;
			else
				r++;
			if(f==r)			//circular queue overflow
				break;
			
			queue[r].t0 = t_arrival;
			fscanf(fin," %s",queue[r].id);	//reading from file(http or video)
			
			if(!strcmp(queue[r].id,"http"))	//if the element to be queued is http
			{
				queue[r].L=80;
				h_length++;
				total_length_h++;
			}	
			else							//if the element to be queued is video
			{
				queue[r].L=400;
				v_length++;
				total_length_v++;
			}	
			queue[r].delay=0;
			queue[r].contents=NULL;	//can be initialised using malloc 
			
			fscanf(fin,"%f",&t_arrival);	//reading from file ( next packet's time of arrival)

			if(f==-1)
				f=0;
		}

/**************************************************************************************************************/

/***************************************Senting out packets in 1 second*******************************************/
		size=0;
		while(size<64000 && r!=-1)
		{
				
			//deallocate contents if malloc had been used to allocate memory
			
			if(queue[f].t0<time)		//if the packet arrival time is less than the time after previous dequeue
				queue[f].delay = time-queue[f].t0;		//provide delay to the first(queued) packet
			else
				time=queue[f].t0;		//jump to the time instant when the packet actually arrived

			if(!strcmp(queue[f].id,"video")) 
			{
				if(queue[f].delay<1)	//sent out the packet since delay is within limits
				{
					size+=queue[f].L;
					fprintf(fout,"\n%.7f\t%s\t%s",time,queue[f].id,"Sent");
					time+=0.00625;		//increment the time
				}	
				else				//drop the packet as the delay exceeded
				{
					dv++;
					fprintf(fout,"\n%.7f\t%s\t%s",time,queue[f].id,"Dropped");
				}
				
				v_length--;
			}
			
			else if(!strcmp(queue[f].id,"http"))	
			{
				if(queue[f].delay<15)		//sent out the packet since delay is within limits
				{
					size+=queue[f].L;
					fprintf(fout,"\n%.7f\t%s\t%s",time,queue[f].id,"Sent");
					time+=0.00125;
				}	
				else		//drop the packet as the delay exceeded
				{
					dh++; 
					fprintf(fout,"\n%.7f\t%s\t%s",time,queue[f].id,"Dropped");
				}	
				
				h_length--;
			}
			if(f==r)	//increment front, as a packet has been dequeued or dropped
			{
				f=-1;
				r=-1;
			}
			else if(f==N-1)
				f=0;
			else
				f++;
		}
		
/*******************************************************************************************************************/
		
		i++;	//incrementing the second's counter.

	}

	fclose(fout);
	fclose(fin);
	return(0);
}

/*****************************************OUTPUT********************************************************

Time(s)   Queue length(KB)   HTTP Q(KB)   Video Q(KB)   HTTP Drops  Video Drops

  0				0				0				0			0.000000    0.000000

  1				480				448				32			0.000000    0.000000

  2				448				384				64			0.000000    0.000000
	
  3				416				320				96			0.000000    0.000000

  4				384				256				128			0.000000    0.000000

  5				352				192				160			0.000000    0.000000

  6				320				128				192			0.000000    0.000000

  7				288				64				224			0.000000    0.000000

  8				256				0				256			0.000000    0.000000

  9				224				0				224			0.000000    22.222221

  10			192				0				192			0.000000    40.000000

  11			160				0				160			0.000000    54.545456

  12			128				0				128			0.000000    58.229168

  13			96				0				96			0.000000    53.750000

  14			64				0				64			0.000000    49.910713

  15			32				0				32			0.000000    46.583332

  16			0				0				0			0.000000    43.671875

  /***********************************************************************************************************/
