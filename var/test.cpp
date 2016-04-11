#include <iostream>

#include "var.h"

using namespace std;

int main(void){

    var<int> i(10);
    var<int> i2(20);
    var<int> i3(20);

    var<std::string> s("abc");
    var<std::string> s2("abc");

    BinaryNode<int> bin(new ValueNode<int>(i2), BinaryNode<int>::PLUS, new ValueNode<int>(i3));
    ExpressionGraph<int> e(bin);
    LinkedExpression<int> le(e, i);
    i2.add_link(le);
    i3.add_link(le);
    std::cout << i << " " << i2 + i3 << " " << i << std::endl;


    std::cout << s + s2 << std::endl;

    return 0;
}