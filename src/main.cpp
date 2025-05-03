#include "endianless.h"
#include <bit>
#include <cassert>


using namespace std;
using namespace Dstring;


struct A {
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    bool same_as(const A& rhs){
        return
            a == rhs.a &&
            b == rhs.b &&
            c == rhs.c &&
            d == rhs.d;
    }
};

int main(int argc, char const *argv[]) {
    endianless<A> a = A{};
    auto b = reverse(a.get());
    endianless<A,endian::little> c(b);

    assert(a.get().same_as(c.get()));

    cout << c.get().a << endl;
    cout << c.get().b << endl;
    cout << c.get().c << endl;
    cout << c.get().d << endl;
    cout << "==============" << endl;
    return 0;
}