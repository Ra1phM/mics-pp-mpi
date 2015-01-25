#include <stdio.h> /* for printf */
#include <stdlib.h> /* for exit */
#include <stdarg.h> /* for va_{ list , args ... } */
#include <unistd.h> /* for sleep */
#include <time.h> /* for elapsed time */
#include <math.h>
#include "hello.decl.h"

#define PI_REF 3.141592653589793238462643383279 // 30 digits

#define AUTOMATE_RUN 1

#define N_REF 1000000000

CProxy_Main mainProxy;
int nElements;

clock_t t1, t2;

/*mainchare*/
class Main : public CBase_Main
{
public:
  Main(CkArgMsg* m)
  {
    //Process command-line arguments
    /*nElements=5;
    if(m->argc >1 ) nElements=atoi(m->argv[1]);
    delete m;

    //Start the computation
    CkPrintf("Running Hello on %d processors for %d elements\n",
	     CkNumPes(),nElements);
    mainProxy = thisProxy;

    CProxy_Hello arr = CProxy_Hello::ckNew(nElements);

    arr[0].SayHi(17);*/

    double elapsedTime = 0.0;

    int id = CkMyRank();
    int p = CkNumPes();

    int i; 
    double x, pi, pi_contribution = 0.0;

    mainProxy = thisProxy;
    CProxy_Hello arr = CProxy_Hello::ckNew(p);

    //CkPrintf("Running with %d processors. (MyID = %d)\n", p, id);

    t1 = clock();
    CkPrintf("Before Callback\n");
    CkCallback *cb = new CkCallback(CkIndex_Hello::SayHi(p), mainProxy);
    CkPrintf("After Callback\n");

    arr.ckSetReductionClient(cb);
    CkPrintf("Checkpoint 1\n");
    arr.SayHi(p);
    CkPrintf("Checkpoint 2\n");

    /*for (i = 0; i < p; i++) {
      arr[i].ckSetReductionClient(cb);
      arr[i].SayHi(p);
    }*/
    
    /*double a = 1.0 / ( 2.0 * (double)N_REF );
    double sum = 0.0;
    for (i = id; i < N_REF; i += p) {
      sum += f( i/(double)N_REF ) + f( (i+1.0)/(double)N_REF );
    }
    //pi_contribution = a * sum;
    pi = a * sum;
    contribute(sizeof(double),&pi,CkReduction::sum_double);
    */
    
    //t2 = clock();
    //elapsedTime = (double)(t2 - t1) / CLOCKS_PER_SEC;

    /*if ( id == 0 ) {
      print_result(elapsedTime, pi);
      save_benchmark(p, elapsedTime);

      mainProxy.done();
    }*/
  };

  void print_result(double elapsedTime, double pi) {
    printf("Elapsed time:          %2f s\n", elapsedTime );
    printf("Result Pi:             %.30f\n", pi);
    printf("Error Approximation:   %.30f\n", fabs(pi - PI_REF));
  };

  void save_benchmark(int numprocs, double elapsedTime) {
    if (AUTOMATE_RUN) {
      printf("\n");
      printf("Auto-Save:\n");
      char command[100];
      snprintf (command, 100,"echo \"%d %f\" >> stats.dat", numprocs, elapsedTime);
      printf("Running Command: %s\n",command);
      system(command);
    }
  };

/*  double f(double x) {
    return 4.0 / ( 1.0 + (x * x) );
  };

  double computeMyPi(int id, int p) {
    double a = 1.0 / ( 2.0 * (double)N_REF );
    double sum = 0.0;
    for (i = id; i < N_REF; i += p) {
      sum += f( i/(double)N_REF ) + f( (i+1.0)/(double)N_REF );
    }

    return a * sum;
  }*/

  void done(double result)
  {
    t2 = clock();
    double elapsedTime = (double)(t2 - t1) / CLOCKS_PER_SEC;

    print_result(elapsedTime, result);
    save_benchmark(CkNumPes(), elapsedTime);

    CkPrintf("All done\n");
    CkExit();
  };
};

/*array [1D]*/
class Hello : public CBase_Hello
{
public:
  Hello()
  {
    CkPrintf("Hello %d created\n",thisIndex);
  }

  Hello(CkMigrateMessage *m) {}
  
  void SayHi(int p)
  {
    CkPrintf("[%d] ThisIndex %d\n",thisIndex, thisIndex);
    double pi = computeMyPi(thisIndex, p);
    CkPrintf("[%d] computed pi %.20f\n",thisIndex, pi);
    contribute(sizeof(double),&pi,CkReduction::sum_double);
    //mainProxy.done();
    CkPrintf("[%d] After Contribute.\n",thisIndex);
  }

  double f(double x) {
    return 4.0 / ( 1.0 + (x * x) );
  }

  double computeMyPi(int id, int p) {
    int i;
    double a = 1.0 / ( 2.0 * (double)N_REF );
    double sum = 0.0;
    for (i = id; i < N_REF; i += p) {
      sum += f( i/(double)N_REF ) + f( (i+1.0)/(double)N_REF );
    }

    return a * sum;
  }

};

#include "hello.def.h"
