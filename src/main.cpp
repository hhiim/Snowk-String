#include "instances/UTF16.h"


using namespace std;
using namespace Dstring;


int main(int argc, char const *argv[]) {
    char16_t str[] = u"Hello World!";
    UTF16 utf16(str);
    return 0;
}