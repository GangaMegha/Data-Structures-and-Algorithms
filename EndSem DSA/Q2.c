/************************************************************Question 2 implementation 1************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 500       //vector length

typedef struct S1   //for storing the non-zero diagonal elements
{
    float value;
    int i;
    struct S1 *p;
    
}SA1;

typedef struct S2   //for storing the non-zero non-diagonal elements
{
    float value;
    int i,j;
    struct S2 *p;
    
}SA2;


struct S1 *sa1=NULL;        //linked list of non-zero diagonal elements in order
struct S2 *sa2=NULL;      //linked list of non-zero non-diagonal elements in order, from starting row and starting column and traverse row-wise. ie, for a particular i, j from 0 to N-1

int Ax_xTA(int x[])     //for calculating the required matrix products
{
    FILE *fout;
    fout = fopen("Output2.dat", "w");   //open output file with file pointer fout
    
    if(fout==NULL)      //if error in opening output, exit
    {
        printf("\nError opening output file!!");
        return(-1);
    }   

    float b[N]={0},c[N]={0};    //b stores A*x and c stores x(T)*A , both b and c are initialised to 0
    int i;
    
    SA1 *top=sa1;   //top points to sa1 which stores the location of the starting element details of the linked list for diagonal elements

    while(top!=NULL)    //condition for reaching end of the linked list
    {
        b[top->i]+=(top->value)*x[top->i];  //diagonal elements multiplied by the corresponding element in vector x and stored in the corresponding location in b
        c[top->i]=b[top->i];            //the above stated is true and same for both b and c
        top=top->p;             //incrementing top to next element in the linked list 
    }

    SA2 *Top=sa2;   //Top points to sa1 which stores the location of the starting element details of the linked list for non-diagonal elements

    while(Top!=NULL)    //condition for reaching end of the linked list
    {
        b[Top->i]+=(Top->value)*x[Top->j]; // the element pointed to by Top is multiplied by the corresponding element in x and added to corresponding element in b 
        c[Top->j]+=(Top->value)*x[Top->i]; // the element pointed to by Top is multiplied by the corresponding element in x and added to corresponding element in c
        Top=Top->p; //incrementing top to next element in the linked list 
    }

    fprintf(fout,"\n B = Ax is : ");    //writing the computed A[][]*x[] into the output file
    for(i=0;i<N;i++)
        fprintf(fout,"\t\n%f",b[i]);    //vertically written
    fprintf(fout,"\n\n C = (xT)A is :");    //writing the computed (x[]T)*A[][] into the output file
    for(i=0;i<N;i++)
        fprintf(fout,"    %f",c[i]);    //horizontally written
    return 0;   //return from the function into main()

}

/*****************************************************************if diagonal element and a valid marix element***************************************/

void createlist1(int r,float val)    //create the linked list for non-zero diagonal elements
{   
    int flag=1;     //to account whether the element has been added to the list

    if(r<=N)      //if the row number is valid for the matrix
    {
        SA1 *top=NULL;  //top points to sa1 which stores the location of the starting element details of the linked list for diagonal elements
        SA1 *temp = (SA1 *)malloc(sizeof(SA1)); //allocate space for the new element which will be stored in temp and added at appropriate position in the list
        
        temp->i = r;    //storing the the row index
        temp->value = val;  //store the value or element
        temp->p = NULL;     //temp->p currently initialised to NULL
        
        
        if(sa1==NULL)       //if list is empty
        {   
            sa1=temp;   //temp is the first element in the list and hence sa1 points to temp
        }
        else if(sa1 != NULL )       //if list is not empty
        {
            top=sa1;        //to navigate through the list
            
            if((temp->i)<(top->i))      //if the new item if to be added in front of sa1(first element in the linked list with lowest row index up till now)
            {
                temp->p=top;    //temp points to top
                sa1 = temp;     //sa1 points to temp as the element with the lowest row index
            }
  
            else if((temp->i)>(top->i))     //if the new item's index is greater than the row-index of top
            {   
                while(flag==1)  //till new element is inserted
                {
                    if(top->p==NULL)    //if temp is to be appended to the end of the list
                    {
                        top->p=temp;    //last element of sa1 points to temp
                        flag=0;         //denotes that new element has been inserted
                    }
                    else if((temp->i)>(top->i) && (temp->i)<(top->p->i))    //if the element is to be inerted in between the list
                    {
                        temp->p=top->p;     //temp is linked to the element to which top is linked to
                        top->p=temp;        //temp is linked to top
                        flag=0;         //denotes that new element has been inserted
                    }
                    top=top->p;     //incrementing top to next element in the linked list 
                }
            }
            
        }
    }
}

