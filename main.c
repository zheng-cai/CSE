//
//  main.c
//  666new
//
//  Created by 蔡政 on 10/31/19.
//  Copyright © 2019 蔡政. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#define MAX 100



//CompontID
#define     GENERATOR       0
#define     QUEUE_STATION   1
#define     EXIT            2

//EventID
#define     GENERATE        3
#define     ARRIVAL         4
#define     DEPARTURE       5

double min_totaltime=INFINITY;
double max_totaltime=0;
double avg_totaltime=0;
double min_waitingtime=INFINITY;;
double max_waitingtime=0;
double avg_waitingtime=0;
int exit_cust=0;
int enter_cust=0;
/*generating uniformly distributed number between[0,1)*/

double urand(void){
    double random=rand()*1.0/RAND_MAX;;
    return random;
}

typedef struct ProbabilityType Prob;
struct ProbabilityType{
    double prob;
    int id;
    Prob *next;
};

/*generate random numbers from an exponential distribution with mean U*/
double randexp(double U){
    
    double random= -U*(log(1.0-urand()));
    return random;
}

typedef struct Customer customer;
struct Customer{
    double creationtime;
    double servingtime;
    double waitingtime;
    double enterlinetime;
    double total_waitingtime;
    customer * next;
};

typedef struct FIFO{
    customer *first;     // pointer to first customer in queue
    customer *last;      // pointer to last customer in queue
    double servingtime;
    double waitingtime;
    Prob *probability;
    int customer_count;
    
}queue;

typedef struct Generator{
    double InterTime;
    int destination;
}generator;


typedef struct EventType{
    int Event; //arrival, departure and generate
    int id; //component's id
    customer *cust;
}event;





struct {
    void *p;
    int componenttype;
}component[MAX];


struct Event {
    double timestamp;        // event timestamp
    void *AppData;            // pointer to application defined event parameters
    struct Event *Next;        // priority queue pointer
};

void EventHandler(event *e);
// Simulation clock variable
double Now = 0.0;

// Future Event List
// Use an event structure as the header for the future event list (priority queue)
// Using an event struct as the header for the priority queue simplifies the code for
// inserting/removing events by eliminating the need to explicitly code special cases
// such as inserting into an empty priority queue, or removing the last event in the priority queue.
// See the Remove() and Schedule() functions below.
struct Event FEL ={-1.0, NULL, NULL};

/////////////////////////////////////////////////////////////////////////////////////////////
// Prototypes for functions used within the Simulation Engine
/////////////////////////////////////////////////////////////////////////////////////////////

// Function to print timestamps of events in event list
void PrintList (void);

// Function to remove smallest timestamped event
struct Event *Remove (void);

/////////////////////////////////////////////////////////////////////////////////////////////
// Simulation Engine Functions Internal to this module
/////////////////////////////////////////////////////////////////////////////////////////////

// Remove smallest timestamped event from FEL, return pointer to this event
// return NULL if FEL is empty
struct Event *Remove (void)
{
    struct Event *e;

    if (FEL.Next==NULL) return (NULL);
    e = FEL.Next;        // remove first event in list
    FEL.Next = e->Next;
    return (e);
}

