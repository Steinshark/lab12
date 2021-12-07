/* SI 413 Fall 2021
 * Lab 11
 * This is a C++ header file for the AST class hierarchy.
 * EVERETT STENBERG
 */

#ifndef AST_HPP
#define AST_HPP
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <typeinfo>
#include "splc.hpp"
#include "frame.hpp"

// This enum type gives codes to the different kinds of operators.
// Basically, each oper below such as DIV becomes an integer constant.
enum Oper {
  ADD, SUB,
  MUL, DIV,
  LT, GT, LE, GE,
  EQ, NE,
  AND, OR, NOT
};

// These are forward declarations for the classes defined below.
// They show the class hierarchy.
class AST;
  class Stmt;
    class NullStmt;
    class Block;
    class IfStmt;
    class WhileStmt;
    class NewStmt;
    class Asn;
    class Write;
    class ExpStmt;
  class Exp;
    class Id;
    class Num;
    class BoolExp;
    class ArithOp;
    class CompOp;
    class BoolOp;
    class NegOp;
    class NotOp;
    class Read;
    class Lambda;
    class Funcall;

/* The AST class is the super-class for abstract syntax trees.
 * Every type of AST (or AST node) has its own subclass.
 */
class AST {
  private:
  protected:
    // These two protected fields determine the structure of the AST.
    vector<AST*> children;

    // Inserts a new AST node as a child of this one.
    void ASTchild(AST* child) { children.push_back(child); }

  public:
    /* Makes a new "empty" AST node. */
    AST() { }

    /* Deallocates memory for this AST note and its children. */
    virtual ~AST() {
      for (AST* child : children) delete child;
      children.clear();
    }
};

/* Every AST node that is not a Stmt is an Exp.
 * These represent actual computations that return something
 * (in particular, a Value object).
 */
class Exp :public AST {
  public:
    /* This is the method that must be overridden by all subclasses.
     * It should emit LLVM code to evaluate this expresion, and return
     * a string for the LLVM value that will hold the result.
     */
    virtual string eval(Frame* ST, Context* con) {
      errout << "ERROR: eval not yet implemented in class " << typeid(*this).name() << endl;
      exit(3);
    }
};

/* An identifier, i.e. variable or function name. */
class Id :public Exp {
  private:
    string val;

  public:
    // Constructor from a C-style string
    Id(const char* v) {
      val = v;
    }

    // Returns a reference to the stored string value.
    string& getVal() { return val; }

    string eval(Frame* ST,Context* con){
        //perform a lookup to find the pointer register
        string valueRegister = ST->lookup(val);

        //load the pointer register into a working register and send that up!
        string workingRegister = con->nextRegister();
        resout << "    " << workingRegister <<  " = load i64, i64* " << valueRegister << endl;
        return workingRegister;
    }
};

/* A literal number in the program. */
class Num :public Exp {
  private:
    int val;

  public:
    Num(int v) {
      val = v;
    }

    // to evaluate, we can return a literal string representation of the number
    string eval(Frame* ST, Context* con) override {
      return to_string(val);
    }
};

/* A literal boolean value like "true" or "false" */
class BoolExp :public Exp {
  private:
    bool val;

  public:
    BoolExp(bool v) {
      val = v;
    }

    string eval(Frame* ST, Context* con) override {
      return to_string(val);
    }


};

/* A binary opration for arithmetic, like + or *. */
class ArithOp :public Exp {
  private:
    Oper op;
    Exp* left;
    Exp* right;

  public:
    ArithOp(Exp* l, Oper o, Exp* r);

    string eval(Frame* ST, Context* con) override; // this is implemented in ast.cpp
};

/* A binary operation for comparison, like < or !=. */
class CompOp :public Exp {
  private:
    Oper op;
    Exp* left;
    Exp* right;

  public:
    CompOp(Exp* l, Oper o, Exp* r);

    string eval(Frame* ST, Context* con){

        if(true and true){resout << "\n;Building CompOp instruction:\n";}
        //eval the right and left sides
        string l = left->eval(ST, con);
        string r = right->eval(ST, con);

        // Write operation
        string dest = con->nextRegister();
        resout << "    " << dest << " = icmp";
        switch(op) {
          case GT:
            resout << " sgt ";
            break;
          case GE:
            resout << " sge ";
            break;
          case LT:
            resout << " slt ";
            break;
          case LE:
            resout << " sle ";
            break;
          case EQ:
            resout << " eq ";
            break;
          case NE:
            resout << " ne ";
            break;
          default:
            // should never happen...
            errout << "Internal Error: Illegal arithmetic operator" << endl;
            exit(8);
        }
        resout << "i64 " << l << " , " << r << endl;

        //convert the i1 to an i64
        string final_dest = con->nextRegister();
        resout << "    " << final_dest << " = zext i1 " << dest << " to i64      ; must cast from i1 to i64 " << endl;
        return final_dest;
    }
};