/********************************************************if non-diagonal element********************************************************************/

void createlist2(int r, int c, float val)    //create the linked list for non-zero non-diagonal elements
{   
    int flag=1;   
    
    if(r<=N && c<=N)    //if valid matrix element
    {
        SA2 *Top=NULL;      //Top points to sa2 which stores the location of the starting element details of the linked list for non-diagonal elements
        SA2 *Temp = (SA2 *)malloc(sizeof(SA2)); //allocate space for the new element which will be stored in temp and added at appropriate position in the list
       
        Temp->i = r;    //storing the the row index
        Temp->j = c;    //storing the the column index
        Temp->value = val;  //store the value or element
        Temp->p = NULL;     //Temp->p currently initialised to NULL
        
        long int index1, index2;    //for storing the matrix in a single line, the index used = i*N+j. This is stored in the index1 and index2
        
        if(sa2==NULL)       //if list is empty
        {
            sa2=Temp;   //Temp is the first element in the list and hence sa2 points to Temp
        }
        else if(sa2 != NULL)        //if list is not empty
        {
            Top=sa2;       //to navigate through the list
            
            index1=(Temp->i)*N + (Temp->j);     //index value for Temp
            index2=(Top->i)*N + (Top->j);       //index value for Top
            
            if(index1<index2)        //if the new item if to be added in front of sa2(first element in the linked list with lowest index up till now)
            {
                Temp->p=Top;        //Temp points to Top
                sa2 = Temp;         //sa2 points to Temp as the element with the lowest index
            }

            else if(index1>index2)      //if the new item's index is greater than the index of Top
            {   
                while(flag==1)      //till new element is inserted
                {
                    if(Top->p==NULL)    //if Temp is to be appended to the end of the list
                    {
                        Top->p=Temp;    //last element of sa2 points to Temp
                        flag=0;         //denotes that new element has been inserted
                    }
                    else if(index1>index2 && index1<((Top->p->i)*N + (Top->p->j)))  //if the element is to be inerted in between the list
                    {
                        Temp->p=Top->p;     //Temp is linked to the element to which Top is linked to
                        Top->p=Temp;        //Temp is linked to Top
                        flag=0;             //denotes that new element has been inserted
                    }
                    Top=Top->p;             //incrementing top to next element in the linked list 
                    index2=(Top->i)*N + (Top->j);   //re-initialisisng the value of index2
                }
            }
        }
    }
}

int main() 
{

   FILE *fin;      //open input file in read mode using file pointer fin
    fin = fopen("input2.dat", "r");
    
    if(fin==NULL)   //if error in opening input file
    {
        printf("\nError opening input file!!");
        return(-1);
    }   

    int val1,i,j,k,x[N];  //x[] stores the vector to be multiplied and val1 is used to read in vector elements
    float val2;     //val2 is used to read in matrix elements
    char *str,c;;
    str = (char *)malloc(80*sizeof(char));  //used while reading the file
    
    for(i=0;i<2;i++)
        fgets(str,80,fin);  //reading in the initial 2 strings in the input file
    
    for(k=0;k<2000;k++)
    {
        fscanf(fin,"%d",&i);    //read in row no.
        fscanf(fin,"%d",&j);    //read in column no.
        fscanf(fin,"%f",&val2); //read in element 
        if(val2!=0.0 && i==j)       //to store only non-zero elements (even if they are on the diagonal)
            createlist1(i, val2);   //function to store the diagonal elements in sa1 
        else if(val2!=0.0 && i!=j)
            createlist2(i, j, val2); //function to store the non-diagonal elements in sa2 

    }
    c = fgetc(fin);
    fgets(str,80,fin);  //read in the string before x[]

    i=0;    //initialise i to 0
    while(fscanf(fin,"%d",&val1)==1)    //read in vector x
    {
        x[i]=val1;  //read in value into x[i]
        i++;        //increment i
    }
   
    Ax_xTA(x);  //compute the products of A[][] with x[] as given in the question 
    free(str);  //free allocated memeory
    fclose(fin);    //close the input file

    return(0);
}