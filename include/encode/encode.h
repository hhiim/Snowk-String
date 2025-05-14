#pragma oonce
#include "common.h"
#include "endianless.h"
#include <cstddef>
#include <sys/cdefs.h>
#include <type_traits>

namespace Dstring {
    using namespace std;
    using namespace Dstring::internal;
    using unicode = char32_t;

    // 每一个特定的编码方式，是一种指针
    // 它本身不对操作进行边界检查
    template<template <endian> typename T, endian E>
    concept _Encode = requires
    (T<E>::Char c, T<E> a, decltype(T<E>::p) t) {
        a++;  ++a;        // 前向移动
        a--;  --a;        // 后向移动
        *a;    a[0];      // 读取
        a != a;  a == a;  // 同一性
        { a.size() }        -> same_as<size_t>;
        { a.len() }         -> same_as<size_t>;
        { T<E>::decode(t) } -> same_as<typename T<E>::Char>;
        { T<E>::encode(c, t) };
    };

    template<template <endian> typename T>
    concept Encode = requires{
        _Encode<T, endian::little> || _Encode<T, endian::big>;
    };

    template <typename Code>
    using Unit = EndianLess<typename remove_reference
    <decltype(*std::declval<Code>().p)>
    ::type>::Type;

    template <typename Code>
    constexpr size_t unitSize = sizeof(Unit<Code>);


    template <typename C>
    concept Config = requires{
        {C::capacity(1)} -> same_as<size_t>;
    };

    // 以 2 的次幂为最小容量
    struct defaultConfig{
        __always_inline
        static size_t capacity(size_t n) {
            return 1 << (sizeof(size_t) * 8 - __builtin_clzl(n - 1));
        }
    };

};
