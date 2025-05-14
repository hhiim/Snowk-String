#pragma oonce
#include <concepts>
#include <type_traits>
#include <memory>

template <int N>
struct reader {
    friend auto flag(reader);
};

template <int N>
struct setter {
    friend auto flag(reader<N>) {}
};

template <int N = 0, auto seed = [] {}>
consteval auto next() {
    constexpr bool exist = requires { flag(reader<N>{}); };
    if constexpr(!exist) {
        setter<N> setter;
        return N;
    } else {
        return next<N + 1>();
    }
}