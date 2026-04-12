#include "common.hpp"

#include <array>

int permutation_to_num(const std::array<int, 7>& perm) {
    int res = 0, p = 1;
    for (int i = perm.size() - 1; i >= 0; --i, p *= 10)
        res += perm[i] * p;
    return res;
}

std::array<int, 7> num_to_permutation(int num) {
    std::array<int, 7> res;
    int cnt = 6;
    while (num) {
        int d = num % 10;
        res[cnt] = d;
        --cnt;
        num /= 10;
    }
    return res;
}