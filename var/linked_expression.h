#ifndef INCLUDED_LINKED_EXPRESSION
#define INCLUDED_LINKED_EXPRESSION

#include "var.h"
#include "expression_graph.h"

template <typename T>
class var;

template <typename T>
class ExpressionGraph;

template <typename T>
class LinkedExpression {
private:
    ExpressionGraph<T> graph;
    var<T>& to;
public:

    LinkedExpression(ExpressionGraph<T> eg, var<T>& t) : graph(eg), to(t) {};

    void update();

};

#endif