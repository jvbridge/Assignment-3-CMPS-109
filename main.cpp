// $Id: main.cpp,v 1.8 2015-04-28 19:23:13-07 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

// settings of the template from listmap.tcc
using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

// scans the options and activates them through trace.cpp's traceflags class
void scan_options (int argc, char** argv) {
   // opterr is an int from getopt (standard c library) which is 1 by
   // default (meaning there is an error) This makes it false so getopt
   // can set it to true in the event that it needs to be set
   opterr = 0;

   // this is an infinite for loop which iterates over the options in
   // argv
   for (;;) {
      // get options that are legitimate (@ is the only one, but
      // requires additional arguments)
      int option = getopt (argc, argv, "@:");
      // if we are out of options break the loop
      if (option == EOF) break;
      // take the current option and
      switch (option) {
         // for the character @ set the flags for the args  optarg is a
         // string which has everything after @ this is sending that
         // string off to trace.cpp to set the flags as necessary
         case '@':
            traceflags::setflags (optarg);
            break;
         default: // if it's anything other than @ complain
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char** argv) {
   // sets the string in sys_info class to the name given in argv[0]
   sys_info::set_execname (argv[0]);
   // uses local function to set the options
   scan_options (argc, argv);

   // make a listmap of string pairs named test
   str_str_map test;

   /**
    * optind is the index of the NEXT argument to argv
    * argp is a pointer to and array of strings, which is initialized as
    * pointing to argv[1] (optind means "next arg")
    *
    * the loop iterates over args in argv, argp will be pointing at the
    * memory location after the last arguent when it is pointing at
    * argv[argc], so that's why it terminates there, ++argp
    */
   for (char** argp = &argv[optind]; argp != &argv[argc]; ++argp) {
      // make a pair, the first member is the current string in argp
      str_str_pair pair (*argp, to_string<int> (argp - argv));
      cout << "Before insert: " << pair << endl;
      test.insert (pair);
   }

   // make an iterator from the listmap template (listmap.tcc) and
   // use it to iterate over the map, printing it to cout
   for (str_str_map::iterator itor = test.begin();
        itor != test.end(); ++itor) {
      cout << "During iteration: " << *itor << endl;
   }

   // erase the iterator
   str_str_map::iterator itor = test.begin();
   test.erase (itor);

   // print exit message and exit
   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}
