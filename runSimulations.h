#include<stdio.h>
#include<stdlib.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>
#include<time.h>
#include<string.h>

struct Tuple{
   int leftPassed;
   int rightPassed;
   int averageWaitTimeLeft;
   int averageWaitTimeRight;
   int maxWaitTimeRight;
   int maxWaitTimeLeft;
   int timeForLeft;
   int timeForRight;

};
typedef struct Tuple TUPLE;



/*Struct def*/

struct queueStruct{
   int data;
   struct queueStruct *next;
};
typedef struct queueStruct QUEUE;

/*Funct def*/

struct Tuple runOneSimulation(int,int, int, int);
void runSimulation(int,int,int,int);
int randomNumberGenerator(int);
QUEUE* newNode (int);
const gsl_rng_type *T;
const gsl_rng *r;

extern QUEUE* push(QUEUE* queue, int data);
extern QUEUE* pop(QUEUE* queue);
