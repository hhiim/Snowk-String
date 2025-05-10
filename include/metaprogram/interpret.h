#ifndef INTERPRET_H
#define INTERPRET_H

#include <algorithm>
#include <cstddef>
#include <concepts>
#include <bitset>


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

// 位视图
template<typename T, size_t N>
struct BitView{
    bytes<N>& data;
    constexpr BitView(T v){ data = v; }
    
    template<size_t M>
    bool getter(){
        constexpr size_t byteOffset = M / 8;
        constexpr size_t bitOffset = M % 8;
        char&  p = ((char*)&data)[byteOffset];
        return (p >> bitOffset) & 1;
    }

    template<size_t M>
    void setter(bool v){
        constexpr size_t byteOffset = M / 8;
        constexpr size_t bitOffset = M % 8;
        char&  p = ((char*)&data)[byteOffset];
        if(v) p |= 1 << bitOffset;
        else p &= ~(1 << bitOffset);
    }
};

#endif