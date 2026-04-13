#include "utils.hpp"

#include <vector>

long long pos_of_perm(const std::vector<int>& perm) {
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

std::vector<int> perm_at_pos(int n, long long pos) {
    if (n <= 0) return {};
    
    std::vector<int> p(n);
    std::vector<int> avail(n);
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