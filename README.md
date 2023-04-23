# ECM2433
C programs to simulate traffic light system

Read Me

Welcome to the traffic light simulator for ECM2433!

To compile and link the program you can use the compileSim shell script. By running

bash compileSim.sh runSimulations

You can from here run the code using 

./runSimulations a b c d

Where a,b,c,d are integers.

In the event that they are not integers we will use 0 instead. Or where  0 > a,b or a,b < 60  where we will get the same results as 0 / 60. If not enough variables are entered the code will end before the simulations start. (60 is 100% chance)

Other documentation can be found in the report and you can alternatively manually compile and link the code using,

gcc -ansi -c runSimulations.c -o runSimulations.o

Followed by,

gcc runSimulations.o -lgsl -lgslcblas -o runSimulations

And then running the code.
