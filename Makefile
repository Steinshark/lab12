## SI 413 Fall 2021
## Makefile for Lab 11

SHELL:=/bin/bash
SYNTAX=spl
PROGS=splc
IMPLS=ast.cpp $(PROGS:=.cpp)
HEADERS=frame.hpp colorout.hpp $(IMPLS:.cpp=.hpp)
CPPFLAGS=-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -ggdb -fmax-errors=3 -O2
SPLS=$(wildcard *.spl)

# Default target
all: $(PROGS)

# Dependencies
$(SYNTAX:=.yy.o): %.yy.o: %.tab.hpp $(HEADERS)
$(SYNTAX:=.tab.o): %.tab.o: $(HEADERS)
$(IMPLS:.cpp=.o) $(PROGS:=.tab.o): %.o: %.hpp $(HEADERS)

# Rules to generate the final program with scanner and parser
$(PROGS): %: $(SYNTAX:=.tab.o) $(SYNTAX:=.yy.o) $(IMPLS:.cpp=.o)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^

# Generic rule for running C++-style flex code generation
# For instance, this will make 'pat.yy.cpp' from 'pat.lpp'.
%.yy.cpp: %.lpp
	flex -o $@ $<

# Generic rule for bison code generation
%.tab.cpp %.tab.hpp: %.ypp
	bison -d $<

# Generic rule to compiler from SPL to LLVM IR
%.ll: %.spl splc
	./splc $< $@

# Generic rule to compile from LLVM IR to an executable
%: %.ll
	clang -O2 $< -o $@

.PHONY: clean all
clean:
	rm -f *.o *.yy.cpp *.tab.* $(PROGS) $(SPLS:.spl=.ll) $(SPLS:.spl=)
