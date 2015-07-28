// $Id: main.cpp,v 1.8 2015-04-28 19:23:13-07 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <fstream>
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

//////////////////////////
// My helper functions  //
//////////////////////////

// function copied from stack overflow

/**
 * Helper fuction that strips out all the whitespace from the ends
 * @param  in string to have whitespace stripped off
 * @return    the stripped string
 */
string strip_whitespace(string& in){
   string str = in;

   TRACE('s', "starting str: |" << str << "|");

   // check if string is just whitespace
   if(str.find_first_not_of (' ') == str.npos){
      return "";
   }

   // trim leading whitespace
   size_t startpos = str.find_first_not_of(" ");
   if( string::npos != startpos ) {
      TRACE('s', "found whitespace at beginning");
      str = str.substr( startpos );
   }

   TRACE('s', "front whitespace stripped: " << "|" << str << "|");

   // trim trailing spaces
   size_t endpos = str.find_last_not_of(" \t");
   if( string::npos != endpos ){
      TRACE('s', "found whitespace at end");
      str = str.substr( 0, endpos+1 );
   }
   TRACE('s', "back whitespace stripped: " << "|" << str << "|");

   return str;
}

/**
 * Reads the line in and executes the appropriate code based on the
 * line's contents
 * @param line the command that is taken in
 */
void read_line(string& line){

   TRACE ('s', "Line is: " << line);

   // check if the line is commented out first
   if (line[0] == '#'){
      TRACE('m', "Comment: " << line);
      return;
   }

   // find where the = sign is
   size_t equals_pos = line.find_first_of('=');

   // if there is no = sign on the line parse it for a key
   if (equals_pos == string::npos){
      // get temporary value for the line
      string str = line;

      // strip whitespace
      str = strip_whitespace(str);

      // if it's empty we return
      if (str.empty()){
         TRACE('m', "Nothing on line, returning");
         return;
      }

      TRACE('m', "No equals sign, printing key: " << line);
      // print_key(key);
      // TODO put function here to print key
      return;
   }

   TRACE('h', "equals_pos: " << equals_pos);

   // split string around = sign
   string first_half;
   string second_half;

   // get the halves of the strings
   // if (equals_pos != 0){
      first_half = line.substr(0, equals_pos);
      second_half = line.substr(equals_pos + 1, line.size());
   // } else{
   //    first_half = "";
   //    second_half = line.substr(equals_pos + 1, line.size());
   // }

   TRACE ('s', "first_half: " << first_half);
   TRACE ('s', "second_half: " << second_half);

   // strip whitespace out of lines using helper function above
   first_half = strip_whitespace(first_half);
   second_half = strip_whitespace(second_half);


   // if they're both just whitespace, print everything;
   if (second_half.empty() && first_half.empty()){
      // TODO: make function work
      TRACE('m', "Printing all keys");
      // print_all();
      return;
   }

   // if there's nothing but whitespace in the second half, delete the
   // key indicated
   if (second_half.empty()){
      TRACE('m', "Deleting key: " << first_half);
      // delete_key (key);
      // TODO make function work
      return;
   }

   if (first_half.empty()){
      TRACE('m', "printing all keys with value: " << second_half);
      // TODO make function work
      // assign_all(second_half);
      return;
   }

   // both halves aren't empty. need to assing a value
   TRACE('m', "Assinging key: " << first_half);
   TRACE('m', "        to be: " << second_half);

   // TODO make function work
   // assign(first_half, second_half);

}

void read_file(ifstream& file){

   // string file_str((std::istreambuf_iterator<char>(my_file)), std::istreambuf_iterator<char>());

   string line;
   // get all characters from each line and do the operations
   while (!file.eof()){
      getline(file, line);
      read_line(line);
   }
}

//////////
// Main //
//////////

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

      // make a pair, the second member is the filename, the second is
      // the argument number
      str_str_pair pair (*argp, to_string<int> (argp - argv));
      cout << "Before insert: " << pair << endl;

      // test code!!!!
      string filename = *argp;
      ifstream my_file (filename);
      if (my_file.is_open()){
         TRACE ('m', *argp << " is a valid file");
         // hand off operations to the helper functions
         read_file(my_file);
      } else {
         cout << *argp << ": No such file or directory" << endl;
      }


      test.insert (pair);
   }

   // make an iterator from the listmap template (listmap.tcc) and
   // use it to iterate over the map, printing it to cout
   for (str_str_map::iterator itor = test.begin();
        itor != test.end(); ++itor) {
      cout << "During iteration: " << *itor << endl;

      // check if the file exists, complain if it doesn't
      // string filename = *itor;
      // ifstream myfile (filename);
      // if (myfile.is_open()){
      //    TRACE ('m', *argp << " is a valid file");
      //    // hand off operations to the helper functions
      //    read_file(myfile);
      // } else {
      //    cout << *argp << ": No such file or directory";
      // }
   }

   // erase the iterator
   // str_str_map::iterator itor = test.begin();
   // test.erase (itor);

   // print exit message and exit
   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}
