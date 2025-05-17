#pragma  once
#include "encode.h"
#include "endianPtr.h"
#include "instances/UTF8.h"
#include "instances/UTF16.h"
#include "instances/UTF32.h"
#include "instances/ASCII.h"


namespace Snowk{
    using namespace std;
    using pmr::memory_resource;
    using pmr::new_delete_resource;

/*
基础字符串，非运行时多态
自带 SSO，不支持 COW 
*/ 
template <
    template <endian> typename strPtr,  // 字符串编码
    endian E = endian::native,          // 字符串字节序
    Config C = defaultConfig       // 默认扩容策略
> requires Encode<strPtr>
class string {
public:
    using code = strPtr<E>;
    using unit = Unit<code>;
    Constexpr unisize = unitSize<code>;
    #define remaining ((sizeof(comm)-1)/unisize)
    
    using Alloc = pmr::polymorphic_allocator<unit>;
    Constexpr alloc = std::pmr::new_delete_resource;

    union{
        struct {
            size_t size;
            strPtr<E> data;
            memory_resource* resource;
            size_t capacity;
        } comm;
        struct{
            unit data    [remaining]{};
            char padding [sizeof(comm)-1-(remaining)*unisize]{};
            unsigned char size;
            
            bool isSSO(){
                return size >= 128;
            }
            void setSSO(bool on){
                if (on) size |= 0b10000000;
                else size &= ~0b10000000;
            }
            unsigned char getSize(){
                return size & 0b01111111;
            }
            void setSize(unsigned char N){
                size &= ~0b01111111;
                size += N;
            }
        } sso;
    };

    __always_inline
    auto SSOreload(auto F1, auto F2, auto&&... args){
        return sso.isSSO() ? F1(args...) : F2(args...);
    }


    string(){};  // 平凡构造函数

    // 以 '\0' 判断结尾
    template<bool inplace = false>
    string(auto* s, memory_resource* resource = alloc()){
        if constexpr(inplace){
            sso.setSSO(false);
            comm.resource = resource;
            comm.data = strPtr<E>(s);
            comm.size = comm.data.size();
            comm.capacity = comm.data.size() + 1;
        }else{
            auto source = strPtr<E>(s);
            size_t units = source.size();

            sso.setSSO(units <= remaining);
            if(units <= remaining){
                std::copy(source.data(), source.data()+units, sso.data);
                sso.setSize((unsigned char)units);
            }else{
                comm.resource = resource;
                comm.capacity = C::capacity(units); //
                comm.size = units;
                // auto ptr = resource->allocate(comm.capacity*unisize);
                // comm.data = strPtr<E>(ptr);
                comm.data = resource->allocate(comm.capacity*unisize);
                std::copy(
                    source.data(),
                    source.data()+units,
                    comm.data.data()
                );
            }
        }
    };
    // 手动输入长度
    template<bool inplace = false>
    string(
        auto* s, size_t size, memory_resource* resource = alloc()){
        if constexpr(inplace){
            sso.setSSO(false);
            comm.resource = resource;
            comm.data = strPtr<E>(s);
            comm.size = size;
            comm.capacity = size;
        }else{
            sso.setSSO(size <= remaining);
            auto source = strPtr<E>(s);
            if(size <= remaining){
                std::copy(source.data(), source.data() + size, sso.data);  //
                sso.setSize(size);
            }else{
                comm.resource = resource;
                comm.capacity = C::capacity(size);
                comm.size = size;

                auto ptr = resource->allocate(comm.capacity*unisize);
                comm.data = strPtr<E>(ptr);
                std::copy(source.data(), source.data()+size, comm.data.data()); //
            }
        }
    };
    

    // 指针以及迭代器
    unit* data(){
        return SSOreload(
            [&](){ return sso.data; },
            [&](){ return comm.data.data(); }
        );
    };

    size_t size(){
        return SSOreload(
            [&](){ return sso.getSize(); },
            [&](){ return comm.size; }
        );
    };

    strPtr<E> begin(){
        return SSOreload(
            [&](){ return sso.data;  },
            [&](){ return comm.data; }
        );
    };

    strPtr<E> end(){
        unit* ptr = data();
        return ptr + size();
    };

    ~string(){
        SSOreload([](){}, [&](){
            this->comm.resource->deallocate(
                this->comm.data.data(),
                this->comm.capacity*sizeof(unit)
            );
        });
    };

    strPtr<E>::Char operator[](size_t i){
        return begin()[i];
    };

    // 拷贝构造函数 （编码相同）
    template<endian E0, Config C0>
    string(
        string<strPtr, E0, C0> &str,
        memory_resource* resource = alloc()
    ){
        str.SSOreload(
        [&](){
            sso.setSSO(true);
            sso.setSize(str.sso.getSize());

            for(size_t i = 0; i < str.sso.getSize(); i++){
                auto source = endianPtr<unit, E0>(str.sso.data) + i;
                auto target = endianPtr<unit, E> (sso.data) + i;
                *target = *source;   // 此处隐式转换
            }
        },
        [&](){
            sso.setSSO(false);
            comm.capacity = str.comm.capacity;
            comm.size = str.comm.size;
            comm.resource = resource;

            auto ptr = resource->allocate(str.comm.capacity*unisize);
            comm.data = strPtr<E>(ptr);
            for(size_t i = 0; i < str.comm.size; i++){
                auto source = endianPtr<unit, E0>(str.comm.data.data()) + i;
                auto target = endianPtr<unit, E>(comm.data.data()) + i;
                *target = *source;   // 此处隐式转换
            }
        });
    };

    // 拷贝构造函数（编码不同）
    template<template <endian> typename strPtr0, endian E0, Config C0>
    requires (!std::is_same_v<strPtr0<E0>, strPtr<E0>>)
    string (
        string<strPtr0, E0, C0> &obj,
        memory_resource* resource = alloc()
    ){
        size_t r = 0;
        for(auto c: obj) { r += code::encode_width(c); }
        bool couldSSO = (r <= remaining * unisize);
        endianPtr<unit, E> ptr = nullptr;

        if(couldSSO){
            sso.setSSO(true);
            size_t obj_unisize = sizeof(obj.sso.data[0]);
            sso.setSize(r/unisize);
            ptr = endianPtr<unit, E>(sso.data);
        }else{
            sso.setSSO(false);
            comm.size = r/unisize;
            comm.capacity = C::capacity(comm.size);
            comm.resource = resource;
            comm.data = resource->allocate(comm.capacity*unisize);
            ptr = comm.data.data();
        }
        // TODO：对于兼容的编码（如UTF-8 和 ASCII），可以优化为直接拷贝
        for(auto c: obj) {
            code::encode(c, ptr);
            ptr += (code::encode_width(c) / unisize);
        }
    };

    // 移动构造函数（编码/端序相同 && 不使用 SSO）
    template<Config C0> string (
        string<strPtr, E, C0> &&obj,
        memory_resource* resource = alloc()
    ){
        obj.SSOreload([&](){
            sso.setSSO(true);
            auto size = obj.sso.getSize();
            sso.setSize(size);
            std::copy(obj.sso.data, obj.sso.data + size, sso.data);
        },[&](){
            sso.setSSO(false);
            comm.capacity = obj.comm.capacity;
            comm.size = obj.comm.size;
            comm.data = obj.comm.data;
            obj.comm.data = code(nullptr);
        });
    };

    // std::cout 支持
    friend std::ostream& operator<<(std::ostream& os, string& str){
        auto u8str = string<UTF8>(str);
        os  << (unsigned char*)u8str.data();
        return os;
    };
};

}
#undef remaining
#undef Constexpr
