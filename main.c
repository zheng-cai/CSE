//
//  main.c
//  222
//
//  Created by 蔡政 on 9/3/19.
//  Copyright © 2019 蔡政. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "PrioQ.h"

int main(){
    /*create an empty priority queue*/
    printf("---creating an empty priority queue---\n");
    PrioQ *queue=PQ_creat();
    PQ_count(queue);
    PQ_print(queue);
    
    /*insert an item into the priority queue*/
    printf("---insert an item into the priority queue---\n");
    int a=5;
    PQ_insert(queue,5,&a);/*insert key=5 and data=5 to one node*/
    PQ_count(queue);
    PQ_print(queue);
    
    /*create a queue contains 5 items by insert function*/
    printf("---crerate a queue contains 5 items by inserting---\n");
    char b='g',c='m';
    double d=1.234,e=0.983;
    PQ_insert(queue,1,&b);
    PQ_insert(queue,7,&c);
    PQ_insert(queue,3,&d);
    PQ_insert(queue,9,&e);
    PQ_count(queue);
    PQ_print(queue);
    
    /*inserting at the beginning, middle and the end*/
    printf("--inserting at the beginning, middle and the end--\n");
    int f=8;
    PQ_insert(queue,0,&f); /*insert an item at the beginning with key=0 and data=8*/
    /*insert at the middle*/
    float g=7.89;
    PQ_insert(queue,4,&g);/*insert an item at the middle with key=4 and data=7.89*/
    /*insert at the end*/
    void* h;
    PQ_insert(queue,15,h);/*insert an item at the end with key=4 and data=random pointer*/
    PQ_count(queue);
    PQ_print(queue);
    
    /*remove all items in the priority queue*/
    printf("--remove all items in the priority queue--\n");
    PQ_free(queue);
    PQ_count(queue);
    PQ_print(queue);
    
    /*deleting an item from an empty queue (error case)*/
    printf("--deleting an item from an empty queuee--\n");
    double *j;
    PQ_delete(queue, j);
    PQ_count(queue);
    PQ_print(queue);
}
