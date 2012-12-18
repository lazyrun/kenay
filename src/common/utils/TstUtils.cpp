//  TstUtils.cpp

#include <stdio.h>
#ifndef __linux__
   #include <conio.h>
   #include <dos.h>
#endif

#include <fstream>

#include "TstUtils.h"
#include "pers_reporter.h"

namespace tut
{

test_runner_singleton runner;

void WaitForKey()
{
#ifndef __linux__
   std::cout << "Press any key..." << std::endl;
   while(_kbhit()) _getch(); _getch();
#endif
}

void tensure(const char *file, int line, bool check_res, const char *msg_)
{
   char buf[500];
   sprintf(buf, "file: %s, line=%d", file, line);
   std::string msg = buf;
   if(msg_ != 0)
   {
      msg += ":";
      msg += msg_;
   }
   ensure(msg.c_str(), check_res);
}

struct test_range
{
   int frst, scnd;
};

const char *fill_test_range(const char *par, test_range& r)
{
   int len, i;
   const char *left = 0, *comma;
   char *part = 0, *minus;

   if((par == 0) || (*par == 0))
      return left;

   comma = strchr(par, ',');
   if(comma != 0)
   {
      left = comma + 1;
      len = left - par - 1;
   }
   else
   {
      len = strlen(par);
      left = par + len;
   }
   part = new char[len + 1];
   memmove(part, par, len);
   part[len] = 0;

   minus = strchr(part, '-');
   if(minus == 0)
   {
      if(comma == 0)
      {
         for(i=0; i<len ; i++)
         {
            if(!::isdigit(part[i]))
               return 0;
         }
      }
      r.frst = r.scnd = ::atoi(part);
   }
   else
   {
      *minus++ = 0;
      r.frst = ::atoi(part);
      r.scnd = ::atoi(minus);
   }

   return left;
}

int run_tests(int argc, char* argv[])
{
   std::string test;
   std::string logfile;
   int param_num = 1, res;
   bool show_list = false;

   if(argc > 1)
   {
      if(argv[1][0] == '-')
      {
         if(tolower(argv[1][1]) == 'l')
            show_list = true;
         else if(tolower(argv[1][1]) == 'h')
         {
            std::cout << "Usage: bcctests [-h] | | [-l] | [-f logfile] [group [test_num]]" << std::endl;
            std::cout << "       Show this screen: bcctests -h" << std::endl;
            std::cout << "       List all groups : bcctests -l" << std::endl;
            std::cout << "       Run one group   : bcctests group" << std::endl;
            std::cout << "       Run one test    : bcctests group test_num" << std::endl;
            WaitForKey();
            return 0;
         }
         if(tolower(argv[1][1]) == 'f')
         {
            logfile = argv[2];
            param_num = 3;
         }
      }

      test = argv[param_num];
   }

   std::ofstream *fos = 0;
   tut::reporter *reporter;
   if(logfile.empty())
   {
      reporter = new tut::reporter;
   }
   else
   {
      fos = new std::ofstream(logfile.c_str());
      if(!fos->is_open())
      {
         std::cout << "Can't open log file:" << logfile.c_str() << std::endl;
         return 1;
      }

      reporter = new tut::reporter(*fos);
   }

   tut::runner.get().set_callback(reporter);

   try
   {
      if(show_list)
      {
         std::cout << "registered test groups:" << std::endl;
         tut::groupnames gl = tut::runner.get().list_groups();
         tut::groupnames::const_iterator i = gl.begin();
         tut::groupnames::const_iterator e = gl.end();
         int cnt = 0;
         while( i != e )
         {
            std::cout << "  " << *i << std::endl;
            ++i;
            cnt++;
         }
         if(cnt == 0)
         {
            std::cout << "    <empty>" << std::endl;
         }
      }
      else if(test.empty())
      {
         tut::runner.get().run_tests();
      }
      else
      {
         tut::test_range r;
         reporter->start_multipass();
         while(!test.empty())
         {
            const char *next = tut::fill_test_range(argv[param_num + 1], r);
            param_num++;

            if(next)
            {
               param_num++;
               while(next)
               {
                  for(int i=r.frst; i<=r.scnd; i++)
                  {
                     tut::runner.get().run_test(test.c_str(), i);
                  }
                  next = tut::fill_test_range(next, r);
               }
            }
            else
            {
               tut::runner.get().run_tests(test.c_str());
            }

            test = (argv[param_num] == 0) ? "" : argv[param_num];
         }
         reporter->end_multipass();
      }
   }
   catch( const std::exception& ex )
   {
      std::cout << "tut raised ex: " << ex.what() << std::endl;
   }

   if(fos != 0)
   {
      delete fos;
      //OSFileStat st(logfile.c_str());
      //if(st.Size() == 0)
      //   unlink(logfile.c_str());
   }

   res = reporter->all_ok() ? 0 : 1;
   delete reporter;

   WaitForKey();

   return res;
}

}// namespace tut

