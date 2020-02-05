//
//  Q.h
//  222
//
//  Created by 蔡政 on 9/1/19.
//  Copyright © 2019 蔡政. All rights reserved.
//

#ifndef PrioQ_h
#define PrioQ_h
#include <stdio.h>

typedef struct PrioQtype PrioQ;
struct PrioQtype{
    double key;
    void * data;
    PrioQ *next;
};

/*create a head pointer for priority queue*/
PrioQ *PQ_creat(){
    PrioQ *PQ=(PrioQ*)malloc(sizeof(PrioQ));
    PQ->next=NULL;              /*let the tail points to NULL*/
    if (PQ==NULL){              /*if there is error on allocating, indicate it and return NULL*/
        printf("Error in allocating");
        return NULL;
    }
    return PQ;
}

/*insert an new node and sort by priority*/
int PQ_insert(PrioQ *PQ, double key, void* data){
    PrioQ *current=PQ;
    PrioQ *insert=(PrioQ*)malloc(sizeof(PrioQ));
    if (insert==NULL){        /*allocating failure case*/
        return NULL;}
    insert->key=key;
    insert->data=data;
    if (current->next==NULL)  /*the case of empty queue*/
    {
        current->next=insert;
        insert->next=NULL;
        return 0;
    }
    while (current->next != NULL && key>current->next->key){  /*use loop to insert by checking key*/
        current=current->next;
    }
    
    insert->next=current->next;
    current->next=insert;
    
    return 0;
}

/*delete the highest priority node*/
void *PQ_delete(PrioQ *PQ, double *key){
    PrioQ *delete,*temp=PQ;
    if (temp->next!=NULL)
    {
        delete=temp->next;
        temp=delete->next;
        *key=delete->key;
        free(delete);
        PQ->next=temp;
    }else if( temp->next==NULL){  /*empty queue case*/
        printf("This queue is already empty\n");
        return NULL;
    }
    return PQ;
    
}

/*count nodes in a priority queue*/
unsigned int PQ_count(PrioQ *PQ){
    int i=1;
    PrioQ *temp;
    temp=PQ->next;
    if (temp==NULL){   /*empty queue case*/
        i=0;
    }
    else{
        while ( temp->next!=NULL){
            i=i+1;
            temp=temp->next;
        }
    }
    return printf("the count is %d\n",i);
}

/*print all keys in priority queue by oreder*/
void PQ_print(PrioQ *PQ){
    PrioQ *temp=PQ;
    if (temp->next==NULL){  /*empty queue*/
        printf("null\n");
    }else{
        temp=temp->next;
        printf("the key is:\n");
        while (temp!= NULL){
            printf("%lf\n",temp->key);
            temp=temp->next;
        }
    }
}

/*delete all items one by one and release all memory*/
void *PQ_free(PrioQ *PQ){
    PrioQ *temp,*current;
    current=PQ->next;
    while (current!= NULL){
        temp=current->next;
        free(current);
        current=temp;
    }
    PQ->next=NULL;
    return PQ;
}
#endif
