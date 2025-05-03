#include "UTF16.h"
#include "catch_amalgamated.hpp"
#include "endianPtr.h"
#include "endianless.h"
#include <bit>
#include <iostream>
#include <tuple>
using namespace std;
using namespace Dstring;

// static_assert(endian::native == endian::little);

TEST_CASE("UTF16"){
    const auto [input, expected,length] = GENERATE(
        tuple{u"Hello World!"       , U"Hello World!"      , 12},
        tuple{u"你好，世界！"       , U"你好，世界！"      , 6},
        tuple{u"∀∂∃∈∉∋∏∑⨌", U"∀∂∃∈∉∋∏∑⨌", 9}
    );
    SECTION("decode"){
        bool allPassed = true;
        auto str = UTF16((char16_t*)input);
        REQUIRE(str.len() == length);
        for(int i = 0; i < length; i++){
            bool test = *str  == expected[i];
            CHECK(test);  allPassed &= test;
            str++;
        }
        REQUIRE(allPassed);
    }
    SECTION("encode"){
        for(int i = 0; i < length; i++){
            char16_t out[2];
            UTF16<>::encode(expected[i], out);
            auto r = UTF16(out);
            bool test = (*r == expected[i]);
            CHECK(test);
        }
    }
}