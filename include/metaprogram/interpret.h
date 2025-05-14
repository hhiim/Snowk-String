#pragma oonce
#include <algorithm>
#include <cstddef>
#include <concepts>
#include <bitset>
#include <sys/cdefs.h>

namespace Snowk::internal{

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
};