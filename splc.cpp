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
void writeStrings(Context*);
void writeBuiltins(Context*);
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
         << "declare i32 @rand()" << endl
         << "declare double @sqrt(double)" << endl

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

  // for builtins
  string randMemPtrLoc = gcon.nextRegister();
  string randPtrReg = gcon.nextRegister();
  resout << "    " << randPtrReg << " = ptrtoint i64(i64)* @" << "randBuiltIn" << " to i64\n";
  resout << "    " << randMemPtrLoc << " = alloca i64" << endl;
  resout << "    " << "store i64 " << randPtrReg << ", i64* " << randMemPtrLoc << endl;
  gframe.bind("rand",randMemPtrLoc);


  string sqrtMemPtrLoc = gcon.nextRegister();
  string sqrtPtrReg = gcon.nextRegister();
  resout << "    " << sqrtPtrReg << " = ptrtoint i64(i64)* @" << "sqrtBuiltIn" << " to i64\n";
  resout << "    " << sqrtMemPtrLoc << " = alloca i64" << endl;
  resout << "    " << "store i64 " << sqrtPtrReg << ", i64* " << sqrtMemPtrLoc << endl;
  gframe.bind("sqrt",sqrtMemPtrLoc);


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
  writeBuiltins(&gcon);
  writeStrings(&gcon);
  // cleanup
  if (argc >= 2) fclose(yyin);
  for (Stmt* node : program) delete node;
  yylex_destroy();

  return 0;
}

void writeLambdas(Context* con){

  auto ptr = con->lStructs.begin();
  while(ptr != con->lStructs.end()){
    // Get the mapping
    string fName = ptr->first;
    lambdaHolder* lambdaContainerStruct = ptr->second;
    // Grab the bodyStmt of the lambda
    Stmt* bodyStmt = lambdaContainerStruct->body;
    Lambda* lamda = lambdaContainerStruct->lambda;
    // Grab the literal string variable name
    string argLiteralName = lambdaContainerStruct->varName;
    // Build the register that holds the argument
    string argRegister = "%" + fName + "var";
    // Grab the reference frame
    Frame* refFrame = lambdaContainerStruct->refFrame;
    // Start the defintion!!
    resout << "define i64 @" << fName << " (i64 " << argRegister << "){" << endl;

    // Implement the argument (register) as a functioning variable
    string argPtr = con->nextRegister();

    // Make a pointer which the argReg will be mapped to

    Frame* framePtr = refFrame;
    resout << ";" << framePtr << endl;
    while(framePtr->parent != NULL){
        auto frameBindingEntry = framePtr->parent->bindings.begin();
        while(frameBindingEntry != framePtr->parent->bindings.end()){
            string variableName     = frameBindingEntry->first;
            string variablePointer  = frameBindingEntry->second;
            resout <<";" << variableName << " stored in " << variablePointer << endl;

            //build a new allocation reg for it
            string newReg = con->nextRegister();
            resout << newReg << " = alloca i64" << endl;


            frameBindingEntry++;
        }
        framePtr = framePtr->parent;
    }


    resout << "    ;Binding fun argument" << endl;
    resout << "    " << argPtr << " = alloca i64" << endl;
    // Store the argument in the new Ptr
    resout << "    " << "store i64 " << argRegister << ", i64* " << argPtr << endl;

    // Map the arg literal name to the newly made argument pointer
    refFrame->bind(argLiteralName,argPtr);


    // Also, make a binding for the return value
    resout << "    ;Binding return value, init to 0\n";
    string retPtr = con->nextRegister();
    resout << "    " << retPtr << " = alloca i64";
    resout << "    " << "store i64 0, i64* " << retPtr << endl;
    refFrame->bind("ret",retPtr);
    // Write the code for the function
    bodyStmt->exec(refFrame,con);
    bodyStmt->getNext()->exec(refFrame,con);


    // load return value and yeet that boi off!
    string retValReg = con->nextRegister();
    resout << "    " << retValReg << "= load i64, i64* " << retPtr << endl;
    resout << "    ret i64  " << retValReg <<  endl;

    // All done, next
    resout << "}\n";
    ptr++;
      }
}
void writeStrings(Context* con){
    auto ptr = con->debugs.begin();
    while(ptr != con->debugs.end()){
        // Gather mpa components
        string baseString = ptr->first;
        string stringReg = ptr->second;

        // Calc string size
        int strConstSize = baseString.length() + 2;

        // Declare the string as a global constant
        string literal = "c\"" + baseString + "\\0A\\00\"";
        resout << stringReg << " = private unnamed_addr constant [" << strConstSize << "x i8] " << literal << ", align 1\n";

        //下个。。。
        ptr++;
    }

}
void writeBuiltins(Context* con){
    // RAND FUNCTION
    resout << "define i64 @randBuiltIn (i64 %randarg){" << endl;
    string randomReg = con->nextRegister();
    resout << "    " << randomReg << " = call i32 @rand()" << endl;

    string randExtendedRand = con->nextRegister();
    resout << "    " << randExtendedRand << " = zext i32 " << randomReg << " to i64" << endl;

    string retReg = con->nextRegister();
    resout << "    " << retReg << " = srem i64 " << randExtendedRand << ", %randarg" << endl;
    resout << "    " << "ret i64 " << retReg << endl;
    resout << "}" << endl << endl;



    // SQRT FUNCTION
    resout << "define i64 @sqrtBuiltIn (i64 %sqrtarg){" << endl;

    string truncArg = con->nextRegister();
    resout << "    " << truncArg << " = sitofp i64 %sqrtarg to double" << endl;

    string sqrtReg = con->nextRegister();
    resout << "    " << sqrtReg << " = call double @sqrt(double " << truncArg << ")" << endl;

    string sqrtExtendedRand = con->nextRegister();
    resout << "    " << sqrtExtendedRand << " = fptoui double " << sqrtReg << " to i64" << endl;


    resout << "    " << "ret i64 " << sqrtExtendedRand << endl;
    resout << "}" << endl << endl;



}