/* A binary operation for boolean logic, like "and". */
class BoolOp :public Exp {
  private:
    Oper op;
    Exp* left;
    Exp* right;

  public:
    BoolOp(Exp* l, Oper o, Exp* r);
    string eval(Frame* ST, Context* con){

        if(true and true){resout << "\n;Building BoolOp instruction:\n";}


        //eval the right and left sides
        string l = left->eval(ST, con);
        string r = right->eval(ST, con);

        // Write operation
        string dest = con->nextRegister();
        resout << "    " << dest << " = ";
        switch(op) {
          case AND:
            resout << " and ";
            break;
          case OR:
            resout << " or ";
            break;
          case NOT:
            resout << " not ";
            break;

          default:
            // should never happen...
            errout << "Internal Error: Illegal arithmetic operator" << endl;
            exit(8);
        }

        // **does not need conversion because anything we pass in
        // will be an i64, and the result of a bitwise will be the
        // same**
        resout << "i64 " << l << " , " << r << endl;
        return dest;

    }
};

/* This class represents a unary negation operation. */
class NegOp :public Exp {
  private:
    Exp* right;

  public:
    NegOp(Exp* r) {
      right = r;
      ASTchild(right);
    }

    string eval(Frame* ST, Context* con){
              if(true and true){resout << "\n;Building NegOp instruction:\n";}
              string r = right->eval(ST, con);

              // Write operation
              string dest = con->nextRegister();
              resout << "    " << dest << " = ";

              // **does not need conversion because anything we pass in
              // will be an i64, and the result of a bitwise will be the
              // same**
              resout << "mul i64 " << -1 << " , " << r << endl;
              return dest;
    }
};

/* This class represents a unary "not" operation. */
class NotOp :public Exp {
  private:
    Exp* right;

  public:
    NotOp(Exp* r) {
      right = r;
      ASTchild(right);
    }

    string eval(Frame* ST, Context* con){
              if(true and true){resout << "\n;Building NotOp instruction:\n";}

              string r = right->eval(ST, con);

              // Write operation
              string compare = con->nextRegister();
              resout << "    " << compare << " = ";

              // do a swap and then recast to i64
              resout << "icmp eq i64 " << 0 << " , " << r << endl;
              //cast
              string dest = con->nextRegister();
              resout << "    " << dest << " = zext i1 " << compare << " to i64" << endl;
              return dest;
    }
};

/* A read expression. */
class Read :public Exp {
  public:
    Read() { }

    string eval(Frame* ST, Context* con){

        if(true and true){resout << "\n;Building READ instruction:\n";}


        // first build an i32 to use as the storage site
        string pointer = con->nextRegister();
        resout << "    " << pointer << " = alloca i32   ; ptr to read into      \n";


        // build the actual call to scanf
        string dest = con->nextRegister();
        resout << "    " << "call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32*  " << pointer << ")\n";



        // get it out of memory
        if(true and true){resout << "    ;Retrieving from mem\n";}
        string value = con->nextRegister();
        resout << "    " << value << " = load i32, i32* " << pointer << endl;


        if(true and true){resout << "    ;Casting to i64\n";}
        //convert to i64
        string final_dest = con->nextRegister();
        resout << "    " <<  final_dest << " = zext i32 " << value << " to i64\n";
        return final_dest;
    }
};

/* A Stmt is anything that can be evaluated at the top level such
 * as I/O, assignments, and control structures.
 * The last child of any statement is the next statement in sequence.
 */
class Stmt :public AST {
  private:
    // Pointer to the next statement in sequence, default null.
    Stmt* next = nullptr;

  public:
    // Default constructor. Note that the setNext method must be
    // called by the parser at some point after construction.
    Stmt () { }

    // Getter and setter for the next statement in sequence.
    Stmt* getNext() { return next; }
    void setNext(Stmt* nextStmt) {
      if (next != nullptr) {
        errout << "Unexpected parser error: trying to set next, but next already set!" << endl;
        delete next;
        children.pop_back();
      }
      children.push_back(nextStmt);
      next = nextStmt;
    }

