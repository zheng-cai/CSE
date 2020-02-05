//
//  zhengcai.c
//  111
//
//  Created by Zheng Cai on 8/24/19.
//  Copyright © 2019 Zheng Cai. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
double a,b,c,r,i,max,Z;
int K,k,j,n,upper,low;
double fx( double x);

int main(){
    /*
     *set seed for random function
     */
    srand(time(NULL));
    /*
     *input the interval [a,b] and test if it is a number and ensure b>=a by a loop
     */
    do{
        printf("Enter a value for a:\n ");
        low=scanf("%lf", &a);
        getchar();
        printf("Enter a value for b:\n ");
        upper=scanf("%lf", &b);
        getchar();
        if (low==0||upper==0){
            printf("Type error\n");
        }
        else if(a>b){
            printf("a must smaller than b\n");
        }
        else{
            printf( "You entered a=%f and b=%f\n", a, b);
        }
    }while(a>b||low==0||upper==0);
    
    /*
     *test if fx is integrable in [a,b]
     */
    for (i=a;i<=b; i+=1){
        if (fx(i)== -INFINITY||fx(i)== INFINITY){
            printf("This function cannot be integrated with [%lf,%lf]\n",a,b);
            return 0;
        }
    }/* increasing the value of i and see if f(i) get the incredible large value*/
    
    /*
     *get the value of k by choosing the point larger than the maximum of fx
     */
    max=fx(a);
    for (i=a;i<=b;i+=0.01){
        if (fx(i)>max){
            max=fx(i);
        }
    } /*try to get the max value of fx by the loop*/
    K=ceil(max)+1;
    
    /*
     *get the n of the count of points that under fx function's line
     *and calculate Z by different N
     */
    int N[5]={1000,10000,100000,1000000,10000000};
    for(k=0;k<=4;k++){
        n=0;
        for (i=a;i<=b;i+=(b-a)/(N[k]/100)){
            for (j=0;j<100;j++){
                r=(rand()%(K*1000))/1000.0;
            if ( r<=fx(i)){
                n=n+1; /*for particular i, if the random number r is less f(i),then it is counted to n */
            }
            }/* for every i in [a,b], generating 100 points*/
        }
        Z=n*K*(b-a)/N[k];
        printf("The integration is %lf and N is %d\n",Z,N[k]);
    
}
}

/*
 *define a function that is used for integral computation
 */
double fx( double x){
    return x*x;
    }
