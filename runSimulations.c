#include"runSimulations.h"

int main(int argc, char **argv){
   /*Error handling for the command line inputs*/
   if(argc < 5){
      FILE *fp;
      if ((fp = fopen ("errorFile.txt", "w+")) != NULL){
         fprintf(fp, "Error, not enough inputs to start\n");
         fclose(fp);
      }   
      return 1;
   }
   /*GSL random numbers set up*/
   gsl_rng_env_setup();
   T = gsl_rng_default;
   r = gsl_rng_alloc(T);
   gsl_rng_set(r,time(0));

   /*Converts the command line input into its interger value or 0*/
   int arg1 = atoi(argv[1]);
   int arg2 = atoi(argv[2]);
   int arg3 = atoi(argv[3]);
   int arg4 = atoi(argv[4]);

   /*Begins the simulations with the parameters given*/
   runSimulation(arg1,arg2,arg3,arg4);

   return 0;

}

/*Uses GSL to output a uniform random number*/
int randomNumberGenerator(int max)
{
   return (int)gsl_ran_flat(r,0,max);
}


struct Tuple runOneSimulation(int leftTrafficArrivalRate, int rightTrafficArrivalRate, int timeGreenLeft, int timeGreenRight){

   /*Sets up all the variables needed to keep track of the information for the simulation*/
   int i;
   int trafficInLeft = 0;
   QUEUE* LeftCars = NULL;
   int trafficInRight = 0;
   QUEUE* RightCars = NULL;
   int leftPassed = 0;
   int rightPassed = 0;
   int lightLeftOn = 0;
   int totalWaitTimeLeft = 0;
   int totalWaitTimeRight = 0;
   int maxWaitTimeRight = 0;
   int maxWaitTimeLeft = 0;
   int currentTimeInGreen = 0;

   /*For loop for out 500 time units where cars can spawn and move throught the lights etc*/
   for(i = 0; i <500;i++){

      /*Checks if this is a run where they need to change the light*/
      currentTimeInGreen++;
      if(lightLeftOn && currentTimeInGreen == timeGreenLeft){
         currentTimeInGreen = -1;
         lightLeftOn = 0;
      } 
      else if(!lightLeftOn && currentTimeInGreen == timeGreenRight){
         currentTimeInGreen = -1;
         lightLeftOn = 1;
      }
      /*If they don't change the lights this time then we are going to instead allow traffic to move and spawn*/
      else{
          
         /*Randomly decides if they should spawn a car on the left*/
         int randomLeft = randomNumberGenerator(60);
         if(randomLeft < leftTrafficArrivalRate){
            trafficInLeft++;
            LeftCars = push(LeftCars,i);
         }
         /*Randomly decides if they should spawn a car on the right*/
         int randomRight = randomNumberGenerator(60);
         if(randomRight < rightTrafficArrivalRate){
            trafficInRight++;
            RightCars = push(RightCars,i);
         }

         /*Checks which side can move the cars and moves one over to the other side on the side that is green*/
         if(lightLeftOn == 0){
            /*Making sure there is traffic to move*/
            if(trafficInRight){ 

               trafficInRight--;
               int temp = RightCars ->data;
               RightCars = pop(RightCars);
               if(i-temp>maxWaitTimeRight){
                  maxWaitTimeRight = i-temp;
               }
               totalWaitTimeRight = totalWaitTimeRight + (i - temp);

               rightPassed++;
               
            }

         }
         else{

            if(trafficInLeft){
               /*Making sure there is traffic to move*/
               trafficInLeft--;
               int temp = LeftCars ->data;
               LeftCars = pop(LeftCars);
               if(i-temp>maxWaitTimeLeft){
                  maxWaitTimeLeft = i-temp;
               }
               totalWaitTimeLeft = totalWaitTimeLeft + (i - temp);

               leftPassed++;
            }
         }
      }
   }

