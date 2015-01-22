#include <stdio.h> /* for printf */
#include <stdlib.h> /* for exit */
#include <stdarg.h> /* for va_{ list , args ... } */
#include <unistd.h> /* for sleep */
#include <time.h> /* for elapsed time */
#include <math.h>

int id = 0; // MPI id for the current process ( set global to be used in xprintf )

#define PI_REF 3.141592653589793238462643383279 // 30 digits

#define AUTOMATE_RUN 1

#define N_REF 10000000

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

double f(double x) {
  return 4.0 / ( 1.0 + (x * x) );
}

void xprintf (char *format, ...) {
  va_list args ;
  va_start (args , format);
  printf ("[Node %i] ", id);
  vprintf (format, args);
  fflush (stdout);
}

int main(int argc , char *argv []) {
  int p; // number of processors
  double elapsedTime = 0.0;

  id = omp_get_thread_num();
  p = omp_get_num_threads();

  int i; 
  double x, pi = 0.0;
  
  clock_t t1 = clock();
  
  double a = 1.0 / ( 2.0 * (double)N_REF );
  double sum = 0.0;

  #pragma omp parallel for reduction(+:sum)
  for (i = 0; i < N_REF; i++) {
    sum += f( i/(double)N_REF ) + f( (i+1.0)/(double)N_REF );
  }
  pi = a * sum;
  
  clock_t t2 = clock();
  elapsedTime = (double)(t2 - t1) / CLOCKS_PER_SEC;

  if ( id == 0 ) {
    print_result(elapsedTime, pi);
    save_benchmark(xmp_all_num_nodes(), elapsedTime);
  }

  return 0;
}