/********************************************Note******************************************************/
/*	Drop rate has been defined as  : total number of drops *100/ total number of such packets ever arrived
	
	hence, drop rate at time = i is the  drop percentage till ith second
*/

/************************************Question 3********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NH 100000		//Maximum size of http queue
#define NV 100000		//Maximum size of video queue

typedef struct 
{
	char id[10];	//stores http or video
	float t0;	//Arrival time of packet in seconds
	float delay;	//Holds the packet delay
	int L;	//size of packet in bytes
	char *contents;	//pointer to contents of packet
}PACKET;

PACKET http[NH];		//http queue
PACKET video[NV];		//video queue

int fh=-1,rh=-1,fv=-1,rv=-1;		//fh & rh holds the index of first and last element of the http
									// circular queue. Same goes for fv and rv for the video circular queue.
float time=0; //to keep track of the time when the packets get dropped or sent

long int current_queue_length()	//returns the length of the http queue in bytes
{
	long int length;
	if(fh!=-1 && rh!=(-1))		//if que is not empty
	{
		if(rh>=fh)
			length=(rh-fh+1)*80;
		else
			length=(NH-fh+rh+1)*80;

		return (length);
	}
	else
		//printf("\nQueue empty");
		return(0);
}
float Last_packet_age()
{
	if(http[fh].t0<time)		//if the packet arrival time is less than the time after previous dequeue
		return(time-http[fh].t0);	
	else return 0;	
}
int main()
{
	FILE *fout,*fin;
	fout = fopen("Output.dat","w");	//output file
	fin = fopen("Input.dat","r");	//input file

	if(fout==NULL || fin==NULL)		//if files couldn't be opened
	{
		printf("\nError opening files!!");
		return(1);
	}

	int i;	//i counts the seconds passed
	float t_arrival=0;	//used while inserting the packets per second
	fscanf(fin,"%f",&t_arrival); //accepting the time of arrival of first packet
	char id[10];
	fscanf(fin,"%s",id);	//accepting id of first packet
	int flag=0;				//used to keep the percentage drop of video packets within the limit 
	int flag_v=0,flag_h=0;		//used later to check if queue is empty and also use while printing length
	long int size;	//to send out 64KB 
	long int dh=0,dv=0;	//http and video drop counter

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

		printf("\t   %f\t    %f\t   %f",(h_length*80.0+v_length*400.0)/1000,h_length*80.0/1000,v_length*400.0/1000);	//http and video in KB

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
		printf("\t%f    %f",dh*100.0/total_length_h,dv*100.0/total_length_v); //drops in percentage

		if(flag_h==1)
			total_length_h=0;
		if(flag_v==1)
			total_length_v=0;

		flag_v=0;
		flag_h=0;
/*************************************************************************************************************/

/**********************************inserting packets for a period of 1second*****************************/
		
		while(t_arrival<=i+1)	//in 1 second, 32kB = 80 video packets(each of 400bytes) inserted into video queue along with initial https
		{
			if(!strcmp(id,"http"))	//if the arrived packet is http
			{
				if(rh==NH-1)		//incrementing rear to queue the new element
					rh=0;
				else
					rh++;
				if(fh==rh)			//circular queue overflow
					break;
				
				http[rh].t0 = t_arrival;//assigning the time of arrival
				strcpy(http[rh].id,id);	//assigning the id
				
				http[rh].L=80;
				h_length++;		//http currently queued
				total_length_h++;		//ttal http ever arrived

				http[rh].delay=0;
				http[rh].contents=NULL;	//can be initialised using malloc 
				
				if(!feof(fin))
				{	
					fscanf(fin,"%f",&t_arrival);	//reading from file ( next packet's time of arrival and id)
					fscanf(fin,"%s",id);
				}
				else
					break;

				if(fh==-1)	//if queue was empty
					fh=0;
			}
			else if(!strcmp(id,"video")) //if the arrived packet was video
			{
				if(rv==NV-1)		//incrementing rear to queue the new element
					rv=0;
				else
					rv++;
				
				if(fv==rv)			//circular queue overflow
					break;
				
				video[rv].t0 = t_arrival;		//assigning the time of arrival
				strcpy(video[rv].id,id);	//assigning the id
				
				video[rv].L=400;
				v_length++;			//no. of currently queued video packets
				total_length_v++;	//total no. of video packets ever arrived

				video[rv].delay=0;
				video[rv].contents=NULL;	//can be initialised using malloc 
				
			
				if(!feof(fin))
				{
					fscanf(fin,"%f",&t_arrival);	//reading from file ( next packet's time of arrival and id)
					fscanf(fin,"%s",id);
				}
				else
					break;

				if(fv==-1)
					fv=0;
			}
		}

/**************************************************************************************************************/