// Print timestamps of all events in the event list (used for debugging)
void PrintList (void)
{
    struct Event *p;

    printf ("Event List: ");
    for (p=FEL.Next; p!=NULL; p=p->Next) {
        printf ("%f ", p->timestamp);
    }
    printf ("\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Simulation Engine functions visible to simulation application
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Note the strategy used for dynamic memory allocation. The simulation engine is responsible
// for freeing any memory it allocates, and is ONLY responsible for freeing memory allocated
// within the simulation engine. Here, the simulation dynamically allocates memory
// for each event put into the event list. The Schedule function allocates this memory.
// This memory is released after the event is processed (in RunSim), i.e., after the event handler
// for the event has been called and completes execution.
// Because we know each event is scheduled exactly once, and is processed exactly once, we know that
// memory dynamically allocated (using malloc) for each event will be released exactly once (using free).
// Similarly, the simulation application (not shown here) is responsible for reclaiming all memory
// it dynamically allocates, but does not release any memory allocated by the simulation engine.
//

// Return current simulation time
double CurrentTime (void)
{
    return (Now);
}

// Schedule new event in FEL
// queue is implemented as a timestamp ordered linear list

void Schedule (double ts, void *data)
{
    struct Event *e, *p, *q;

    // create event data structure and fill it in
    if ((e = malloc (sizeof (struct Event))) == NULL) exit(1);
    e->timestamp = ts;
    e->AppData = data;

    // insert into priority queue
    // p is lead pointer, q is trailer
    for (q=&FEL, p=FEL.Next; p!=NULL; p=p->Next, q=q->Next) {
        if (p->timestamp >= e->timestamp) break;
        }
    // insert after q (before p)
    e->Next = q->Next;
    q->Next = e;
}

// Function to execute simulation up to a specified time (EndTime)
void RunSim (double EndTime)
{
    struct Event *e;

    printf ("Initial event list:\n");
    PrintList ();

    // Main scheduler loop
    while ((e=Remove()) != NULL) {
        Now = e->timestamp;
        if (Now > EndTime) break;
        EventHandler(e->AppData);
        free (e);    // it is up to the event handler to free memory for parameters
        PrintList ();
    }
}


void BuildGenerator(int id, float InterTime, int destination){
    generator* g;
    event* e;
    printf("Generator, ID:%d, Interarrival Time：%lf, Destination%d\n",id,InterTime,destination);
    if ((g=malloc (sizeof(generator))) == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }
    if ((e=malloc (sizeof(event))) == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }
    g->InterTime=InterTime;
    g->destination=destination;
    component[id].componenttype=GENERATOR;
    component[id].p=g;
    e->Event=GENERATE;
    e->id=id;
    e->cust=NULL;
    Schedule(randexp(InterTime),e);
     
}

void BuildQueueStation(int id,double servingtime, Prob*probability){
    queue*q;
    printf ("Queue Station, ID=%d, Average Serving Time: %f, Destination: ",id,servingtime);
    Prob *temp=probability;
    while (temp!=NULL){
        printf("%d ",temp->id);
        temp=temp->next;
    }
    if ((q=malloc (sizeof(queue))) == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);}
    component[id].componenttype=QUEUE_STATION;
    q->first=NULL;
    q->last=NULL;
    q->servingtime=servingtime;
    q->probability=probability;
    q->customer_count=0;
    q->waitingtime=0;
    component[id].p=q;
}


void BuildExit(int id){
    component[id].componenttype=EXIT;
}

void generate(event*e){
    customer * c;
    event *temp_e;
    double timestamp;
    event *temp_e2;
    
    if ((c=malloc (sizeof(customer))) == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);}
    
    if ((temp_e=malloc (sizeof(event))) == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }
    
    if ((temp_e2=malloc (sizeof(event))) == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }
    
    if (e->Event != GENERATE) {
        fprintf (stderr, "event error\n");
        exit(1);
    }
    if (component[e->id].componenttype != GENERATOR) {
        fprintf(stderr, "componenet error\n");
        exit(1);
    }
    generator *cmp=(generator*)component[e->id].p;
    

    // create a new customer
    enter_cust++;
    c->creationtime=CurrentTime();
    c->servingtime=0;
    c->waitingtime=0;
    c->enterlinetime=0;
    c->total_waitingtime=0;
    c->next=NULL;
    // arrange the event of the new customer
    temp_e->id=cmp->destination;
    temp_e->cust=c;
    temp_e->Event=ARRIVAL;
    timestamp=CurrentTime();
    Schedule(timestamp, temp_e);
    
    if ((temp_e=malloc (sizeof(event))) == NULL) {
            fprintf(stderr, "malloc error\n");
            exit(1);
        }
    //set parameters for next generation event
    temp_e->Event=GENERATE;
    temp_e->id=e->id;
    timestamp=CurrentTime() +randexp(cmp->InterTime);
    Schedule(timestamp,temp_e);
    
}

void arrival(event*e){
    event *temp_e;
    double timestamp;
    
    if ((temp_e=malloc (sizeof(event))) == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }
    if (e->Event != ARRIVAL) {
        fprintf (stderr, "event error\n");
        exit(1);
    }
    if (component[e->id].componenttype==QUEUE_STATION ){
        queue *temp_q=(queue*)component[e->id].p;
        if (temp_q->first==NULL){
            temp_q->first=e->cust;
            temp_q->last=temp_q->first;
            temp_e->Event=DEPARTURE;
            temp_e->id=e->id;
            temp_e->cust=e->cust;
            timestamp=CurrentTime()+randexp(temp_q->servingtime);
            Schedule(timestamp,temp_e);
        }
        else{
            temp_q->last->next=e->cust;
            temp_q->last=e->cust;
            e->cust->enterlinetime=CurrentTime();
        }
    }
    else if(component[e->id].componenttype==EXIT){
        exit_cust++;
        customer *cus=e->cust;
        double totaltime=CurrentTime()- cus->creationtime;
        double total_waitingtime=e->cust->total_waitingtime;
        avg_totaltime=(avg_totaltime*(exit_cust-1)+totaltime)/exit_cust;
        avg_waitingtime=(avg_waitingtime*(exit_cust-1)+total_waitingtime)/exit_cust;
        if (totaltime<min_totaltime){
            min_totaltime=totaltime;
            printf("%lf\n",min_totaltime);
        }
        if (totaltime>max_totaltime){
            max_totaltime=totaltime;
            printf("%lf\n",max_totaltime);

        }
        if(total_waitingtime<min_waitingtime){
            min_waitingtime=total_waitingtime;
        }
        if(total_waitingtime>max_waitingtime){
            max_waitingtime=total_waitingtime;
        }
    }
    free(e);
}

