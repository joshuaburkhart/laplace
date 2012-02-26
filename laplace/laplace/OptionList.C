
/*--------------------------------------------------------------*/
/*  Copyright (c) 2000 by the University of Oregon.		*/
/*  See the COPYRIGHT file in this directory for permission	*/
/*  to use and distribute this software.			*/
/*--------------------------------------------------------------*/

// $Id$

// OptionList.C -- implementation of OptionList class			

// John Conery								
// University of Oregon							
// May 2000								


#include "OptionList.h"

#include <ostream>
#include <stdlib.h>

// Copy the command line arguments to a local list.  Start at
// argv[1] since we don't want the program name (argv[0]).

OptionList::OptionList(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++)
    theList.push_back(string(argv[i]));
}

// Delete the list and any items left

OptionList::~OptionList() {
  theList.clear();
}

// Local procedures to search the list for command names.  One
// version is used when a command expects a value -- it returns
// the string that immediately follows the command, or else
// "" if the command isn't in the list.  For error checking,
// the procedure makes sure the argument exists (i.e. the user
// didn't give a command as the last item on the line) and the
// value doesn't start with a hyphen.  If the command is found,
// it is removed from the list, and if the argument is valid,
// it is also removed.

// The second procedure is used if a command is a simple flag; 
// if the command is in the list the procedure removes the command
// and returns true.
//
// Future addition: return a list of values up to but not
// including the next command.

string removeArg(const string &name, list<string> &L) {
  string argvalue = "";
  string argname = "-" + name;

  list<string>::iterator ptr = find(L.begin(), L.end(), argname);
  list<string>::iterator next = L.end();

  if (ptr != L.end()) {
    next = ptr;
    advance(next,1);
    L.erase(ptr);
    if ((next != L.end()) && ((*next)[0] != '-')) {
      argvalue = *next;
      L.erase(next);
    }
  }

  return argvalue;
}

bool removeFlag(const string &name, list<string> &L) {
  string argname = "-" + name;

  list<string>::iterator ptr = find(L.begin(), L.end(), argname);

  if (ptr != L.end()) {
    L.erase(ptr);
    return true;
  }
  else {
    return false;
  }
}

// Public methods -- getInt(), getDouble(), and getOption() call
// removeArg() to locate and remove a command and its argument;
// getFlag() calls removeFlag() to locate and remove a single command.

int OptionList::getInt(const string &name, int idefault) {
  int argvalue = idefault;
  string arg = removeArg(name,theList);
  if (arg.length() > 0)
    argvalue = atoi(arg.c_str());
  return argvalue;
}

double OptionList::getDouble(const string &name, double ddefault) {
  double argvalue = ddefault;
  string arg = removeArg(name,theList);
  if (arg.length() > 0)
    argvalue = atof(arg.c_str());
  return argvalue;
}

bool OptionList::getFlag(const string &name, bool bdefault) {
  if (removeFlag(name,theList))
    return true;
  else
    return bdefault;
}

string OptionList::getOption(const string &name, const string &sdefault) {
  string argvalue = sdefault;
  string arg = removeArg(name,theList);
  if (arg.length() > 0)
    argvalue = arg;
  return argvalue;
}

// Debugging utility -- print the contents of the list:

void OptionList::print() const {
  //  copy(theList.begin(), theList.end(), ostream_iterator<string>(cout,"\n"));
}

// Return a copy of the list:

list<string> &OptionList::contents() {
  return theList;
}

// Return the next string in the list -- don't do this until
// all the options are removed...

string OptionList::next() {
  string s = theList.front();
  theList.pop_front();
  return s;
}

// See if the list is empty

bool OptionList::empty() const {
  return (theList.size() == 0);
}
