#pragma once
#include <iostream>
#include <memory>
#include "encode/fixLen.h"


namespace Snowk::internal {
using namespace std;

template<endian E = endian::native>
struct ASCII: public fixLen<ASCII<E>,char>{
public:
    typedef endianless<char32_t, E> Char;
    typedef endianless<char, E> achar;
    typedef endianPtr<char, E> pachar;

    ASCII(char* ptr)         { this->p.ptr = ptr; }
    ASCII(void* ptr)         { this->p = (char*)ptr; }
    ASCII(pachar ptr)        { this->p = ptr; }
    ASCII (const ASCII &obj) { this->p = obj.p; }

    static Char decode(pachar p) {
        return static_cast<Char>(*p);
    }
    
    static void encode(Char unicode, pachar ptr){
        if (unicode <= 0x7F) [[likely]] {
            *ptr = static_cast<achar>(unicode);
        } else { *ptr = '\0'; }
    }
};

}
