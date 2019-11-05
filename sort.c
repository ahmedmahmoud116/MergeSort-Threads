#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define bigint 2147483647 //as infinity

int *arr;
int n;

void printArray(int A[], int size) ;

void * mergesortTH(void * u){

    int *arg = u;

    int l = arg[0];
    int r = arg[1];

    int arg1[2];    // to have arguments for thread 1 to avoid mutual exclusion
    int arg2[2];    // to have arguments for thread 2 to avoid mutual exclusion


    if (l < r)
    {
        int m = (l+r)/2;

        pthread_t thread1; //thread 1 to have half of sorting
          arg1[0] = l;
          arg1[1] = m;

       // mergeSort(arr, l, m);   //divide
        pthread_create(&thread1,NULL,&mergesortTH,arg1); //divide


        pthread_t thread2; //thread 2 to have half of sorting
          arg2[0] = m+1;
          arg2[1] = r;
       // mergeSort(arr, m+1, r); //divide
        pthread_create(&thread2,NULL,&mergesortTH,arg2); //divide


        /***waiting for two threads to join before merge**/
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        merge(arr, l, m, r);    //conquer
    }
}

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int L[n1 + 1], R[n2 + 1]; //to merge the left and right array

    /*put the left and right array in them*/
    for (i = 0; i < n1; i++){
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++){
        R[j] = arr[m + 1 + j];
    }

    L[n1] = bigint;    //so when j and i be equal n last element of the array it continue work and get the rest of element of L or R
    R[n2] = bigint;

    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray


    for(k = l;k<=r;k++)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
    }
}

int readfromFilee(){
   FILE *fptr = fopen("input.txt","r");
     if (fptr == NULL){
        printf("Could not open input file\n");
        return 0;
    }
    else{
         fscanf(fptr,"%d",&n);
         arr = malloc(n * sizeof(int));
         for (int i = 0; i < n; i++) {
            fscanf(fptr,"%d",&arr[i]);
         }
         return 1;
    }
}

void writeinFile(){
    FILE *fptr = fopen("output.txt","w");// to reset the file everytime open the program
    fprintf(fptr,"Output Array\n");
    for (int i = 0; i < n; i++) {
      fprintf(fptr,"%d ",arr[i]);
    }
    fclose(fptr);
}

int main()
{
    /**reading from file**/
    int check = readfromFilee();
    if(check == 0){
        return 0;
    }

    /***print array***/
    printf("Input array is \n");
    printArray(arr, n);

    /***array to have arguments for threads have l and r of every subarray***/
    int arg[2];
    arg[0] = 0;    // l
    arg[1] = n - 1; // r

    pthread_t thread; // the first thread

    pthread_create(&thread,NULL,&mergesortTH,arg);


    pthread_join(thread,NULL); // wait for the last thread then print it

    writeinFile();

    /***print array***/
    printf("\nSorted array is \n");
    printArray(arr, n);


    return 0;
}

void printArray(int A[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}
