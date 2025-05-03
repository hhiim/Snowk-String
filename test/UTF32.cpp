#include "UTF32.h"
#include "catch_amalgamated.hpp"
#include "endianPtr.h"
#include "endianless.h"
#include <bit>
#include <iostream>
#include <tuple>
using namespace std;
using namespace Dstring;

TEST_CASE("UTF32"){
    const auto [input, expected,length] = GENERATE(
        tuple{U"Hello World!"       , U"Hello World!"      , 12},
        tuple{U"你好，世界！"       , U"你好，世界！"      , 6},
        tuple{U"∀∂∃∈∉∋∏∑⨌", U"∀∂∃∈∉∋∏∑⨌", 9}
    );
    SECTION("decode"){
        bool allPassed = true;
        auto str = UTF32((char32_t*)input);
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
            char32_t out[1];
            UTF32<>::encode(expected[i], out);
            auto r = UTF32(out);
            bool test = (*r == expected[i]);
            CHECK(test);
        }
    }
}