/***************************************Senting out packets in 1 second*******************************************/
		size=0;
		while(size<64000 && (rh!=-1 || rv!=-1))
		{
			if(http[fh].t0<time)		//if the packet arrival time is less than the time after previous dequeue
				http[fh].delay = time-http[fh].t0;		//provide delay to the first(queued) packet
			else
				flag_h=1;
			
			if(video[fv].t0<time)		//if the packet arrival time is less than the time after previous dequeue
				video[fv].delay = time-video[fv].t0;		//provide delay to the first(queued) packet
			else
				flag_v=1;
			
			while(http[fh].delay>15 && fh!=-1)		//dropping http packets that exceed the delay
			{
				dh++;
				fprintf(fout,"\n%.7f\t%s\t%s",time,http[fh].id,"Dropped");
				h_length--;
				
				//deallocate contents if malloc had been used to allocate memory
				
				if(fh==rh)	//increment front, as a packet has been dropped
				{
					fh=-1;
					rh=-1;
				}
				else if(fh==NH-1)
					fh=0;
				else
					fh++;
			}

			while(video[fv].delay>1 && fv!=-1)	//dropping video packets if it exceeds the delay
			{
				dv++;
				fprintf(fout,"\n%.7f\t%s\t%s",time,video[fv].id,"Dropped");
				v_length--;

				//deallocate contents if malloc had been used to allocate memory
				
				if(fv==rv)	//increment front, as a packet has been  dropped
				{
					fv=-1;
					rv=-1;
				}
				else if(fv==NV-1)
					fv=0;
				else
					fv++;

			}

			if(dv*100.0/total_length_v>2)	//checking if the drop percentage is more than 2. It could be changed to 10, but them overall percentage of drops increase
			{								//this proved to decreade the drop percentage. Hence 2 and not 10
				flag=1;
				//break;
			}	
			else
				flag=0;

			if(flag==0 && http[fh].delay<15 && fh!=-1)		//sending out http packets. These get more preference because of flag
			{	
				size+=http[fh].L;
				if(flag_h==1)
					time=http[fh].t0;		//jump to the time instant when the packet actually arrived
				fprintf(fout,"\n%.7f\t%s\t%s",time,http[fh].id,"Sent");
				time+=0.00125;		//increment the time
				
				h_length--;

				//deallocate contents if malloc had been used to allocate memory

				if(fh==rh)	//increment front, as a packet has been dequeued 
				{
					fh=-1;
					rh=-1;
				}
				else if(fh==NH-1)
					fh=0;
				else
					fh++;
			}
			
			else if(video[fv].delay<1)	//sending out video packets
			{			
				size+=video[fv].L;
				if(flag_v==1)
					time=video[fv].t0;		//jump to the time instant when the packet actually arrived
				fprintf(fout,"\n%.7f\t%s\t%s",time,video[fv].id,"Sent");
				time+=0.00625;		//increment the time
		
				v_length--;

				//deallocate contents if malloc had been used to allocate memory
				
				if(fv==rv)	//increment front, as a packet has been dequeued 
				{
					fv=-1;
					rv=-1;
				}
				else if(fv==NV-1)
					fv=0;
				else
					fv++;
			}	
			
			flag=0;
			flag_h=0;
			flag_v=0;
			
		}
		
/*******************************************************************************************************************/
		
		i++;	//incrementing the second's counter.

	}

/*************************************16th second**************************************************/		

		printf("\n\n  %d",i);		//printing out 16th second

		printf("\t   %f\t    %f\t   %f",(h_length*80.0+v_length*400.0)/1000,h_length*80.0/1000,v_length*400.0/1000);	//http and video in KB

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
		printf("\t%f    %f",dh*100.0/total_length_h,dv*100.0/total_length_v); //drops in percentage

		if(flag_h==1)
			total_length_h=0;
		if(flag_v==1)
			total_length_v=0;

		flag_v=0;
		flag_h=0;

	fclose(fout);
	fclose(fin);
	return(0);
}

/*****************************************OUTPUT********************************************************

Time(s)   Queue length(KB)   HTTP Q(KB)   Video Q(KB)   HTTP Drops  Video Drops

  0	  		 0.000000	  	  0.000000	   0.000000		0.000000    0.000000

  1	  		 480.000000	  	  448.000000   32.000000	0.000000    0.000000

  2	  		 446.400000	  	  444.800000   1.600000		0.000000    2.500000

  3	  		 414.000000	  	  383.200000   30.800000	0.000000    2.083333

  4	  		 381.200000	  	  379.200000   2.000000		0.000000    2.187500
		
  5	  		 348.320000	  	  317.520000   30.800000	0.000000    2.250000

  6	  		 315.920000	  	  314.320000   1.600000		0.000000    2.083333

  7	  		 283.120000	  	  251.920000   31.200000	0.000000    2.142857

  8	  		 250.720000	  	  249.520000   1.200000		0.000000    2.031250

  9	  		 217.920000	  	  186.320000   31.600000	0.000000    2.083333

  10	     185.120000	      183.920000   1.200000		0.000000    2.125000

  11	     152.400000	      121.600000   30.800000	0.000000    2.045455

  12	     119.600000	      118.000000   1.600000		0.000000    2.083333

  13	     87.200000	      56.800000	   30.400000	0.000000    2.019231

  14	     54.400000	      52.400000	   2.000000		0.000000    2.053571

  15	     22.400000	      0.000000	   22.400000	0.000000    1.916667

  /***********************************************************************************************************/


