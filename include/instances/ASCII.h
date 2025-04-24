#ifndef ASCII_H
#define ASCII_H

#include <iostream>
#include <memory>
#include "encode/fixLen.h"


namespace Dstring {
using namespace std;

struct ASCII: public fixLen<ASCII,char>{
public:
    ASCII(char* ptr){ p = ptr; };
    ASCII (const ASCII &obj){ p = obj.p; }

    char32_t decode(char* p) {
        return static_cast<char32_t>(*p);
    }
    
    void encode(char32_t unicode, char* ptr){
        if (unicode <= 0x7F) [[likely]] {
            *ptr = static_cast<char>(unicode);
        } else { *ptr = '\0'; }
    }
};

}

#endif