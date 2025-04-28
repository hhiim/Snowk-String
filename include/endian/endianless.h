#ifndef ENDIANLESS_H
#define ENDIANLESS_H

#include <iostream>
#include <memory>


namespace Dstring {
using namespace std;

const auto E = endian::native;

//  字节序转换
template<typename T>
__always_inline T endianTransfer(T data) {
    size_t size = sizeof(T);
    char* p = (char*)&data;
    char* q = new char[size];
    for (size_t i = 0; i < size; i++) {
        q[i] = p[size - 1 - i];
    }
    T result = *(T*)q;
    delete[] q;
    return result;
}

template<typename T>
void endianTransfer(T* data, size_t n) {
    for (size_t i = 0; i < n; i++) {
        data[i] = endianTransfer(data[i]);
    }
}

// 端序无关数据类型
// 内部统一使用机器端序，读写时转换为设定端序
template <typename T, endian read, endian write>
class endianless{
public:
    T& data;
    endianless(T& data) : data(data) {}
    endianless(T&& old) noexcept : data(std::move(old)) {} 

    // 读取
    template<typename U>
    void operator=(const U& u){
        bool consist = (read == endian::native);
        if constexpr (is_convertible_v<U,T>) {
            T temp = (T)u;
            data = consist ? temp : endianTransfer(temp);
        } else {
            void* p = (void*)u;
            T temp = *(T*)p;
            data = consist ? temp : endianTransfer(temp);
        }
    }

    // 写入
    template<typename U>
    operator U(){
        bool consist = (write == endian::native);
        if constexpr (is_convertible_v<T,U>) {
            U temp = (U)data;
            return consist ? temp : endianTransfer(temp);
        } else {
            void* p = (void*)data;
            U temp = *(U*)p;
            return consist ? temp : endianTransfer(temp);
        }
    }

    template <typename U>
    auto staticCast(){
        endianless<U,read,write>(
            static_cast<U>(data)
        );
    }
};

// 判断是否属于 endianless
namespace{
    template <typename D, endian R, endian W>
    void f(endianless<D,R,W> t){};
}
template <typename T>
concept isEndianless = requires(T t) { f(t); };

# include "operations.h"

};
# include "endianPtr.h"

#endif