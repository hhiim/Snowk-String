#ifndef UTF_32_H
#define UTF_32_H

#include <memory>
#include "encode/fixLen.h"

namespace Dstring {
using namespace std;

struct UTF32: public fixLen<UTF32,char32_t>{
    public:
        UTF32(char32_t* ptr){ p = ptr; };
        UTF32 (const UTF32 &obj){ p = obj.p; }
    
        char32_t decode(char32_t* p) {
            return *p;
        }
        
        void encode(char32_t unicode, char32_t* ptr){
            if (unicode <= 0x10FFFF) [[likely]] {
                *ptr = unicode;
            } else { *ptr = U'\xFFFD'; }
        }
    };

}


#endif