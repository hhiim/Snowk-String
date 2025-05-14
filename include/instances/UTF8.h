#pragma oonce
#include "common.h"

namespace Dstring::internal {
using namespace std;

template<endian E = endian::native>
class UTF8 {
public:
    static const bool fix = false;
    typedef endianless<char32_t, E> Char;
    typedef endianPtr<char32_t, E> pChar;
    typedef endianless<char8_t, E> char8;
    typedef endianPtr<char8_t, E> pchar8;

    pchar8 p;
    UTF8(char8_t* ptr) : p(ptr) {}
    UTF8(void* ptr) : p((char8_t*)ptr) {}
    UTF8(pchar8 ptr) : p(ptr.ptr) {}
    UTF8(const UTF8& obj) : p(obj.p) {}

    char8_t* data() { return p; }

    UTF8& operator++() {
        p += get_width(*p);
        return *this;
    }
    UTF8 operator++(int) {
        UTF8 tmp = *this;
        ++(*this);
        return tmp;
    }

    UTF8& operator--() {
        p -=1;
        startByte(p);
        return *this;
    }
    UTF8 operator--(int) {
        UTF8 tmp = *this;
        --(*this);
        return tmp;
    }

    Char operator*() const {
        return decode(p);
    }

    bool operator==(UTF8& other) const{
        return (this->p) == other.p;
    }
    bool operator!=(UTF8& other) const{
        return (this->p) != other.p;
    }

    // 获取码点级长度
    size_t len(){
        char8_t* cursor = p;
        size_t sum = 0;
        while (*cursor!='\0'){
            size_t w = get_width(*cursor);
            sum += 1;
            cursor += w;
        }
        return sum;
    }

    // 获取单元级长度 ( 不包括 \0 )
    size_t size(){
        char8_t* cursor = p;
        size_t sum = 0;
        while (*cursor!='\0'){
            sum ++;
            cursor++;
        }
        return sum;
    }

    // 重定位开始的字节
    static char8_t* startByte(char8_t *ptr) {
        for(int i=0; i>=-3; i--){
            if ((ptr[i] & 0xC0) != 0x80) {
                return ptr+i;
            }
        } [[unlikely]]
        { return nullptr; }
    }

    // 根据首字节，获取码点宽度
    static size_t get_width(uint8_t byte) {
        if (byte <= 0x7F) return 1; // ASCII
        else if ((byte & 0xE0) == 0xC0) return 2; // 110xxxxx
        else if ((byte & 0xF0) == 0xE0) return 3; // 1110xxxx
        else if ((byte & 0xF8) == 0xF0) return 4; // 11110xxx
        return 1;
    }

    // 获取 UTF8 指针的 Unicode 码点
    static Char decode(pchar8 p){
        uint8_t byte = static_cast<uint8_t>(*p);
        size_t width = get_width(byte);

        switch (width) {
            case 1: return static_cast<char32_t>(*p); // ASCII
            case 2: {
                // 110xxxxx 10xxxxxx
                char32_t code_point =\
                (static_cast<char32_t>(p[0] & 0x1F) << 6)
                | (static_cast<char32_t>(p[1] & 0x3F));
                return code_point;
            }
            case 3: {
                // 1110xxxx 10xxxxxx 10xxxxxx
                char32_t code_point =\
                (static_cast<char32_t>(p[0] & 0x0F) << 12)
                | (static_cast<char32_t>(p[1] & 0x3F) << 6)
                | (static_cast<char32_t>(p[2] & 0x3F));
                return code_point;
            }
            case 4: {
                // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                char32_t code_point =\
                (static_cast<char32_t>(p[0] & 0x07) << 18)
                | (static_cast<char32_t>(p[1] & 0x3F) << 12)
                | (static_cast<char32_t>(p[2] & 0x3F) << 6)
                | (static_cast<char32_t>(p[3] & 0x3F));
                return code_point;
            }
            default: return U'\xFFFD';
        }
    }

    // 重载 a[] 操作符
    Char operator[](size_t index) const {
        char8_t* cursor = p;
        for (size_t i = 0; i < index; ++i) {
            cursor += get_width(*cursor);
        }
        return decode(cursor);
    }


    static size_t encode_width(Char unicode) {
        if (unicode <= 0x7F) {
            return 1;
        } else if (unicode <= 0x7FF) {
            return 2;
        } else if (unicode <= 0xFFFF) {
            return 3;
        } else if (unicode <= 0x10FFFF) {
            return 4;
        }
    }

    // 返回一个 UTF8 编码后的 Unicode
    // 并写入相应的地址
    static void encode(Char unicode, pchar8 ptr){
        size_t length = encode_width(unicode);
        char8_t* utf8 = ptr;
        if (length == 1) {
            utf8[0] = static_cast<char8_t>(unicode);
        } else if (length == 2) {
            utf8[0] = static_cast<char8_t>(0xC0 | (unicode >> 6));
            utf8[1] = static_cast<char8_t>(0x80 | (unicode & 0x3F));
        } else if (length == 3) {
            utf8[0] = static_cast<char8_t>(0xE0 | (unicode >> 12));
            utf8[1] = static_cast<char8_t>(0x80 | ((unicode >> 6) & 0x3F));
            utf8[2] = static_cast<char8_t>(0x80 | (unicode & 0x3F));
        } else if (length == 4) {
            utf8[0] = static_cast<char8_t>(0xF0 | (unicode >> 18));
            utf8[1] = static_cast<char8_t>(0x80 | ((unicode >> 12) & 0x3F));
            utf8[2] = static_cast<char8_t>(0x80 | ((unicode >> 6) & 0x3F));
            utf8[3] = static_cast<char8_t>(0x80 | (unicode & 0x3F));
        }
    }

};

}
