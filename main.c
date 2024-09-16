//
//  main.c
//  444
//
//  Created by 蔡政 on 9/22/19.
//  Copyright © 2019 蔡政. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "main.h"

/*relocate the centroids*/
/*initialization*/




int main(int argc, const char * argv[]) {
    
    FILE *Kmeans=fopen(argv[1], "r");
    int a[2]={};
    /*read the first line to know how many data and dimension*/
    for(int i=0;i<2;i++){
        fscanf(Kmeans,"%d ",&a[i]);
    }
    int cnt=a[0], dim=a[1];
    
    double data[cnt][dim];
    for (int i=0;i<cnt;i++){
        for (int j=0; j<dim;j++){
            fscanf(Kmeans,"%lf ",&data[i][j]);
        }
        
    }
    fclose(Kmeans);
    
   /*kmeans
    get the first k points in the data as centroids */
    int k;
    k=atoi(argv[2]);
    int cluster[cnt],clusterCNT[k];
    double centroids[k][dim];
    double oldcentroids[k][dim];
    for (int i=0; i<k; i++){
        for (int j=0;j<dim;j++ ){
            centroids[i][j]=data[i][j];
        }
    }

    int flag=0;
    while (flag!=1){
        
        Cluster( cnt, dim,data,k,centroids,cluster,clusterCNT);
        for(int i=0;i<=k;i++){
            for(int j=0;j<dim;j++){
                oldcentroids[i][j]=centroids[i][j];
            }
        }
        Newcentroids(cnt,dim,k, cluster,clusterCNT, data,centroids);
   
        flag= stop(cnt, dim,k,oldcentroids,centroids);
    }
        
    
    //output file
    FILE * fp2;
    fp2 = fopen(argv[3], "w");
    fprintf(fp2, "%d %d\n", cnt,dim+1);
    for(int i=0; i<cnt; i++)
    {
        for(int j=0;j<dim;j++){
            fprintf(fp2, "%lf ", data[i][j]);
        }
        fprintf(fp2, "%d\n", cluster[i]);
    }
    
    fclose(fp2);
    return 0;
 
}





