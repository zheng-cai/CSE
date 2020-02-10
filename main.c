//
//  main.c
//  777
//
//  Created by 蔡政 on 11/14/19.
//  Copyright © 2019 蔡政. All rights reserved.
//

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define threads 10
#define size 50
int main(void){
    
    
    if (threads<=0 || size<=0){
        printf("Errors in the threads and sizes");
        exit(1);
    }
    double a[size][size];
    double b[size][size];
    double c[size][size];
    double d[size][size];
    int row=0;
    //initializing the matrix
    for (int i=0;i<size; i++){
        for (int j=0;j<size; j++){
            b[i][j]=rand()*1.0/RAND_MAX;
            a[i][j]=rand()*1.0/RAND_MAX;
            c[i][j]=0;
            d[i][j]=0;
    
    }
    }
        printf("The matrix a is\n");
         for (int i=0;i<size; i++){
                for (int j=0;j<size; j++){
                    printf("%f ",a[i][j]);
                }
               printf("\n");
        }
        printf("The matrix b is\n");
            for (int i=0;i<size; i++){
                   for (int j=0;j<size; j++){
                       printf("%f ",b[i][j]);
                   }
                  printf("\n");
           }
    
    
    //static parallel programming
    omp_set_num_threads(threads);
    double  start_static = omp_get_wtime();
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        for (int i=id;i<size;i+=threads){
            for (int j=0;j<size;j++){
                for (int k=0;k<size;k++){
                    c[i][j]+=a[i][k]*b[k][j];
                
                }
               
            }
        }
    }
    //get the running time of static parallel programming
    double time1=omp_get_wtime()-start_static;
     printf("The matrix c is\n");
        for (int i=0;i<size; i++){
                for (int j=0;j<size; j++){
                    printf("%f ",c[i][j]);
                }
               printf("\n");
        }


     // dynamic  parallel programming
 double  start_dyn = omp_get_wtime();
    #pragma omp parallel
{
        while(row<size){
            int temp=0;
            double Temp[size];
            for(int i=0;i<size;i++){
                Temp[i]=0;
            }
            #pragma omp critical
            {
                    temp = row;
                    row++;
            }
            for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                    Temp[i]+=a[temp][j]*b[j][i];
                }
            }
            #pragma omp critical
            {
                for(int i=0;i<size;i++){
                    d[temp][i]=Temp[i];
                }
            }
        }
        
    }
    //get the running time of dynamic parallel programming
    double time2=omp_get_wtime()-start_dyn;
    printf("The matrix d is\n");
    for (int i=0;i<size; i++){
            for (int j=0;j<size; j++){
                printf("%f ",d[i][j]);
            }
           printf("\n");
    }
    
    printf("Thread: %d, Size of matrix: %d,\n Static running time: %lf,\n Dynamic running time: %lf\n",threads,size,time1,time2);
}
    

