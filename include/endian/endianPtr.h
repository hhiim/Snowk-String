#ifndef ENDIANPTR_H
#define ENDIANPTR_H 

#include <iostream>
#include <memory>
#include "endianless.h"

namespace Dstring{
using namespace std;

// 端序无关指针
template <typename T, endian R, endian W>
class endianPtr{
    using Content = endianless<T,R,W>;
private:
    T* ptr;
public:
    endianPtr(T* ptr) : ptr(ptr) {}

    endianPtr& operator++() {
        ptr++;
        return *this;
    }
    endianPtr operator++(int) {
        endianPtr temp = *this;
        ++(*this);
        return temp;
    }

    endianPtr& operator--() {
        ptr--;
        return *this;
    }
    endianPtr operator--(int) {
        endianPtr temp = *this;
        --(*this);
        return temp;
    }

    Content& operator*(){
        Content temp{*ptr};
        return temp;
    }
    Content& operator[](int i){
        Content temp{*(ptr+i)};
        return temp;
    }

    Content* operator->()
        {return this->ptr; }
};

namespace{
    template <typename D, endian R, endian W>
    void f(endianPtr<D,R,W> t){};
}
template <typename T>
concept isEndianPtr = requires(T t) { f(t); };


// 加减法运算符重载
template <typename T, endian R, endian W>
auto operator+(const endianPtr<T,R,W>& lhs, int rhs)
    {return endianPtr<T,R,W>{lhs.ptr+rhs}; }

template <typename T, endian R, endian W>
auto operator+(int lhs, const endianPtr<T,R,W>& rhs)
    {return endianPtr<T,R,W>{rhs.ptr+lhs}; }

template <typename T, endian R, endian W>
auto operator-(const endianPtr<T,R,W>& lhs, int rhs)
    {return endianPtr<T,R,W>{lhs.ptr-rhs}; }

template <typename T, endian R, endian W>
auto operator-(int lhs, const endianPtr<T,R,W>& rhs)
    {return endianPtr<T,R,W>{rhs.ptr-lhs}; }

}
#endif