#ifndef ENDIANPTR_H
#define ENDIANPTR_H 

#include <bit>
#include <concepts>
#include <cstddef>
#include <iostream>
#include <memory>
#include "endianless.h"  //  NOLINT

namespace Snowk::internal{
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
    
    bool operator==(const endianPtr& rhs) const{
        return ptr == rhs.ptr;
    }
    bool operator!=(const endianPtr& rhs) const{
        return ptr != rhs.ptr;
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

    // endianPtr& operator=(endianPtr rhs){
    //     ptr = rhs.ptr;
    // }

    using Ptr = T*; 
    operator Ptr() const{
        return ptr;
    }
};
namespace{
    template <typename D, endian E>
    void f(endianPtr<D,E> _){};
}
template <typename T>
concept isEndianPtr = requires(T t) { f(t); };


// 加减法运算符重载
#define PTR endianPtr<D,E>
    template <integral T, typename D, endian E>
    PTR operator+(const PTR& lhs, T rhs)
        { return PTR{lhs.ptr+rhs}; }

    template <integral T,typename D, endian E>
    PTR operator+(T lhs, const PTR& rhs)
        { return PTR{rhs.ptr+lhs}; }

    template <integral T, typename D, endian E>
    PTR operator-(const PTR& lhs, T rhs)
        { return PTR{lhs.ptr-rhs}; }

    template <integral T, typename D, endian E>
    PTR operator-(T lhs, const PTR& rhs)
        { return PTR{rhs.ptr-lhs}; }

    // 求指针之间字节距离
    template <typename D, endian E>
    size_t operator-(const PTR& lhs, const PTR& rhs)
        { return lhs.ptr-rhs.ptr; }
#undef PTR
}

#endif