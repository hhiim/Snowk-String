#include "Dstring.hpp"

#include <cassert>
#include <cstddef>
#include <new>
#include <utility>
#include <vector>
using namespace Dstring;

int main() {
    char8_t s[] = 
        u8"你好世界 你好世界 你好世界 你好世界 你好世界 你好世界 你好世界 你好世界"
    ;
    base::Dstring<UTF8> a(s);
    return 0;
}