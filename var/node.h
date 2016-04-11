#ifndef INCLUDED_NODE
#define INCLUDED_NODE

#include "var.h"

template <typename T>
class var;

template <typename T>
class Node {
public:
    virtual T evaluate() = 0;
};

template <typename T>
class ValueNode : public Node<T> {
public:
    var<T>& va;
    ValueNode(var<T>& v) : va(v) {};

    T evaluate(){
        return va.data;
    }
};

template <typename T>
class UnaryNode : public Node<T> {
public:
    enum Op {
        PLUS  = 0,
        MINUS = 1,
        NOT   = 2
    };

    Node<T> *node;
    UnaryNode::Op op;

    UnaryNode(UnaryNode::Op o, Node<T>* n) : node(n), op(o) {};

    //T evaluate();//
    T evaluate(){
    if(op == UnaryNode::PLUS){
        return +node->evaluate();
    } else if(op == UnaryNode::MINUS){
        return -node->evaluate();
    } else if(op == UnaryNode::NOT){
        return !node->evaluate();
    }
}
};

template <typename T>
class BinaryNode : public Node<T> {
public:
    enum Op {
        PLUS  = 0,
        MINUS = 1,
        TIMES = 2,
        DIV   = 3
    };

    Node<T> *left;
    Node<T> *right;
    BinaryNode::Op op;

    BinaryNode(Node<T>* l, BinaryNode::Op o, Node<T> *r) : left(l), right(r), op(o) {};

    T evaluate(){
        if(op == BinaryNode::PLUS){
            return left->evaluate() + right->evaluate();
        } else if(op == BinaryNode::MINUS){
            return left->evaluate() - right->evaluate();
        } else if(op == BinaryNode::TIMES){
            return left->evaluate() * right->evaluate();
        } else if(op == BinaryNode::DIV){
            return left->evaluate() / right->evaluate();
        } else {
            return T();
        }
    }
};

#endif