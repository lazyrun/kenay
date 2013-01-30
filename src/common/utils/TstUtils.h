//  TstUtils.h

#ifndef TstUtils_h
#define TstUtils_h

#include "tut.h"

namespace tut
{
   int run_tests(int argc, char* argv[]);
   void tensure(const char *file, int line, bool check_res, const char *msg=0);
}

#endif

