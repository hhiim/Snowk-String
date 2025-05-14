#ifndef  __LOGIC_H__
#define  __LOGIC_H__

#include <bit>

namespace Dstring {
using namespace std;

constexpr endian operator!(endian obj){
    switch (obj) {
        case endian::little:
            return endian::big;
        case endian::big:
            return endian::little;
    }
}

constexpr endian operator&&(endian X, endian Y){
    if(X == endian::native && Y == endian::native)
        return endian::native;
    else return !endian::native;
}

constexpr endian operator||(endian X, endian Y){
    if(X == endian::native || Y == endian::native)
        return endian::native;
    else return !endian::native;
}

};

#endif