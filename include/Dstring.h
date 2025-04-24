#ifndef Dstring_H
#define Dstring_H

#include "endian/endianless.h"
#include "encode/encode.h"
#include "instances/UTF8.h"
#include "instances/UTF16.h"
#include "instances/UTF32.h"
#include "instances/ASCII.h"
namespace Dstring {
using namespace std;
template <
    typename U,
    Encode code_it,             // 字符串编码
    endian E = endian::native,  // 字符串字节序
    bool Null = true,           // 是否使用 \0 结尾
    bool COW = false            // 是否使用 写时复制
>
class Dstring {
private:
    size_t capacity;
    size_t size;
    using ptr = conditional_t<COW,
        shared_ptr<code_it>, code_it*>>;
    ptr data;
public:
    // 支持多种源字符串编码
    Dstring(char8_t* s){};
    Dstring(char16_t* s){};
    Dstring(char32_t* s){};
    Dstring(char* s){};

    code_it begin(){};
    code_it end(){};
    ~Dstring();
};

}
#endif