#include "node.h"

template <typename T>
T BinaryNode<T>::evaluate(){
    if(op == BinaryNode::PLUS){
        return left->evaluate() + right->evaluate();
    } else if(op == BinaryNode::MINUS){
        return left->evaluate() - right->evaluate();
    } else if(op == BinaryNode::TIMES){
        return left->evaluate() * right->evaluate();
    } else if(op == BinaryNode::DIV){
        return left->evaluate() / right->evaluate();
    }
}

template <typename T>
T UnaryNode<T>::evaluate(){
    if(op == UnaryNode::PLUS){
        return +node->evaluate();
    } else if(op == UnaryNode::MINUS){
        return -node->evaluate();
    } else if(op == UnaryNode::NOT){
        return !node->evaluate();
    }
}

template <typename T>
T ValueNode<T>::evaluate(){
    return va.value;
}