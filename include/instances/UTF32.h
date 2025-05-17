#pragma once
#include "common.h"
#include "fixLen.h"

namespace Snowk::internal {
using namespace std;

template<endian E = endian::native>
class UTF32: public fixLen<UTF32<E>, char32_t>{
public:
    typedef endianless<char32_t, E> Char;
    typedef endianPtr<char32_t, E> pChar;

    UTF32(char32_t* ptr)     { this->p.ptr = ptr; }
    UTF32(void* ptr)         { this->p.ptr = (char32_t*)ptr; }
    UTF32(pChar ptr)         { this->p = ptr; }
    UTF32 (const UTF32 &obj) { this->p = obj.p; }

    static Char decode(pChar p) {
        return *p;
    }

    // 等于和不等于
    template<template<endian>typename Temp, endian E0>
    bool operator==(const Temp<E0> &obj) {
        if constexpr(E0==E){
            return *this->p == *obj.p;
        }else {
            return false;
        }
    }

    static void encode(Char unicode, pChar ptr){
        if (unicode <= 0x10FFFF) [[likely]] {
            *ptr = unicode;
        } else { *ptr = U'\xFFFD'; }
    }
};

}