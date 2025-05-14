    // // 类型转换偏特化
    // template<typename T>
    // operator T();

    // template<endian E0, Config C = config>
    // operator Dstring<strPtr, E0, C>(){
    //     SSOreload(
    //     [&](){
    //         auto reverse = Dstring<strPtr, E0, C>();
    //         reverse.sso.setSSO(true);
    //         reverse.sso.setSize(sso.getSize());

    //         for(size_t i = 0; i < sso.getSize(); i++){
    //             unit* source = sso.data + i;
    //             auto target = reverse.sso.data + i;
    //             *target = *source;   // 此处隐式转换
    //         }
    //     },
    //     [&](){
    //         auto reverse = Dstring<strPtr, !E, C>();
    //         reverse.sso.setSSO(false);
    //         reverse.comm.capacity = comm.capacity;
    //         reverse.comm.size = comm.size;

    //         for(size_t i = 0; i < comm.size; i++){
    //             unit* source = comm.data.data() + i;
    //             auto target = reverse.sso.data + i;
    //             *target = *source;   // 此处隐式转换
    //         }
    //     });
    // }

    // template<template <endian> typename strPtr0, endian E0, Config C0>
    // operator Dstring<strPtr0, E0, C0>(){
    //     static_assert(!is_same_v<strPtr0<E>, strPtr<E>>);
    // }

#ifndef CONVERT_HPP
#define CONVERT_HPP
#pragma  once

#include "Dstring.hpp"

#endif
