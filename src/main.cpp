#include "UTF8.h"
#include "encode.h"
#include <cassert>
using namespace Dstring;

int main() {
    UTF8 utf8((char8_t*)u8"Hello, World!");
    assert(utf8[0] == U'H');
    assert(utf8[1] == U'e');
    assert(utf8[2] == U'l');
    assert(utf8[3] == U'l');

    static_assert(Encode<UTF8<>>);
}