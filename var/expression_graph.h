#ifndef INCLUDED_EXPRESSION_GRAPH
#define INCLUDED_EXPRESSION_GRAPH

#include "node.h"

template <typename T>
class Node;

template <typename T>
class ExpressionGraph {

private:
    Node<T>& graph;

public:

    ExpressionGraph(Node<T> &g) : graph(g) {};

    T evaluate() {
        return graph.evaluate();
    }
};
#endif