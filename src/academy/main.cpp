#include "Executor.h"

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   Executor exe;
   exe.start();
   
   return a.exec();
}

