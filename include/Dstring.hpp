#pragma  once

#include "encode.h"
#include "instances/UTF8.h"
#include "instances/UTF16.h"
#include "instances/UTF32.h"
#include "instances/ASCII.h"
// #include "Convert.hpp"


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
    Config config = defaultConfig       // 默认扩容策略
> requires Encode<strPtr>
class string {
private:
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

public:
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
                comm.capacity = config::capacity(units); //
                comm.size = units;
                auto ptr = resource->allocate(comm.capacity);
                comm.data = strPtr<E>(ptr);
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
                comm.capacity = config::capacity(size);
                comm.size = size;

                auto ptr = resource->allocate(comm.capacity);
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
            [&](){ return sso.data; },
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
};

}

#undef remaining
#undef Constexpr
