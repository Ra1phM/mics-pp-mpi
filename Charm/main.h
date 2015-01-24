#ifndef __MAIN_H__
#define __MAIN_H__

class Main : public CBase_Main {

 private:
  int id;
  int p;

 public:
  /// Constructors ///
  Main(CkArgMsg* msg);
  Main(CkMigrateMessage* msg);

  /// Methods ///
  void print_result(double elapsedTime, double pi);
  void save_benchmark(int numprocs, double elapsedTime);
  void xprintf (char *format, ...);
  double f(double x);

  /// Entry Methods ///
  void done();
};

#endif //__MAIN_H__