#include <stdio.h> /* for printf */
#include <stdlib.h> /* for exit */
#include <stdarg.h> /* for va_{ list , args ... } */
#include <unistd.h> /* for sleep */
#include <time.h> /* for elapsed time */
#include <math.h>

#include "main.decl.h"

#include "main.h"
#include "hello.decl.h"

/* readonly */ CProxy_Main mainProxy;

#define PI_REF 3.141592653589793238462643383279 // 30 digits

#define AUTOMATE_RUN 1

#define N_REF 1000000000

// Entry point of Charm++ application
Main::Main(CkArgMsg* msg) {

  id = CkMyRank();
  p = CkNumPes();

  // Display some info about this execution
  // for the user.
  CkPrintf("Running \"Hello World\" with %d elements "
           "using %d processors.\n",
           numElements, CkNumPes());

  // Set the mainProxy readonly to point to a
  // proxy for the Main chare object (this
  // chare object).
  mainProxy = thisProxy;

  // Create the array of Hello chare objects.
  CProxy_Hello helloArray = CProxy_Hello::ckNew(numElements);

  // Invoke the "sayHi()" entry method on all of the
  // elements in the helloArray array of chare objects.
  helloArray.sayHi(-1);
}

// Constructor needed for chare object migration (ignore for now)
// NOTE: This constructor does not need to appear in the ".ci" file
Main::Main(CkMigrateMessage* msg) { }

void print_result(double elapsedTime, double pi) {
  printf("Elapsed time:          %2f s\n", elapsedTime );
  printf("Result Pi:             %.30f\n", pi);
  printf("Error Approximation:   %.30f\n", fabs(pi - PI_REF));
}

void save_benchmark(int numprocs, double elapsedTime) {
  if (AUTOMATE_RUN) {
    printf("\n");
    printf("Auto-Save:\n");
    char command[100];
    snprintf (command, 100,"echo \"%d %f\" >> stats.dat", numprocs, elapsedTime);
    printf("Running Command: %s\n",command);
    system(command);
  }
}

void xprintf (char *format, ...) {
  va_list args ;
  va_start (args , format);
  printf ("[Node %i] ", id);
  vprintf (format, args);
  fflush (stdout);
}

double f(double x) {
  return 4.0 / ( 1.0 + (x * x) );
}

// When called, the "done()" entry method will increment the doneCount.
// Once all of the Hello chare objects have indiciated that they have
// completed their tasks, "done()" will cause the program to exit.
void Main::done() {
  // Increment the doneCount. If all of the Hello chare
  // objects have indicated that they are done, then exit.
  // Otherwise, continue waiting for the Hello chare objects.
  //doneCount++;
  //if (doneCount >= numElements)
  
  CkExit();
}

#include "main.def.h"