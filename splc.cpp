/* SI413 Fall 2021
 * Lab 11
 * Main function to run the compiler
 * Everett Stenberg
 */

#include <fstream>
#include <vector>
#include "splc.hpp"
#include "spl.tab.hpp"
#include "ast.hpp"
#include "frame.hpp"

// global variable declarations
colorout resout(1, 'u');
colorout errout(2, 'r');

Stmt* tree = nullptr;
void writeLambdas(Context*);

int main(int argc, char** argv) {
  // 0, 1, and 2 correspond to stdin, stdout, and stderr respectively.
  bool interactive = isatty(0) && isatty(2);

  // this saves the entire program's AST to deallocate at the end
  vector<Stmt*> program;

  // figure out filenames if there is a command-line argument for that
  if (argc >= 2) {
    if (!(yyin = fopen(argv[1],"r"))) {
      cerr << "Could not open input file \"" << argv[1] << "\"!" << endl;
      exit(2);
    }
    string ofname = argv[1];
    int dotind = ofname.rfind('.');
    if (dotind != string::npos) {
      ofname = ofname.substr(0, dotind);
    }
    ofname += ".ll";
    if (!resout.redirect(ofname)) {
      cerr << "Could not open output file \"" << ofname << "\"" << endl;
      exit(2);
    }
    interactive = false;
    cerr << "Reading input from " << argv[1] << " and writing output to " << ofname << endl;
  }

  // LLVM header stuff
  resout << "target triple = \"x86_64-pc-linux-gnu\"" << endl
         << "@pfmt = constant [5 x i8] c\"%ld\\0A\\00\"" << endl
         << "declare i32 @printf(i8*,...)" << endl

         // my stuff
         << "@.str = private unnamed_addr constant [3 x i8] c\"%d\\00\", align 1" << endl
         << "@.int = private unnamed_addr constant [4 x i8] c\"%f\\0A\\00\", align 1" << endl
         << "declare i32 @scanf(i8*,...)" << endl;
         // end my stuff

  // this is the root Frame to hold all global variable bindings
  Frame gframe(nullptr);

  // this is the global context, used to keep track of register names
  Context gcon;

  // start LLVM main
  resout << "define i32 @main() {" << endl;


  // loop to read in program statements, one at a time
  tree = nullptr;
  if (interactive) cerr << "spl> ";
  while (yyparse() == 0 && tree != nullptr) {
    // save the tree for later deallocation
    program.push_back(tree);

    // this actually "executes" the tree, which means emitting LLVM code
    tree->exec(&gframe, &gcon);
    if (interactive) cerr << "spl> ";
  }

  if (interactive) cerr << "Goodbye" << endl;

  // end LLVM main
  resout << "    ret i32 0" << endl
         << "}" << endl;
  writeLambdas(&gcon);
  // cleanup
  if (argc >= 2) fclose(yyin);
  for (Stmt* node : program) delete node;
  yylex_destroy();

  return 0;
}

void writeLambdas(Context* con){

  auto ptr = con->lStructs.begin();
  while(ptr != con->lStructs.end()){
    //get the mapping
    string fName = ptr->first;
    lambdaHolder* lambdaContainerStruct = ptr->second;
    //grab the bodyStmt of the lambda
    Stmt* bodyStmt = lambdaContainerStruct->body;
    //grab the literal string variable name
    string argLiteralName = lambdaContainerStruct->varName;
    //build the register that holds the argument
    string argRegister = "%" + fName + "var";
    //grab the reference frame
    Frame* refFrame = lambdaContainerStruct->refFrame;
    //start the defintion!!
    resout << "define i64 @" << fName << " (i64 " << argRegister << "){" << endl;

    // implement the argument (register) as a functioning variable
    string argPtr = con->nextRegister();

    // make a pointer which the argReg will be mapped to
    resout << "    " << argPtr << " = alloca i64" << endl;
    // store the argument in the new Ptr
    resout << "    " << "store i64 " << argRegister << ", i64* " << argPtr << endl;

    // map the arg literal name to the newly made argument pointer
    resout << ";BOUND FUN LOCAL " << argLiteralName << " to " << argPtr << endl;
    refFrame->bind(argLiteralName,argPtr);

    //also, make a binding for the return value
    // write the code for the function
    bodyStmt->exec(refFrame,con);
    bodyStmt->getNext()->exec(refFrame,con);


    //TEMP
    resout << "    ret i64 0\n";
    //TEMP
    resout << "}\n";
    ptr++;
      }
}
