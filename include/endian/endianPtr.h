#ifndef ENDIANPTR_H
#define ENDIANPTR_H 

#include <bit>
#include <iostream>
#include <memory>
#include "endianless.h"  //  NOLINT

namespace Dstring{
using namespace std;

// 端序无关指针
template <typename T, endian E>
class endianPtr{
    using Content = endianless<T,E>;
public:
    T* ptr;
    endianPtr(T* ptr) : ptr(ptr) {}
    endianPtr(){}

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
        return *reinterpret_cast<Content*>(ptr);
    }
    Content& operator[](int i){
        return *reinterpret_cast<Content*>(ptr+i);
    }

    Content* operator->()
        {return this->ptr; }
    
    endianPtr& operator+=(int rhs){
        ptr += rhs;
        return *this;
    }
    endianPtr& operator-=(int rhs){
        ptr -= rhs;
        return *this;
    }

    using Ptr = T*; 
    operator Ptr() {
        return ptr;
    }
};
namespace{
    template <typename D, endian E>
    void f(endianPtr<D,E> t){};
}
template <typename T>
concept isEndianPtr = requires(T t) { f(t); };


// 加减法运算符重载
#define PTR endianPtr<D,E>
    template <typename T, typename D, endian E>
    PTR operator+(const PTR& lhs, T rhs)
        {return PTR{lhs.ptr+rhs}; }

    template <typename T,typename D, endian E>
    PTR operator+(T lhs, const PTR& rhs)
        {return PTR{rhs.ptr+lhs}; }

    template <typename T, typename D, endian E>
    PTR operator-(const PTR& lhs, T rhs)
        {return PTR{lhs.ptr-rhs}; }

    template <typename T, typename D, endian E>
    PTR operator-(T lhs, const PTR& rhs)
        {return PTR{rhs.ptr-lhs}; }
#undef PTR
}
#endif