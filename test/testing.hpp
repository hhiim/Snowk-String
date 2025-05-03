#ifndef  TESTING_H
#define  TESTING_H

#include "encode.h"
#include "catch_amalgamated.hpp"
#include <iostream>
using namespace std;
using namespace Dstring;

template <Encode E>
inline void encodeTest(char32_t c){
    char* buf = new char[4];
    E::encode(c, (void*)buf);
    auto result = E::decode((void*)buf);
    CHECK( result == c );
    delete [] buf;
}

#endif
