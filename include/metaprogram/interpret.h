#ifndef INTERPRET_H
#define INTERPRET_H

#include <algorithm>
#include <cstddef>
#include <concepts>
#include <bitset>
#include <sys/cdefs.h>


template<size_t N>
using bytes = std::array<std::byte, N>;
#define become std::bit_cast

// 拷贝反转函数
template <size_t N> __always_inline
auto reverse(const bytes<N>& x){
    bytes<N> y;
    std::reverse_copy(
        x.begin(), x.end(),
        y.begin()
    ); return y;
}

//对内存数据的解释
template<typename C, size_t N>
C& interpret(bytes<N>& data){
    return *become<C*>(&data[0]);
};


template<typename T, size_t N>
struct BitView{
    bytes<N>& data;
    public:
    constexpr BitView(T& v): data(*become<bytes<N>*>(&v)){}

    bool getter(size_t M) noexcept {
        const auto* ptr = reinterpret_cast<const unsigned char*>(data.data());
        return (ptr[M/8] >> (M%8)) & 0x01;
    }

    __always_inline void setter(size_t M, bool v) noexcept {
        auto* ptr = reinterpret_cast<unsigned char*>(data.data());
        const unsigned mask = 1u << (M%8);
        ptr[M/8] = v ? (ptr[M/8] | mask) : (ptr[M/8] & ~mask);
    }
};
template<typename T>
BitView(T&) -> BitView<T, sizeof(T)>;


#endif