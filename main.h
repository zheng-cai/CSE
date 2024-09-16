//
//  sb.c
//  444
//
//  Created by 蔡政 on 9/26/19.
//  Copyright © 2019 蔡政. All rights reserved.
//
#ifndef main_h
#define main_h
#include <stdio.h>
#include <math.h>
void Newcentroids(int cnt,int dim,int centroidsNUM, int *cluster, int *clusterCNT, double data[cnt][dim],double newcentroids[centroidsNUM][dim]){
    for (int i=0;i<centroidsNUM;i++){
        for(int j=0;j<dim;j++){
            newcentroids[i][j]=0;
        }
    }
    for (int i=0;i<centroidsNUM;i++){
        for (int j=0;j<=cnt;j++){
            if (cluster[j]==i){
                for(int k=0;k<dim;k++){
                    newcentroids[i][k]=newcentroids[i][k]+data[j][k];
                }
            }
        }
        for(int k=0;k<dim;k++){
            newcentroids[i][k]=newcentroids[i][k]/clusterCNT[i];
        }
        
    }
}


/*main cluster process*/
void Cluster( int cnt, int dim,double data[cnt][dim],int centroidsNUM,double centroids[centroidsNUM][dim],int cluster[cnt],int clusterCNT[centroidsNUM]){
    /*kmeans
     get the center point*/
    
    
    int temp = 0;
    double root_mean_square=0;
    /*initialize all items in clusterCN to zero*/
    for (int i=0;i<centroidsNUM;i++){
        clusterCNT[i]=0;
    }
    
    /*calculate the min distance*/
    for (int i=0;i<cnt;i++){
        double min=INFINITY;
        for(int k=0; k<centroidsNUM;k++){
            double half_dist[dim],dist=0;
            for(int j=0;j<dim;j++){
                half_dist[j]=pow(data[i][j]-centroids[k][j],2);
                dist=dist+half_dist[j];
            }
            if (sqrt(dist)<min){
                min=sqrt(dist);
                temp=k;
            }
        }
        root_mean_square=root_mean_square+min;
        cluster[i]=temp;  /*get the cluster label for every point*/
        
        clusterCNT[temp]++;/*get the count of each cluster*/
    }
    /*output the centroids after this cluster*/
    for (int i=0;i<centroidsNUM;i++){
        printf("label %d's count: %d\n",i,clusterCNT[i]);
        printf("The centroid is ");
        for(int j=0;j<dim;j++){
            printf(" %lf ",centroids[i][j]);
        }
        printf("\n");
    }
    printf("root mean square:%lf\n",root_mean_square);
    printf("-------------------\n");
}


/*it will be stpopped when centroids don't change*/
int stop(int cnt, int dim,int centroidsNUM,double oldcentroids[centroidsNUM][dim],double newcentroids[centroidsNUM][dim]){
    int trigger=0,flag=0;
    for (int i=0;i<centroidsNUM;i++){
        double dist=0,half_dist[dim];
        for (int j=0;j<dim;j++){
            half_dist[j]=pow(newcentroids[i][j]-oldcentroids[i][j],2);
            dist=dist+half_dist[j];
        }
        if (sqrt(dist)<0.001){
            trigger++;
        }
    }
    if (trigger==centroidsNUM){
        flag=1;
    }
    return flag;
}
#endif
