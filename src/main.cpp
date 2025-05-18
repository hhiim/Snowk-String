#include "string.hpp"
#include "UTF16.h"
#include <cassert>
#include <cstddef>
#include <fstream>

using namespace std;
using namespace Snowk;
using namespace Snowk::internal;

int main() {
    Snowk::string<UTF8>  test1 ((char8_t*) u8"To be or not to be, that is the question.");
    Snowk::string<UTF16> test2 ((char16_t*) u"To be or not to be, that is the question.");
    Snowk::string<UTF32> test3 ((char32_t*) U"To be or not to be, that is the question.");
    Snowk::string<ASCII> test4 (   (char*)   "To be or not to be, that is the question.");
    std::cout <<  test1 << std::endl;
    std::cout <<  test2 << std::endl;
    std::cout <<  test3 << std::endl;
    std::cout <<  test4 << std::endl;
}