    /* This is the command that must be implemented everywhere to
     * write the LLVM code to execute this Stmt.
     */
    virtual void exec(Frame* ST, Context* con) {
      errout << "ERROR: exec not yet implemented in class " << typeid(*this).name() << endl;
      exit(3);
    }
};

/* This class is necessary to terminate a sequence of statements. */
class NullStmt :public Stmt {
  public:
    NullStmt() { }

    // Nothing to execute!
    void exec(Frame* ST, Context* con) override { }
};

/* This is a statement for a block of code, i.e., code enclosed
 * in curly braces { and }.
 */
class Block :public Stmt {
  private:
    Stmt* body;

  public:
    Block(Stmt* b) {
      body = b;
      ASTchild(body);
    }

    void exec(Frame* ST, Context* con){
        body->exec(new Frame(ST),con);
        getNext()->exec(ST, con);
    }
};

/* This class is for "if" AND "ifelse" statements. */
class IfStmt :public Stmt {
  private:
    Exp* clause;
    Stmt* ifblock;
    Stmt* elseblock;

  public:
    IfStmt(Exp* e, Stmt* ib, Stmt* eb);

    void exec(Frame* ST, Context* con){
        string branchBegin  = con->nextLabel();
        string branchTrue   = con->nextLabel();
        string branchFalse  = con->nextLabel();
        string branchMerge  = con->nextLabel();


        string conditionalIntermediate  = clause->eval(ST, con);

        //point the program to the initial branch
        resout << "    " << "br label %" << branchBegin << "   ;I was added in an IFELSE" << endl;
        if(true == true){resout << "\n;Building IfStmt\n";}

        //cast conditional to an i1
        string conditional = con->nextRegister();
        resout << branchBegin << ":" << endl;
        resout << "    " << conditional << " = trunc i64 " << conditionalIntermediate << " to i1" << endl;

        //build the IfStmtlogic
        resout << "    " << "br i1 " << conditional << ", label %" << branchTrue << ", label %" << branchFalse << endl;

        //execute if TRUE
        if(true == true){resout << "\n;---TRUE\n";}
        resout << branchTrue << ":" << endl;
        ifblock->exec(ST, con);
        resout << "    " << "br label %" << branchMerge << endl;

        //execute if FALSE

        if(true == true){resout << "\n;---FALSE\n";}
        resout << branchFalse << ":" << endl;
        elseblock->exec(ST, con);

        resout << "    " << "br label %" << branchMerge << endl;
        //bring us back home
        if(true == true){resout << "\n;---END\n";}
        resout << branchMerge << ":" << endl;
        getNext()->exec(ST,con);//used 2b end each stmt


    }
};

/* Class for while statements. */
class WhileStmt :public Stmt {
  private:
    Exp* clause;
    Stmt* body;

  public:
    WhileStmt(Exp* c, Stmt* b) {
      clause = c;
      body = b;
      ASTchild(clause);
      ASTchild(body);
    }

    void exec(Frame* ST, Context* con){
        string startBranch      = con->nextLabel();
        string workingBranch    = con->nextLabel();
        string mergeBranch      = con->nextLabel();

        //set us to the while check
        resout << "    " << "br label %" << startBranch << ";I was added for a WHILE stmt " << endl;

        //inside WHILE CONDITION
        resout << ";WHILE" << endl;
        resout << startBranch << ":" << endl;
        string conditionalIntermediate = clause->eval(ST, con);
        string whileConditional = con->nextRegister();
        resout << "    " << whileConditional << " = trunc i64 " << conditionalIntermediate << " to i1" << endl;
        resout << "    " << "br i1 " << whileConditional << ", label %" << workingBranch << ", label %" << mergeBranch << endl;

        //inside WORKING BRANCH
        resout << ";DO" << endl;
        resout << workingBranch << ":" << endl;
        body->exec(ST,con);
        getNext()->exec(ST,con);
        resout << "    " << "br label %" << startBranch << endl;
        resout << ";DO END" << endl;


        //inside MERGE
        resout << mergeBranch << ":" << endl;



    }
};

/* A "new" statement creates a new binding of the variable to the
 * stated value.  */
class NewStmt :public Stmt {
  private:
    Id* lhs;
    Exp* rhs;