void departure(event *e){
    double id=e->id;
    double timestamp;
    event *e2;
    event *e_next;
    if ((e2=malloc (sizeof(event))) == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
    }
    if ((e_next=malloc (sizeof(event))) == NULL) {
    fprintf(stderr, "malloc error\n");
    exit(1);
    }
    if (e->Event != DEPARTURE) {
         fprintf (stderr, "event error\n");
         exit(1);
     }
    if (component[e->id].componenttype==QUEUE_STATION){
        //pop the first customer from the queue
        queue *cmp=(queue*)component[e->id].p;
        cmp->customer_count++;
        cmp->first=cmp->first->next;
        //arrange the state of the element to arrive next compomnent
        Prob *temp_p=cmp->probability;
        double cumulative=0;
        int r=rand()%100;
        while(temp_p!= NULL){
            cumulative=cumulative+temp_p->prob;
            if (r<cumulative){
                e_next->id=temp_p->id;
                break;
            }
            temp_p=temp_p->next;
        }
        customer *temp=e->cust;
        e_next->cust=temp;
        e_next->Event=ARRIVAL;
        temp->next=NULL;
        timestamp=CurrentTime();
        Schedule(timestamp, e_next);
        //let next customer be ready to departure
        if (cmp->first!=NULL){
            e2->cust=cmp->first;
            e2->Event=DEPARTURE;
            e2->id=id;
            e2->cust=cmp->first;
            e2->cust->waitingtime=CurrentTime()-e2->cust->enterlinetime;
            e2->cust->servingtime=randexp(cmp->servingtime);
            e2->cust->total_waitingtime=e2->cust->total_waitingtime+e2->cust->waitingtime;
            cmp->waitingtime=(cmp->waitingtime*(cmp->customer_count-1)+e2->cust->waitingtime)/cmp->customer_count;
            timestamp=CurrentTime()+cmp->servingtime;
            Schedule(timestamp,e2);
        }
    
}
}
void EventHandler(event *e){
    switch(e->Event){
            case GENERATE:generate(e);
            break;
            case ARRIVAL: arrival(e);
            break;
            case DEPARTURE: departure(e);
            break;
            default:fprintf (stderr, "Illegal event found\n"); exit(1);
    }
    
        

        // coerce type so the compiler knows the type of information pointed to by the parameter data.
        // call an event handler based on the type of event
        
}

int getenteredCustomers(){
    return enter_cust;
 //   return enteredcustomer;
}
int getexitedCustomers(){
  //  return exitedcustomer;
    return exit_cust;
}
//static 2.
double getminimumtime(){
    return min_totaltime;
}
double getmaximumtime(){
    return max_totaltime;
}
double getavgtime(){
    return avg_totaltime;
}
//static 3.
double getminimumtimeinQS(){
    return min_waitingtime;
}
double getmaximumtimeinQS(){
    return max_waitingtime;
}
double getavgtimeinQS(){
    return avg_waitingtime;
}
// static 4.


Prob *ConstructProb(double prob,int id ) {
    Prob *queue = (Prob*) malloc(sizeof (Prob));
    if (queue == NULL) {
        return NULL;
    }

    queue->prob = prob;
    queue->id = id;
    queue->next = NULL;
    return queue;
}


int Enqueue_prob(Prob *pQueue,  double prob, int id) {
    /* Bad parameter */
    if ((pQueue == NULL)) {
        return 0;
    }

        pQueue->next->prob = prob;
        pQueue->id = id;
    return 1;
}