   /*Once the 500 rounds are done we need to clear the queue*/
   int tempBool = 0;
   int timeForRight = 0;
   int timeForLeft = 0;
   while(!tempBool){
      i++;
      /*Checks if the termination conditions are true*/
      if(trafficInLeft == 0){
         if(trafficInRight == 0){
            tempBool = 1;
         }
         /*If the left is clear but the right isn't*/
         else{
            timeForRight++;
            trafficInRight--;
            int temp = RightCars ->data;
            RightCars = pop(RightCars);
            if(i-temp>maxWaitTimeRight){
                 maxWaitTimeRight = i-temp;
            }
            totalWaitTimeRight = totalWaitTimeRight + (i - temp);
            rightPassed++;
         }
      }
      /*If the right is clear but the left isn't*/
      else if (trafficInRight == 0){
         timeForLeft++;
         trafficInLeft--;
         int temp = LeftCars ->data;
         LeftCars = pop(LeftCars);
         if(i-temp>maxWaitTimeLeft){
            maxWaitTimeLeft = i-temp;
         }
         totalWaitTimeLeft = totalWaitTimeLeft + (i - temp);
         leftPassed++;
      }
      /*If neither side is clear*/
      else{
         timeForLeft++;
         timeForRight++;
         currentTimeInGreen++;
         if(lightLeftOn && currentTimeInGreen == timeGreenLeft){
            currentTimeInGreen = 0;
             lightLeftOn = 0;
         } 
         else if(!lightLeftOn && currentTimeInGreen == timeGreenRight){
            currentTimeInGreen = 0;
            lightLeftOn = 1;
         }
         else{
            if(lightLeftOn == 0){
               if(trafficInRight){ 
                  trafficInRight--;
                  int temp = RightCars ->data;
                  RightCars = pop(RightCars);
                  if(i-temp>maxWaitTimeRight){
                     maxWaitTimeRight = i-temp;
                  }
                  totalWaitTimeRight = totalWaitTimeRight + (i - temp);
                  rightPassed++;
              }
           }
           else{
              if(trafficInLeft){
                 trafficInLeft--;
                 int temp = LeftCars ->data;
                 LeftCars = pop(LeftCars);
                 if(i-temp>maxWaitTimeLeft){
                    maxWaitTimeLeft = i-temp;
                 }
                 totalWaitTimeLeft = totalWaitTimeLeft + (i - temp);
                 leftPassed++;
               }
            }
         }
      }
   }
   /*Formats all the data collected ready to be returned*/       
   TUPLE returnTemp;
   returnTemp.leftPassed  = leftPassed;
   returnTemp.rightPassed = rightPassed;
   returnTemp.averageWaitTimeRight = totalWaitTimeRight/500;
   returnTemp.averageWaitTimeLeft = totalWaitTimeLeft/500;
   returnTemp.maxWaitTimeRight = maxWaitTimeRight;
   returnTemp.maxWaitTimeLeft = maxWaitTimeLeft;
   returnTemp.timeForLeft = timeForLeft;
   returnTemp.timeForRight = timeForRight;
   return returnTemp;
}

void runSimulation(int  leftTrafficArrivalRate, int  rightTrafficArrivalRate, int  greenTimeLeft, int  greenTimeRight){
   int i;
   /*Varaibles to hold all of the information collected by the simulations, long ints as they will get quite large numbers for some*/
   long int leftPassed = 0;
   long int rightPassed = 0;
   long int totalAverageWaitTimeLeft = 0;
   long int totalAverageWaitTimeRight = 0;
   long int maxWaitTimeRight = 0;
   long int maxWaitTimeLeft = 0;
   long int timeForLeft = 0;
   long int timeForRight = 0;
   for(i = 0; i<100; i++){
      TUPLE outputTuple = runOneSimulation( leftTrafficArrivalRate, rightTrafficArrivalRate, greenTimeLeft, greenTimeRight);
      /*Counts the totals of all the data we have collected*/
      leftPassed =  leftPassed + outputTuple.leftPassed;
      rightPassed = rightPassed + outputTuple.rightPassed;
      totalAverageWaitTimeLeft = totalAverageWaitTimeLeft + outputTuple.averageWaitTimeLeft;
      totalAverageWaitTimeRight = totalAverageWaitTimeRight + outputTuple.averageWaitTimeRight;
      maxWaitTimeRight = maxWaitTimeRight + outputTuple.maxWaitTimeRight;
      maxWaitTimeLeft = maxWaitTimeLeft + outputTuple.maxWaitTimeLeft;
      timeForLeft = timeForLeft + outputTuple.timeForLeft;
      timeForRight = timeForRight + outputTuple.timeForRight;
   }



   /*Outputs all the results by mean average*/
   printf("Parameter values :\n");
   printf("   from left :\n      traffic arrival rate: %03d\n      traffic light period : %03d\n",  leftTrafficArrivalRate,  greenTimeLeft);
   printf("   from right :\n      traffic arrival rate: %03d\n      traffic light period : %03d\n",  rightTrafficArrivalRate,  greenTimeRight);
   printf("Results (averaged over 100 runs) :\n");
   printf("   from left :\n      number of vehicles : %03ld\n      average waiting time : %03ld\n      maximum waiting time : %03ld\n      clearance time : %ld\n", leftPassed/100, totalAverageWaitTimeLeft/100, maxWaitTimeLeft/100, timeForLeft/100);
   printf("   from right :\n      number of vehicles : %03ld\n      average waiting time : %03ld\n      maximum waiting time : %03ld\n      clearance time : %ld\n", rightPassed/100, totalAverageWaitTimeRight/100, maxWaitTimeRight/100, timeForRight/100);
}


/*Queue functions*/
QUEUE *newNode (int data){
   QUEUE *temp;
   if(!(temp = (QUEUE*) malloc(sizeof(QUEUE))))
   {
      FILE *fp;
      if ((fp = fopen ("file.txt", "w+")) != NULL){
         fprintf(fp, "Out of memory\n");
         fclose(fp);
      } 
       
      exit(1);
   }
   temp->data = data;
   temp->next = NULL;
   return temp;
}
extern QUEUE* push(QUEUE* queue, int data){ /*Push to the end*/
   QUEUE* temp = queue;
   if(temp != NULL){
      while(temp -> next!= NULL){
            temp = temp -> next;
      }
      QUEUE *nextNode = newNode(data);
      temp->next = nextNode;

   }
   else{
      queue = newNode(data);
   }
   return queue;
   
}
extern QUEUE* pop(QUEUE* queue){

   QUEUE* temp = queue;
   int tempData = 0;
   tempData = temp -> data;
   queue = (temp -> next);
   free(temp);
   return queue;

}
