CC  = g++
CXX = g++ -O0 

#-fno-elide-constructors

INCLUDES =

CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -std=c++14 -g -Wall $(INCLUDES) -o3

LDFLAGS = -g
LDLIBS  =

default: test

test: test.o

test.o: var.o expression_graph.o linked_expression.o node.o

.PHONY: clean
clean:
	rm -f var expression_graph linked_expression test a.out core *.o

.PHONY: all
all: clean default