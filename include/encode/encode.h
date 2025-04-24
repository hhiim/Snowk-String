#ifndef ENCODE_H
#define ENCODE_H

#include <concepts>

namespace Dstring {
    using namespace std;
    using unicode = char32_t;

    // 每一个特定的编码方式，是一种指针
    // 它本身不对操作进行边界检查
    template<typename T>
    concept Encode = requires
    (char32_t c, T a, decltype(T::p) t) {
        a++;  ++a;        // 前向移动
        a--;  --a;        // 后向移动
        *a;    a[0];      // 读取
        a != a;  a == a;  // 同一性
        { a.size() }        -> same_as<size_t>;
        { a.len() }         -> same_as<size_t>;
        { T::decode(t) }    -> same_as<char32_t>;
        { T::encode(c, t) };
    };
};

# endif