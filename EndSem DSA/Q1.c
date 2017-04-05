/**************************************************************Question 1**********************************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

#define M 15

 
int cmpfunc (const void * a, const void * b)    //a utility function for using inbuilt function for quicksort from library
{
   return ( *(int*)a - *(int*)b );
}

int greedy(int Weight[], int weights[][3000],int N[],int flag)  //greedy algorithm. Weights has the max_weight , weights has the available weights and N has the no of weights for different test cases (flag)
{
    int i1,i2,sum1=0,sum2=0,i,j;
    int W,w[3000],n;

    printf("\n\n\t\tGreedy Algorithm\n");

    for(i=0;i<=flag;i++)    //iterate through the different test cases
    {
        W=Weight[i];    //max_weight
        n=N[i];     //no. of weights
        for(j=0;j<n;j++)    //assigning the weights for test case i
            w[j]=weights[i][j];
       
        sum1=0; //sum via method a
        sum2=0; //sum via method b

        qsort(w, n, sizeof(int), cmpfunc);  //sort weights in ascending order
        
        /**************************************Traverse till a particular weight can't be added**************************************************/
        for(i1=0;i1<n;i1++)     //front approach, ie, maximum no.of weights are added to the sack
        {
            sum1+=w[i1];    //add weight to the cumulative sum of weights 
            if(sum1+log(i1+1)>W)    //if condition is not satisfied, then limit reached. subtract the last added weight
            {  
                sum1-=w[i1];
                break;  
            }
        }

        for(i2=n-1;i2>=0;i2--)      //bag approach ie, minimum no.of weights are added to the sack
        {
            sum2+=w[i2];    //add weight to the cumulative sum of weights 
            if(sum2+log(n-i2)>W)     //if condition is not satisfied, then limit reached. subtract the last added weight
            {
                sum2-=w[i2];
                break;
            }
        }
        printf("\nTest case %d :",i+1);
        printf("\n\tGreedy Algorithm-1 used weights : ");

        if(sum1>sum2)        //If method 1a gives larger sum
        {
            for(j=0;j<i1;j++)
                printf("   %d",w[j]);   //print out the used weights
            printf("\n\tObtained sum = %d",sum1);   //print  max sum of weights
        }
        else        //if method 1b gives larger sum
        {
           for(j=n-1;j>i2;j--)
                printf("   %d",w[j]);   //print out the used weights
            printf("\n\tObtained sum = %d",sum2);   //print out the maximum sum of weights
        }

        /*********************************Traversing even if a weight in between cannot be added**********************************************/
        int count1=0,count2=0;      //keeps track of the no. of weights accepted in 2a and 2b
        int index1[n],index2[n];    //stores the index of the weights accepted
        sum1=0;     //stores the cumulative sum of weights in method 2a
        sum2=0;     //stores cumulative sum of weights in method 2b

        for(i1=0;i1<n;i1++)     //front approach, ie, maximum no.of weights are added to the sack
        {
            if(sum1+w[i1]+log(count1+1)<=W) //as long as condition is satisfied
            {
                index1[count1]=i1;  //store index of the accepted weight
                count1++;   //increment the no. of weights accepted 
                sum1+=w[i1];    //increment the sum
            }   
              
        }

        for(i2=n-1;i2>=0;i2--)      //bag approach ie, minimum no.of weights are added to the sack
        {
            if(sum2+w[i2]+log(count2+1)<=W) //as long as condition is satisfied
            {
                index2[count2]=i2;  //store index of the accepted weight
                count2++;        //increment the no. of weights accepted 
                sum2+=w[i2];    //increment the sum
            }
        }

        printf("\n\tGreedy Algorithm-2 used weights :");

        if(sum1>sum2)          //If method 1a gives larger sum
        {
            for(j=0;j<count1;j++)   //print out the accepted weights using method 2a
                printf("   %d",w[index1[j]]);
            printf("\n\tObtained sum = %d",sum1);   //print the cumulative sum in method 2a
        }
        else        //If method 1a gives larger sum
        {
            for(j=0;j<count2;j++)
                printf("   %d",w[index2[j]]);    //print out the accepted weights using method 2b
            printf("\n\tObtained sum = %d",sum2);    //print the cumulative sum in method 2b
        }
    }
}

