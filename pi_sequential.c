#include <stdio.h> /* for printf */
#include <stdlib.h> /* for exit */
#include <stdarg.h> /* for va_{ list , args ... } */
#include <unistd.h> /* for sleep */
#include <math.h>

int id = 0; // MPI id for the current process ( set global to be used in xprintf )

#define PI_REF 3.141592653589793238462643383279 // 30 digits

#define AUTOMATE_RUN 0

void print_result(int numprocs, double elapsedTime, double pi) {
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
  printf ("[Node %i] ", my_id);
  vprintf (format, args);
  fflush (stdout);
}

double f(double x) {
  return 4.0 / ( 1.0 + (x * x) );
}

int main(int argc , char *argv []) {
  int p; // number of processors
  unsigned int n = 0;
  double elapsed_time = 0.0;
  
  printf ("Input n = ");
  scanf("%u", &n);

  clock_t t1 = clock();
  
  // Compute Pi
  double a = 1.0 / ( 2.0 * (double)n );
  double sum, i = 0.0;
  for(i = 0.0; i < n; i++) {
    sum += f( i/(double)n ) + f( (i+1.0)/(double)n );
  }
  double pi = a * sum;

  clock_t t2 = clock();
  elapsedTime = (double)(t2 - t1) / CLOCKS_PER_SEC;
  
  print_result(p, elapsedTime, pi);
  
  return 0;
}