  public:
    NewStmt(Id* l, Exp* r) {
      lhs = l;
      rhs = r;
      ASTchild(lhs);
      ASTchild(rhs);
    }

    void exec(Frame* ST, Context* con){
        string identifier = lhs->getVal();
        string expResult = rhs->eval(ST, con);

        string idRegister = con->nextRegister();

        //build a pointer and load the expression result into memory
        if(true and true){resout << "\n;Building NewStmt instruction:\n";}

        // first build an i64 to use as the storage site
        resout << "    " << idRegister << " = alloca i64   ; ptr to read into      \n";
        // now load the expression result into that site
        resout << "    " << "store i64 " << expResult << ", i64* " << idRegister << endl;

        ST->bind(identifier,idRegister);
        getNext()->exec(ST, con);
    }
};

/* An assignment statement. This represents a RE-binding in the symbol table. */
class Asn :public Stmt {
  private:
    Id* lhs;
    Exp* rhs;

  public:
    Asn(Id* l, Exp* r) {
      lhs = l;
      rhs = r;
      ASTchild(lhs);
      ASTchild(rhs);
    }

    void exec(Frame* ST, Context* con){
        string identifier = lhs->getVal();
        string expResult = rhs->eval(ST, con);

        string idRegister = ST->lookup(identifier);

        //build a pointer and load the expression result into memory
        if(true and true){resout << "\n;Building Asn instruction:\n";}

        // now load the expression result into that site
        resout << "    " << "store i64 " << expResult << ", i64* " << idRegister << endl;
        getNext()->exec(ST, con);

    }
};

/* A write statement. */
class Write :public Stmt {
  private:
    Exp* val;

  public:
    Write(Exp* v) {
      val = v;
      ASTchild(val);
    }

    void exec(Frame* ST, Context* con) override; // implemented in ast.cpp
};

/* A debugging statement embedded in the code. */
class Debug :public Stmt {
  private:
    string msg;

  public:
    Debug(const string& themsg) {
      msg = themsg;
    }
};

/* A single expression as a statement. */
class ExpStmt :public Stmt {
  private:
    Exp* body;

  public:
    ExpStmt(Exp* b) {
      body = b;
      ASTchild(body);
    }

    void exec(Frame* ST, Context* con){
        body->eval(ST, con);
        getNext()->exec(ST, con);
    }
};

/* A lambda expression consists of a parameter name and a body. */
class Lambda :public Exp {
  private:
    Id* var;
    Stmt* body;

  public:
    Lambda(Id* v, Stmt* b) {
      var = v;
      body = b;
      ASTchild(var);
      ASTchild(body);
    }

    // These getter methods are necessary to support actually calling
    // the lambda sometime after it gets created.
    string getVar() { return var->getVal(); }
    Stmt* getBody() { return body; }

    string eval(Frame* ST, Context* con){
      //eval lambda adds to list of lambdas that all get written
      //at the end;
      string fName = con->nextFunction();
      string lambdaPtrReg = con->nextRegister();
      resout << "    " << lambdaPtrReg << " = ptrtoint i64(i64)* @" << fName << " to i64\n";

      //ST->bind(var->getVal(),var )
      //save this info for later
      lambdaHolder* lH = new lambdaHolder;
      lH->funName = fName;
      lH->body = body;
      lH->refFrame = ST;
      lH->varName = var->getVal();
      lH->lambda = this;

      //add the function-struct pair to the map
      con->bindLambda(fName,lH);
      cout << "RETURNING FUN NAME: " << lambdaPtrReg << endl;
      return lambdaPtrReg;
    }
};

/* A function call consists of the function name, and the actual argument.
 * Note that all functions are unary. */
class Funcall :public Exp {
  private:
    Exp* funexp;
    Exp* arg;

  public:
    Funcall(Exp* f, Exp* a) {
      funexp = f;
      arg = a;
      ASTchild(funexp);
      ASTchild(arg);
    }

    string eval(Frame* ST, Context* con){
      string argReg = arg->eval(ST,con);
      string funPtrAs64 = funexp->eval(ST,con);
      string callResultReg = con->nextRegister();
      string funPtr = con->nextRegister();
      resout << "    " << funPtr << " = inttoptr i64" << funPtrAs64 << " to i64 (i64)*" << endl;
      resout << "    " << callResultReg << " =  call i64 " << funPtr << " ( i64 "<< argReg <<  ")";
      return callResultReg;
    }
};

#endif //AST_HPP
