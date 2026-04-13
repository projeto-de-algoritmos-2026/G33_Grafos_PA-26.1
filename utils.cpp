#include "utils.hpp"
// #include "dbg/dbg.h"

#include <cassert>
#include <vector>
using namespace std;

long long pos_of_perm(const vector<int>& perm) {
    int n = perm.size();
    if (n <= 1) return 0;
    
    long long res = 0, fact = 1;
    for (int i = n - 2; i >= 0; --i) {
        int inv = 0;
        for (int j = i + 1; j < n; ++j)
            if (perm[j] < perm[i]) inv++;
        res += inv * fact;
        fact *= (n - i);
    }
    return res;
}

vector<int> perm_at_pos(int n, long long pos) {
    vector<int> p(n);
    vector<int> avail(n);
    long long fact = 1;
    
    for (int i = 0; i < n; ++i) {
        avail[i] = i;
        if (i > 0) fact *= i;
    }
    
    for (int i = 0; i < n - 1; ++i) {
        long long inv = pos / fact;
        pos %= fact;
        p[i] = avail[inv];
        avail.erase(avail.begin() + inv); 
        fact /= (n - 1 - i);
    }
    p[n - 1] = avail[0];
    
    return p;
}

// Comprime as informações nesse número de base 2187
// porque 3^7 = 2187 que é o maior valor pro axis_mask_base3.
int compress_state(const vector<int>& perm, int axis_mask_base3) {
    return pos_of_perm(perm) * 2187 + axis_mask_base3;
}

pair<vector<int>, int> uncompress_state(int state) {
    int perm = state / 2187;
    int axis_mask_base3 = state % 2187;
    return {perm_at_pos(7, perm), axis_mask_base3};
}