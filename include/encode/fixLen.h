#ifndef FIXLEN_H
#define FIXLEN_H

#include "common.h"
#include "logic.h"


namespace Snowk::internal {

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

    fixLen(): p(nullptr) {}
public:
    endianPtr<T,E> p;
    T* data(){ return p; }
    
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

    // 获取单元级大小（不包括结尾 0）
    size_t size(){
        T* ptr = p;
        size_t size = 0;
        while(*(ptr + size) != 0){
            size ++;
        } return size;
    }

    size_t len(){ return size(); }

    constexpr __always_inline 
    size_t encode_width(Char){
        return sizeof(T);
    }

};
    

}


#endif