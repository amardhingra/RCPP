#ifndef INCLUDED_VAR
#define INCLUDED_VAR

#include <iostream>
#include <string>

#include "linked_expression.h"

template <typename T>
class LinkedExpression;

template<typename T>
class var {
private:
    T data;
    LinkedExpression<T> *l;
    void update(){
        if(l){
            l->update();
        }
    }
public:
    var(T tt) : data(tt), l(nullptr) {};
    var(var<T> &tt) : data(tt.data), l(nullptr) {};

    void operator=(var<T> &&v){
        data = v.data;
    }
    
    var<T> operator+(const var<T>& other){
        var<T> v(this->data + other.data);
        this->update();
        return v;
    }

    var<T> operator-(const var<T>& other){
        var<T> v(this->data - other.data);
        this->update();
        return v;
    }

    var<T> operator*(const var<T>& other){
        var<T> v(this->data * other.data);
        this->update();
        return v;
    }

    var<T> operator/(const var<T>& other){
        var<T> v(this->data / other.data);
        this->update();
        return v;
    }

    void add_link(LinkedExpression<T>& ll){
        l = &ll;
    }

    friend std::ostream& operator<<(std::ostream& out, const var<T>& v){
        out << v.data;
        return out;
    };

    friend std::istream& operator>>(std::istream& in, const var<T>& v){
        in >> v.data;
        return in;
    };
};

#endif