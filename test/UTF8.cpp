#include "UTF8.h"
#include "string.hpp"
#include "catch_amalgamated.hpp"
#include "endianPtr.h"
#include "endianless.h"
#include <bit>
#include <iostream>
#include <tuple>
using namespace std;
using namespace Snowk;
using namespace Snowk::internal;

TEST_CASE("UTF8"){
    const auto [input, expected,length] = GENERATE(
        tuple{u8"Hello World!"       , U"Hello World!"      , 12},
        tuple{u8"你好，世界！"       , U"你好，世界！"      , 6},
        tuple{u8"∀∂∃∈∉∋∏∑⨌", U"∀∂∃∈∉∋∏∑⨌", 9}
    );
    SECTION("decode"){
        bool allPassed = true;
        auto str = UTF8((char8_t*)input);
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
            char8_t out[4];
            UTF8<>::encode(expected[i], out);
            auto r = UTF8(out);
            bool test = (*r == expected[i]);
            CHECK(test);
        }
    }
    SECTION("index"){
        auto str = Snowk::string<UTF8>((char8_t*)input);
        for(int i = 0; i < length; i++){
            bool test = str[i] == expected[i];
            CHECK(test);
        }
    }
}