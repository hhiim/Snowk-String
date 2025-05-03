#ifndef OPERATIONS_H
#define OPERATIONS_H

// 对于二元运算：
// 两者都是 endianless 情况
// 1. 端序一致，返回数据随运算后类型
// 2. 端序不一致，不匹配
// 只有一个 endianless 的情况，取出数据运算

# define binOp(op) \
    template <\
        typename D1, typename D2, endian E>\
    auto operator op(\
        endianless<D1,E> lhs,\
        endianless<D2,E> rhs\
    ){\
        using ret_D = decltype(rhs.get() op lhs.get());\
        return endianless<ret_D,E>(lhs.get() op rhs.get());\
    };\
    template <typename D, endian E>\
    auto operator op(endianless<D,E> lhs, auto rhs){\
        using ret_D = decltype(rhs op lhs.get());\
        return endianless<ret_D,E>(lhs.get() op rhs);\
    };\
    template <typename D, endian E>\
    auto operator op(auto lhs, endianless<D,E> rhs){\
        using ret_D = decltype(rhs.get() op lhs);\
        return endianless<ret_D,E>(rhs.get() op lhs);\
    };


// 就地运算
# define placeOp(op) \
    template <typename T, typename D, endian E>\
    auto operator op(endianless<D,E>& lhs, T rhs){\
        if constexpr(isEndianless<T>){\
            auto temp = lhs.get();\
            temp op rhs.get();\
            lhs.data = endianCons<D,E>(temp);\
            return lhs;\
        }else{\
            auto temp = lhs.get();\
            temp op rhs;\
            lhs.data = endianCons<D,E>(temp);\
            return lhs;\
        }\
    };


// 比较运算
# define cmpOp(op) \
    template <typename X, typename Y>\
    bool operator op(const X lhs, const Y rhs){\
        if constexpr (isEndianless<X> && isEndianless<Y>)\
            return lhs.get() op rhs.get();\
        if constexpr (isEndianless<X>)\
            return lhs.get() op rhs;\
        if constexpr (isEndianless<Y>)\
            return lhs op rhs.get();\
        return lhs op rhs;\
    };


#endif