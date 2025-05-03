#ifndef FIXLEN_H
#define FIXLEN_H

#include "common.h"
#include "metaprogram/functional.h"

namespace Dstring {

template <typename T> struct getEndian;
template <template <endian> class Template, endian Arg>
struct getEndian<Template<Arg>> {
    const static endian E = Arg;
};

template <typename sub, typename T>
class fixLen {
protected:
    static const bool fix = true;
    static const endian E = getEndian<sub>::E;
    typedef endianless<char32_t, E> Char;

    fixLen(){}
public:
    endianPtr<T,E> p;
    fixLen& operator++() {
        p ++;
        return *this;
    }
    fixLen operator++(int) {
        fixLen tmp = *this;
        ++(*this);
        return tmp;
    }

    fixLen& operator--() {
        p --;
        return *this;
    }
    fixLen operator--(int){
        fixLen tmp = *this;
        --(*this);
        return tmp;
    }

    Char operator*() {
        return sub::decode(p);
    }
    Char operator[](size_t index)  {
        endianPtr<T,E> cursor = p;
        cursor += index;
        return sub::decode(cursor);
    }

    bool operator==(sub other){
        return (this->p) == other.p;
    }

    size_t len(){
        size_t len = 0;
        while(*(p + len) != 0){
            len ++;
        } return len;
    }
    size_t size(){
        return len()*sizeof(T);
    }
};
    

}


#endif