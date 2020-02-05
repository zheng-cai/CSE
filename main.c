//
//  main.c
//  333
//
//  Created by 蔡政 on 9/10/19.
//  Copyright © 2019 蔡政. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct Queuetype FIFO;
struct Queuetype{
    int i,j;
    FIFO *next;
};
FIFO *FQ_create(){
    FIFO *FQ=(FIFO*)malloc(sizeof(FIFO));
    FQ->next=NULL;
    if (FQ==NULL){
        printf("Error in allocating");
        return NULL;
    }
    return FQ;
}


/* insert an item into FIFO queue*/
 int FQ_insert(FIFO*FQ,int i,int j){
    FIFO *insert=(FIFO*)malloc(sizeof(FIFO));
    FIFO *current=(FIFO*)malloc(sizeof(FIFO));
    current=FQ;
    insert->i=i;
    insert->j=j;
    if (current->next==NULL){ /*check if it's an empty queue*/
        current->next=insert;
        insert->next=NULL;
        return 0;
    }
     while (current->next!=NULL){
        current=current->next;
     }
     current->next=insert;
     insert->next=NULL;
     return 0;
    }


/*pop the first item out of the queue*/
int FQ_pop(FIFO*FQ){
    if (FQ->next==NULL){    /*the case of empty queue*/
        return 0;
    }
    FIFO *temp=FQ->next;
    FQ->next=temp->next;
    free(temp);
    
    return 0;
}


/*generating a grid with black(1) and white(0) node*/
int**generate(int K, double P){
    srand(time(NULL));  /*set the seed*/
    int i,j;
    int**grid=(int**)malloc(K*sizeof(int*));
    for(i=0;i<=K-1;i++){
        grid[i]=(int*)malloc(K*sizeof(int));
        for (j=0;j<=K-1;j++){
            if ((rand()%100)<P*100){
                grid[i][j]=1; /*let 1 be the black*/
            }
            else{
                grid[i][j]=0; /*let 0 be the white*/
        }
    }
}
    return grid;
}


/*print the grid */
void grid_print(int **grid,int K){
    int i,j;
    for(i=0;i<=K-1;i++){
        for(j=0;j<=K-1;j++){
            printf("%d ",grid[i][j]);
        }
        printf("\n");
    }
}


/*Breadth First Search */
int BFS(int**grid,FIFO*FQ,int K){
    int i,j,count=1,max=0;
    for(i=0;i<K;i++){
        for(j=0;j<K;j++){
            if(grid[i][j]==1){  /*use the first black one as the start point */
                FQ_insert(FQ,i,j);
                grid[i][j]=2;
                count=1;
                while(FQ->next!=NULL ){
                    if (i-1>=0){   /*check if i-1 is out of range */
                        if (grid[i-1][j]==1){
                            count++;
                            grid[i-1][j]=2;   /*let the searched one be other color*/
                            FQ_insert(FQ,i-1,j);
                        }
                    }
                    if(i+1<=K-1){    /*check if i+1 is out of range */
                        if (grid[i+1][j]==1){
                            count++;
                            grid[i+1][j]=2;
                            FQ_insert(FQ,i+1,j);
                        }
                    }
                    if(j-1>=0){    /*check if j-1 is out of range */
                        if (grid[i][j-1]==1){
                            count++;
                            grid[i][j-1]=2;
                            FQ_insert(FQ,i,j-1);
                        }
                    }
                    if(j+1<=K-1){   /*check if j+1 is out of range */
                        if (grid[i][j+1]==1){
                            count++;
                            grid[i][j+1]=2;
                            FQ_insert(FQ,i,j+1);
                        }
                    }
                    FQ_pop(FQ); /*get out the first item, and search adjacency of the next point */
                    if (FQ->next!=NULL){
                        i=FQ->next->i;
                        j=FQ->next->j;
                    }
                }
                if(count>max){   /*get the max component*/
                    max=count;
                }
            }
            
        }
    }
    return max;
}
int main(int argc, const char * argv[]){
    int i,j;
    int K[3]={5,10,100};
    double P[3]={0.2,0.5,0.7};
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            int **grid=generate(K[i],P[j]);
            grid_print(grid,K[i]);
            FIFO *FQ=FQ_create();
            int a=BFS(grid,FQ,K[i]);
            printf("the largest component containing %d cells\nThe K is %d\nThe P is %lf\n",a,K[i],P[j]);
            
        }
    }
}






