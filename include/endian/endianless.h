#ifndef ENDIANLESS_H
#define ENDIANLESS_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <sys/cdefs.h>
#include "interpret.h"


namespace Dstring {

using std::is_convertible_v;
using std::endian;
using std::byte;


//  字节序转换
template<typename T, endian E = endian::native>
__always_inline auto endianCons(T& data) {
    constexpr size_t size = sizeof(T);
    byte* source = become<byte*>(&data);
    bytes<size> target;
    if constexpr (E == endian::native) {
        std::copy(
            source, source+size,
            target.begin()
        );
    }else {
        std::reverse_copy(
            source, source+size,
            target.begin()
        );
    } return target;
}

// 端序无关数据类型（显示指明读写端序）
template <typename T, endian E = endian::native>
class endianless{
public:
    bytes<sizeof(T)> data;

    endianless() : data(){}
    endianless(T t) : data(
        endianCons<T, E>(t)
    ){}
    endianless(bytes<sizeof(T)> data): data(data){}

    // 赋值构造
    template<typename U>
    void operator=(const U& u){
        if constexpr (is_convertible_v<U,T>) {
            auto temp = (T)u;
            data = endianCons<T,E>(temp);
        } else {
            T* p = become<T*>(&u);
            data = endianCons<T,E>(*p);
        }
    }

    __always_inline T get() const{
        if constexpr(E == endian::native){
            return *become<T*>(&data[0]);
        }else{
            auto newArray = reverse(data);
            T* p = become<T*>(&newArray[0]);
            return *p;
        }
    }

    // 强制转换
    template<typename U>
    __always_inline  operator U() const{
        if constexpr (is_convertible_v<U,T>) {
            return static_cast<U>(get());
        } else{
            static_assert(sizeof(T) == sizeof(U));
            T t = get();
            return *(become<U*>(&t));
        }
    }

    template <typename U>
    __always_inline U staticCast(){
        return U(this->get());
    }
};

// endianless 萃取模板
template <typename T> struct EndianLess;
template <typename D, endian _E>
struct EndianLess<endianless<D,_E>>{
    using Type = D;
    static constexpr auto E = _E;
};

template <typename T>
concept isEndianless = requires { EndianLess<T>{}; };

# include "operations.h"

binOp(+); binOp(-); binOp(*); binOp(/);
binOp(&); binOp(|); binOp(^); binOp(%);
binOp(>>); binOp(<<);

placeOp(+=); placeOp(-=); placeOp(*=); placeOp(/=);
placeOp(&=); placeOp(|=); placeOp(^=); placeOp(%=);

cmpOp(==); cmpOp(<);  cmpOp(>);
cmpOp(!=); cmpOp(<=); cmpOp(>=);



};

#undef become
#endif