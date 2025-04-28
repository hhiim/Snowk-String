#ifndef OPERATIONS_H
#define OPERATIONS_H

// 对于二元运算：
// 两者都是 endianless 情况
// 1. 读写端序一致，返回数据随运算后类型
// 2. 读写端序不一致，不匹配
// 只有一个 endianless 的情况，就地运算并返回 endianless


# define binOp(op) \
    template <\
        typename D1, typename D2,\
        endian R, endian W>\
    auto operator op(\
        endianless<D1,R,W>& lhs,\
        endianless<D2,R,W>& rhs\
    ){\
        using ret_D = decltype(rhs.data op lhs.data);\
        return endianless<ret_D,R,W>(lhs.data op rhs.data);\
    };\
    template <typename D, endian R, endian W>\
    auto operator op(endianless<D,R,W>& lhs, auto rhs){\
        lhs.data = (D)(lhs.data op rhs);\
        return lhs;\
    };\
    template <typename D, endian R, endian W>\
    auto operator op(auto lhs, endianless<D,R,W>& rhs){\
        rhs.data = (D)(rhs.data op lhs);\
        return rhs;\
    };
binOp(+); binOp(-); binOp(*); binOp(/);
binOp(&); binOp(|); binOp(^); binOp(%);

// 就地运算
# define placeOp(op) \
    template <typename T, typename D, endian R, endian W>\
    auto operator op(endianless<D,R,W>& lhs, T rhs){\
        if constexpr(isEndianless<T>){\
            lhs.data op rhs.data;\
            return lhs;\
        }else{ lhs.data op rhs; }\
    };
placeOp(+=); placeOp(-=); placeOp(*=); placeOp(/=);
placeOp(&=); placeOp(|=); placeOp(^=); placeOp(%=);

// 比较运算
# define cmpOp(op) \
    template <typename X, typename Y>\
    bool operator op(X lhs, Y rhs){\
        if constexpr (isEndianless<X> && isEndianless<Y>)\
            return lhs.data op rhs.data;\
        if constexpr (isEndianless<X>)\
            return lhs.data op rhs;\
        if constexpr (isEndianless<Y>)\
            return lhs op rhs.data;\
        return lhs op rhs;\
    };
cmpOp(==); cmpOp(<);

#endif