#include <bits/stdc++.h>
using namespace std;
#define J(a, b) if constexpr (requires { (a); }) { b; } else
#define L(d) cerr << "\e[94m" << (f++ ? d : "")
template <class T> void p(T x) {
    int f = 0;
    J(cerr << x, cerr << x)
    J(x.pop(), { struct y : T { using T::c; }; p(((y&)x).c); }) {
        cerr << '{';
        J(get<0>(x), apply([&](auto... a) { ((L(","), p(a)), ...); }, x))
        for (auto i : x) { J(begin(*begin(x)), cerr << "\n\t") L(","); p(i); }
        cerr << '}';
    }
} template <class... T>
void E(T... a) { int f = 0; ((L(" | "), p(a)), ...); cerr << "\e[m\n"; }
#define dbg(...) cerr << __LINE__ << ": [" << #__VA_ARGS__ << "] = ", E(__VA_ARGS__);