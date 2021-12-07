/* SI 413 Fall 2021
 * Lab 11
 * This file contains the implementations of longer methods in the
 * AST class hierarchy.
 * Everett Stenberg
 */

#include "ast.hpp"

// ArithOp constructor
ArithOp::ArithOp(Exp* l, Oper o, Exp* r) {
  op = o;
  left = l;
  right = r;
  ASTchild(left);
  ASTchild(right);
}

// Evaluates an arithmetic operation
string ArithOp::eval(Frame* ST, Context* con) {
  string l = left->eval(ST, con);
  string r = right->eval(ST, con);
  string dest = con->nextRegister();
  resout << "    " << dest << " = ";
  switch(op) {
    case ADD:
      resout << "add";
      break;
    case SUB:
      resout << "sub";
      break;
    case MUL:
      resout << "mul";
      break;
    case DIV:
      resout << "sdiv";
      break;
    default:
      // should never happen...
      errout << "Internal Error: Illegal arithmetic operator" << endl;
      exit(8);
  }
  resout << " i64 " << l << ", " << r << endl;
  return dest;
}

// Constructor for CompOp
CompOp::CompOp(Exp* l, Oper o, Exp* r) {
  op = o;
  left = l;
  right = r;
  ASTchild(left);
  ASTchild(right);
}

// Constructor for BoolOp
BoolOp::BoolOp(Exp* l, Oper o, Exp* r) {
  op = o;
  left = l;
  right = r;
  ASTchild(left);
  ASTchild(right);
}

// Constructor for IfStmt
IfStmt::IfStmt(Exp* e, Stmt* ib, Stmt* eb) {
  clause = e;
  ifblock = ib;
  elseblock = eb;
  ASTchild(clause);
  ASTchild(ifblock);
  ASTchild(elseblock);
}

// execute a write statement by calling printf
void Write::exec(Frame* ST, Context* con) {
  string r = val->eval(ST, con);
  string fmtstr = con->nextRegister();

  if(true==true){resout << "\n;Building write stmt\n";}
  resout
    << "    " << fmtstr << " = bitcast [5 x i8]* @pfmt to i8*" << endl
    << "    call i32(i8*,...) @printf(i8* " << fmtstr << ", i64 " << r << ")" << endl;
  getNext()->exec(ST, con);
}
