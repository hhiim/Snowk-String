#ifndef UTF_16_H
#define UTF_16_H

#include "common.h"
#include "static_cast.h"


namespace Dstring {
using namespace std;

template<endian E = endian::native>
class UTF16 {
    static const bool fix = false;
    typedef endianless<char32_t, E> Char;
    typedef endianPtr<char32_t, E> pChar;
    typedef endianless<char16_t, E> char16;
    typedef endianPtr<char16_t, E> pchar16;

public:
    pchar16 p;
    UTF16(char16_t* ptr) : p(ptr) {}
    UTF16(pchar16 ptr) : p(ptr.ptr) {}
    UTF16(const UTF16& obj) : p(obj.p) {}

    // ! 已修改
    UTF16& operator++() {
        p += get_width(*p);
        return *this;
    }
    UTF16 operator++(int) {
        UTF16 tmp = *this;
        ++(*this);
        return tmp;
    }

    UTF16& operator--() {
        --p;
        startByte(p);
        return *this;
    }
    UTF16 operator--(int) {
        UTF16 tmp = *this;
        --(*this);
        return tmp;
    }


    // 实现 迭代器接口
    auto begin(){ return *this; }
    auto end(){ return UTF16(p + len()); }

    Char operator*() const {
        return decode(p);
    }

    bool operator==(const UTF16& other) const {
        return p == other.p;
    }

    size_t len() const {
        auto cursor = p;
        size_t sum = 0;
        while (*cursor != u'\0') {
            sum += 1;
            cursor += get_width(*cursor);
        } return sum;
    }
    size_t size() const {
        auto cursor = p;
        size_t sum = 0;
        while (*cursor != u'\0') {
            sum += 2; // 每个 UTF-16 单元占 2 字节
            cursor++;
        }
        return sum;
    }

    static pchar16 startByte(pchar16 ptr) {
        for (int i = 0; i >= -1; i--) {
            if (ptr[i] < 0xDC00 || ptr[i] > 0xDFFF) {
                return ptr + i;
            }
        } return nullptr;
    }
    static size_t get_width(char16 unit) {
        if (unit < 0xD800 || unit > 0xDFFF) return 1; // BMP 字符
        if (unit >= 0xD800 && unit <= 0xDBFF) return 2; // 高位代理
        return 1; // 无效或低位代理
    }

    static Char decode(pchar16 p) {
        char16 unit = *p;
        if (unit < 0xD800 || unit > 0xDFFF) { // BMP 字符
            return static_cast<Char>(unit);
        } else if (unit >= 0xD800 && unit <= 0xDBFF) { // 高位代理
            char16 low = *(p + 1);
            if (low >= 0xDC00 && low <= 0xDFFF) { // 跟随一个低位代理
                return 0x10000 +
                        ((static_cast<Char>(unit - 0xD800) << 10) |
                        (static_cast<Char>(low - 0xDC00)));
            }
        }
        return Char(U'\xFFFD'); // 替代字符
    }

    Char operator[](size_t index) const {
        pchar16 cursor = p;
        for (size_t i = 0; i < index; ++i) {
            cursor += get_width(*cursor);
        }
        return decode(cursor);
    }

    static void encode(Char unicode, pchar16 ptr) {
        if (unicode <= 0xFFFF) { // BMP 字符
            ptr[0] = static_cast<char16>(unicode);
        } else if (unicode <= 0x10FFFF) { // 补充平面字符
            unicode -= 0x10000;
            ptr[0] = static_cast<char16>(0xD800 | ((unicode >> 10) & 0x3FF));
            ptr[1] = static_cast<char16>(0xDC00 | (unicode & 0x3FF));
        }
    }
};

} // namespace Dstring

#undef static_cast
#endif