int dynamicp(int Weight[], int weights[][3000],int N[],int flag)    //dynamic approach for knapsack
{
    int i,j,m,p;
    int W,w[3000],n;

    FILE *fout;     //output file to write the table
    fout = fopen("Output1.dat","w");   

    if(fout==NULL)
    {
        printf("\nError opening Output1 file!!");
    }

    fprintf(fout, "\n\n\t\tDynamic Programming\n");
    printf("\n\n\t\tDynamic Programming\n");

    for(p=0;p<=flag;p++)    //iterate through the various test cases
    {
        W=Weight[p];    //max_weight for test case p
        n=N[p];         //no. of weights for test case p
        for(j=0;j<n;j++)
            w[j]=weights[p][j]; //weights for test case p

        int K[n+1][W+1],count[n+1][W+1];    //K stores cumulative sum. K[i][j]=sum <= j-log(m) using the first i weights
        int keep[n+1][W+1]; //to keep track of the weights used to obtain K[][].

        
        for(j=0;j<=W;j++)   //first row of zeros
        {
            K[0][j] = 0;
            count[0][j]=0;
        }
        for(i=1;i<=n;i++)   //first column of zeros
        {
            K[i][0]=0;
        }

        for(i=1;i<=n;i++)   //filling the rest of the K matrix
        {
            for(j=0;j<=W;j++)
            {
                if(w[i-1]<=j && (K[i-1][j-w[i-1]] + w[i-1] > K[i-1][j]) && K[i-1][j-w[i-1]] + w[i-1]<=j-log(count[i-1][j-w[i-1]]+1)) // if condition holds when weight added, add weight
                {
                    K[i][j] = K[i-1][j-w[i-1]] + w[i-1];    //w[i-1] considered
                    count[i][j]=count[i-1][j-w[i-1]]+1;     //increment the no. of used weights at position(i,j)
                    keep[i][j]=1;   //note down that weight has been used
                }
                else //if by adding the weight condition fails
                {
                    K[i][j] = K[i-1][j];    //w[i-1] not considered
                    count[i][j]=count[i-1][j];  //count is same as without w[i-1]
                    keep[i][j]=0;   //note down that weight has not been used
                }

            }
        }
        
        fprintf(fout,"\n\n\nTable for Test case %d : ",p+1);

        for(i=0;i<=n;i++)       //print out K[][] onto the output file
        {
            fprintf(fout,"\n");
            for(j=0;j<=W;j++)
                fprintf(fout,"   %d",K[i][j]);
        }
        printf("\nTest case %d : ",p+1);
        printf("\n\tWeights used : ");

        j=W;    //initialise max_weight
        for(i=n;i>0;i--)    //printing out the used weghts by traversing through the keep matrix
            if(keep[i][j]==1)   //weight used
            {
                printf("   %d",w[i-1]);
                j=j-w[i-1]; //subtract the used weight from the cumulative sum
            }

        printf("\n\tMaximum attained sum of weights : %d",K[n][W] );    //max sum of weights obtained under the given condition
    }

    return 0;
 }

int main() 
{
    /*********assumed max. no. of test cases as M****/
    FILE *myFile;
    myFile = fopen("Input1.dat", "r");  //open input file using file pointer fin
    if(myFile==NULL)
        printf("wrong\n");
    char c;
    int W[M],i,j,flag=0;  //W[] holds max capacity for different test cases , flag keepd track of the test cases
    int w[M][3000];    //w[i][] holds the weights for the ith test case
    int n[M]={0};  //n[i] hold the no. of weights in the ith test case
    int count[M]={0};  //used while accepting the values, to keep track of \n
   

    for (c = fgetc(myFile); c != EOF; c = fgetc(myFile))        //reading from file character-wise
    {
        if(c=='\n') //if \n encoutered, increment count
            count[flag]++;
        if(isdigit(c) && count[flag]==0)    //if c is a digit
         {
            fseek(myFile,-1,SEEK_CUR);  //shift file pointer before c
            fscanf(myFile,"%d",&W[flag]);   //read in W[0]
         }  
        else if(isdigit(c) && count[flag]==1)   //if the digit comes after 1 \n
        {
            fseek(myFile,-1,SEEK_CUR);
            fscanf(myFile,"%d",&w[flag][n[flag]]);  //read in w[][]
            n[flag]++;
            
        }

        if(count[flag]>1 && isdigit(c)) //if no. of \n is more than one and digit encountered => new test case
        {
            flag++;            //acknowledge that its a new test case
            fseek(myFile,-1,SEEK_CUR);  //move file pointer to before c 
            fscanf(myFile,"%d",&W[flag]);   //read in the W for the next test case
        }
    }

    fclose(myFile);

    greedy(W,w,n,flag);
    dynamicp(W,w,n,flag);
 
    return 0;

}