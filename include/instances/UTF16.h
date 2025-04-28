#ifndef UTF_16_H
#define UTF_16_H

#include <memory>
#include "endianless.h"
#include "static_cast.h"

namespace Dstring {
using namespace std;

template<endian read = E, endian write = E>
class UTF16 {
    using char16 = endianless<char16_t, read, write>;
    using pchar16 = endianPtr<char16_t, read, write>;
    using Char = endianless<char32_t, read, write>;
    using pChar = endianPtr<char32_t, read, write>;

    static const bool fix = false;
    pchar16 p;

public:
    UTF16(char16_t* ptr) : p(ptr) {}
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

    char32_t operator*() const {
        return decode(p);
    }

    bool operator==(const UTF16& other) const {
        return p == other.p;
    }

    size_t len() const {
        char16_t* cursor = p;
        size_t sum = 0;
        while (*cursor != u'\0') {
            sum += 1;
            cursor += get_width(*cursor);
        }
        return sum;
    }
    size_t size() const {
        char16_t* cursor = p;
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
    // ! 应该没什么问题
    static size_t get_width(char16 unit) {
        if (unit < 0xD800 || unit > 0xDFFF) return 1; // BMP 字符
        if (unit >= 0xD800 && unit <= 0xDBFF) return 2; // 高位代理
        return 1; // 无效或低位代理
    }

    static Char decode(const pchar16 p) {
        char16 unit = *p;
        if (unit < 0xD800 || unit > 0xDFFF) { // BMP 字符
            return static_cast<char32_t>(unit);
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

    static void encode(Char unicode, char16* ptr) {
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