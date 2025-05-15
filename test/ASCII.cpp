#include "ASCII.h"
#include "string.hpp"
#include <bit>
#include <string>
#include <tuple>
#include <iostream>
#include "catch_amalgamated.hpp"
using namespace std;
using namespace Snowk;
using namespace Snowk::internal;

TEST_CASE("ASCII"){
    const auto [input, expected,length] = GENERATE(
        tuple{"Hello World!", U"Hello World!", 12}
    );
    SECTION("decode"){
        bool allPassed = true;
        auto str = ASCII((char*)input);
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
            char out[1];
            ASCII<>::encode(expected[i], out);
            auto r = ASCII(out);
            bool test = (*r == expected[i]);
            CHECK(test);
        }
    }
    SECTION("index"){
        auto str = Snowk::string<ASCII>((char*)input);
        for(int i = 0; i < length; i++){
            bool test = str[i] == expected[i];
            CHECK(test);
        }
    }
}