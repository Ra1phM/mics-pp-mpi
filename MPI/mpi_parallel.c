#include <stdio.h> /* for printf */
#include <stdlib.h> /* for exit */
#include <stdarg.h> /* for va_{ list , args ... } */
#include <unistd.h> /* for sleep */
#include <math.h>
#include <mpi.h>

int id = 0;

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

int main(int argc , char *argv []) {
  int p; // number of processors
  double elapsedTime = 0.0;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  int i; 
  double x, pi, pi_contribution = 0.0;
  
  MPI_Barrier(MPI_COMM_WORLD);
  elapsedTime = -MPI_Wtime();
  
  double a = 1.0 / ( 2.0 * (double)N_REF );
  double sum = 0.0;
  for (i = id; i < N_REF; i += p) {
    sum += f( i/(double)N_REF ) + f( (i+1.0)/(double)N_REF );
  }
  pi_contribution = a * sum;
  
  MPI_Reduce(&pi_contribution, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  
  elapsedTime += MPI_Wtime();

  if ( id == 0 ) {
    print_result(elapsedTime, pi);
    save_benchmark(p, elapsedTime, pi);
  }

  MPI_Finalize();
  return 0;
}
