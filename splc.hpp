/* SI413 Fall 2021
 * Lab 11
 * Global definitions for the compiler
 * EVERETT STENBERG
 */

#ifndef SPLC_HPP
#define SPLC_HPP

#include "colorout.hpp"
#include "frame.hpp"
using namespace std;

// forward declarations
class Stmt;
class Lambda;

// Global variables
extern colorout resout;
extern colorout errout;
extern Stmt* tree;

// Holds everything pertinent for a future function call
struct lambdaHolder{
	string funName;
	Stmt* body;
  Lambda* lambda;
	Frame* refFrame;
	string varName;

};

// This class holds the context for AST evaluation
// Right now, it just keeps track of register or label names to use
class Context {
  private:
    int regcount = 0;
    int brcount  = 0;
    int funcount = 0;

  public:
    map<string,lambdaHolder*> lStructs;
    // returns a series of unique register names like
    // "%v1", "%v2", etc.
    string nextRegister() {
      return "%v" + to_string(++regcount);
    }
    string nextLabel(){
        return "b" + to_string(++brcount);
    }
    string nextFunction(){
	return "fun" + to_string(++funcount);
    }
    //
    void bindLambda(string fName, lambdaHolder* lH){
    	if(lStructs.count(fName) == 0){
        	lStructs[fName] = lH;
    	}
    	else{
    		errout << "ERROR: Variable " << fName << " already bound!\n";
    	}
    }

    lambdaHolder* lookup(string fName){
	    lambdaHolder* lH = lStructs[fName];
	    if(!lH){
		    errout << " bad : (\n";
	    	exit(5);
	    }
	    return lH;
    }


};

#endif // SPLC_HPP