int main(int argc, const char * argv[]) {
    srand((unsigned)time(NULL));
        //int id[]={3,4,5,1};
        //double possibility[]={0.4,0.4,0.1,0.1};
        BuildGenerator(0,15.0,1);
       
    //receive a tray
        Prob *q1;
        q1=malloc (sizeof(Prob));
        q1->next=malloc (sizeof(Prob));
        q1->next->next=malloc (sizeof(Prob));
        q1->next->next->next=malloc (sizeof(Prob));
        q1->prob=20;
        q1->id=2;
        q1->next->id=3;
        q1->next->prob=40;
        q1->next->next->id=4;
        q1->next->next->prob=20;
        q1->next->next->next->id=5;
        q1->next->next->next->prob=20;
        q1->next->next->next->next=NULL;
        BuildQueueStation(1,6,q1);
//A Station
        Prob *q2;
        q2=malloc (sizeof(Prob));
        q2->next=malloc (sizeof(Prob));
        q2->prob=50;
        q2->id=6;
        q2->next->id=7;
        q2->next->prob=50;
        q2->next->next=NULL;
        BuildQueueStation(2,192,q2);
 //B Station
        Prob *q3;
        q3=malloc (sizeof(Prob));
        q3->next=malloc (sizeof(Prob));
        q3->prob=50;
        q3->id=6;
        q3->next->id=7;
        q3->next->prob=50;
        q3->next->next=NULL;
        BuildQueueStation(3,102,q3);
    //C Station
        Prob *q4;
        q4=malloc (sizeof(Prob));
        q4->next=malloc (sizeof(Prob));
        q4->prob=50;
        q4->id=6;
        q4->next->id=7;
        q4->next->prob=50;
        q4->next->next=NULL;
        BuildQueueStation(4,198,q4);
    
    //D Station
        Prob *q5;
        q5=malloc (sizeof(Prob));
        q5->next=malloc (sizeof(Prob));
        q5->prob=50;
        q5->id=6;
        q5->next->id=7;
        q5->next->prob=50;
        q5->next->next=NULL;
        BuildQueueStation(5,168,q5);
    
        //beverage bar 1
        Prob *q6;
        q6=malloc (sizeof(Prob));
        q6->next=malloc (sizeof(Prob));
        q6->prob=50;
        q6->id=8;
        q6->next->id=9;
        q6->next->prob=50;
        q6->next->next=NULL;
        BuildQueueStation(6,30,q6);
        //beverage bar 2
        Prob *q7;
        q7=malloc (sizeof(Prob));
        q7->next=malloc (sizeof(Prob));
        q7->prob=50;
        q7->id=8;
        q7->next->id=9;
        q7->next->prob=50;
        q7->next->next=NULL;
        BuildQueueStation(7,30,q7);
        //checkout queue 1
        Prob *q8;
        q8=malloc (sizeof(Prob));
        q8->next=malloc (sizeof(Prob));
        q8->next->next=malloc (sizeof(Prob));
        q8->next->next->next=malloc (sizeof(Prob));
        q8->next->next->next->next=malloc (sizeof(Prob));
        q8->prob=95;
        q8->id=10;
        q8->next->id=2;
        q8->next->prob=1.25;
        q8->next->next->id=3;
        q8->next->next->prob=1.25;
        q8->next->next->next->id=4;
        q8->next->next->next->prob=1.25;
        q8->next->next->next->next->id=5;
        q8->next->next->next->next->prob=1.25;
        q8->next->next->next->next->next=NULL;
        BuildQueueStation(8,60,q8);
        //checkout queue 2
        Prob *q9;
        q9=malloc (sizeof(Prob));
        q9->next=malloc (sizeof(Prob));
        q9->next->next=malloc (sizeof(Prob));
        q9->next->next->next=malloc (sizeof(Prob));
        q9->next->next->next->next=malloc (sizeof(Prob));
        q9->prob=95;
        q9->id=10;
        q9->next->id=2;
        q9->next->prob=1.25;
        q9->next->next->id=3;
        q9->next->next->prob=1.25;
        q9->next->next->next->id=4;
        q9->next->next->next->prob=1.25;
        q9->next->next->next->next->id=5;
        q9->next->next->next->next->prob=1.25;
        q9->next->next->next->next->next=NULL;
        BuildQueueStation(9,60,q9);
           
          
    
        //BuildQueueStation(3,7.0,1);

        

        BuildExit(10);
    
        RunSim(3600);
        if(exit_cust==0){
            printf("no customer exit from the system\n");
        }
        else{
        printf("number of entered customers:%d\n",getenteredCustomers());
        printf("number of exited customers:%d\n",getexitedCustomers());
        printf("minimum time of a customer stay in the system:%f\n",getminimumtime());
        printf("maximum time of a customer stay in the system:%f\n",getmaximumtime());
        printf("avg time of a customer stay in the system:%f\n",getavgtime());
        printf("minimum time of a customer stay in all the QS:%f\n",getminimumtimeinQS());
        printf("maximum time of a customer stay in all the QS:%f\n",getmaximumtimeinQS());
        printf("avg time of a customer stay in all the QS:%f\n",getavgtimeinQS());
        }
        for (int i=0;i<MAX;i++){
            if (component[i].componenttype==QUEUE_STATION){
                queue* q =(queue *)component[i].p;
                if (q->customer_count==0){
                    printf("Nobody entered No%d queue station\n",i);
                    continue;
                }
                printf("avg waiting time experienced by a customer in QS%d:%lf\n",i,q->waitingtime);
                
                
            }
                
        }
        
}

