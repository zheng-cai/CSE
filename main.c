//
//  main.c
//  555
//
//  Created by 蔡政 on 10/1/19.
//  Copyright © 2019 蔡政. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>


typedef struct Nodetype node;
struct Nodetype{
    int vertex;
    double link_weight;
    node *next;
};
  
/*function to generate new node*/
 node *createNode( int v){
     node *newnode=malloc(sizeof(node));
     newnode->vertex=v;
     newnode->next=NULL;
     newnode->link_weight=0;
     return newnode;
 }

/*add node to adjacency list and generate the random link weight*/
 void addNodes(int v,int u,node **adjList,double a[],int w1, int w2){
     double link_weight=(double)(rand()%((w2-w1)*100)+w1*100)/100;
     node *newnode=createNode(v);
     adjList[v]=newnode;
     adjList[v]->next=createNode(u);
     adjList[v]->next->link_weight=link_weight;
     node*current=adjList[u];
     node* temp =current;
     while(current!=NULL){
         temp=current;
         current=current->next;
     }
     temp->next=createNode(v);
     temp->next->link_weight=link_weight;
     a[u]++;
     a[v]++;
 }

/*add edge to create fully connected network*/
void addEdge(int v, int u,node **adjList,double a[],int w1,int w2){
    double link_weight=(double)(rand()%((w2-w1)*100)+w1*100)/100;
    node*u_current=adjList[u];
    node* temp =u_current;
    while(u_current!=NULL){
        temp=u_current;
        u_current=u_current->next;
        }
    temp->next=createNode(v);
    temp->next->link_weight=link_weight;
    node*v_current=adjList[v];
    temp =v_current;
    while(v_current!= NULL){
        temp=v_current;
        v_current=v_current->next;
        }
    temp->next=createNode(u);
    temp->next->link_weight=link_weight;
    a[u]++;
    a[v]++;
    
    }
/*calculate probability of being added another node*/
void prob( double p[],double a[],int N){
    double sum=0;
    for (int i=0;i<N;i++){
        sum=sum+a[i];
    }
    for (int j=0;j<N;j++){
        p[j]=a[j]/sum;
    }
}

/*transfer pdf to cdf*/
double addup(double p[],int n){
    double sum=0;
    for(int i=0;i<=n;i++){
        sum=sum+p[i];
    }
    return sum*100;
}

int main(int argc, char * argv[]) {
    int opt;
    char *h = NULL;
    char *o = "top.txt";
    while ((opt = getopt ((argc-3), &argv[3], "o:h:")) != EOF)
    {
        switch (opt){
            case 'h': h = optarg; break;
            case 'o': o = optarg; break;
                if (opt == 'h')
                {
                    fprintf(stderr, "Option -%c requires an argument. \n", optopt);
                }
                else
                {
                    fprintf(stderr, "Unknown Option Character \\x%x. \n", optopt);}
                return 0;
            default:
                break;
        }
    }
    
    srand(time(NULL));
    int N=atoi(argv[1]),max=0;
    int w1=atoi(argv[2]), w2=atoi(argv[3]);
    double a[N];
    double p[N];
    
    /*initializing probability and number of nodes each point linked to*/
    for (int i=0;i<N;i++){
        a[i]=0;
        p[i]=0;
    }
    node **adjList= malloc(N*sizeof(node*));
    for (int i=0;i<N;i++){
        adjList[i]=NULL;
    }
    node *firstNode=createNode(0);
    adjList[0]=firstNode;
    addNodes(1,0,adjList,a,w1,w2);
    addNodes(2,0,adjList,a,w1,w2);
    addEdge(2,1,adjList,a,w1,w2);
    prob(p,a,N);
    for (int i=3;i<N;i++){
        int random=rand()%100;
        for (int j=0;j<N;j++){
            if(addup(p,j)>random){
                addNodes(i,j,adjList,a,w1,w2);
                prob(p,a,N);
                break;
            }
    }
    }
    
    
    for (int i=0; i<N;i++){
        if (a[i]>max){
            max=a[i];
        }
    }
    int count[max+1];
    for (int i=0;i<=max;i++){
        count[i]=0;
    }
    /*count for each degree*/
   for (int i=0;i<=max;i++){
        for (int j=0;j<N;j++){
            if (a[j]==i){
                count[i]++;
            }
        }
    }
   
    
    /*output “histogram”*/
    FILE *fp;
       fp=fopen(h,"w");
       fprintf(fp,"i\tNum\tNode\t\n");
       for (int i=0;i<=max;i++){
           fprintf(fp,"%d\t%d\t",i,count[i]);
           fprintf(fp,"|");
           for (int j=0;j<N;j++)
               if (a[j]==i){
                   fprintf(fp,"%d,",adjList[j]->vertex);
               }
           fprintf(fp,"\n");
       }
    fclose(fp);
   
    /*output the node index,distance and the neighbour one  */
    fp=fopen(o,"w");
    fprintf(fp,"%d\n",N);
    for (int i=0;i<N;i++){
        node*current=adjList[i];
        current=current->next;
        while(current!=NULL){
                   fprintf(fp,"%d %d %lf ",i,current->vertex,current->link_weight);
                   current=current->next;
               }
        
    }
    fclose(fp);
    
    /*free pointer*/
    for (int i=0;i<N;i++){
        free(adjList[i]);
    }
}



