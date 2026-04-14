#include "utils.hpp"

#include <array>
#include <span>
#include <utility>

using namespace std;

constexpr array<int, 7> FACTORIALS = {1, 1, 2, 6, 24, 120, 720};

// Permutação -> Índice no vetor ordenado
int lehmer_encode(span<const int> perm) {
    int n = perm.size();
    int res = 0;
    
    for (int i = 0; i < n - 1; ++i) {
        int inv = 0;
        for (int j = i + 1; j < n; ++j)
            if (perm[j] < perm[i]) inv++;
        res += inv * FACTORIALS[n - 1 - i];
    }
    
    return res;
}

// Índice no vetor ordenado -> Permutação
array<int, 7> lehmer_decode(int pos) {
    array<int, 7> p, avail = {0, 1, 2, 3, 4, 5, 6};
    
    for (int i = 0; i < 7; ++i) {
        int fact = FACTORIALS[6 - i];
        int inv = pos / fact;
        pos %= fact;
        p[i] = avail[inv];
        for (int j = inv; j < 6 - i; ++j)
            avail[j] = avail[j + 1];
    }
    
    return p;
}

// 2187 porque axis_mask_base3 é no máximo 3^7. Daí comprime o estado
// do cubo nesse inteiro para dar para guardar em um vetor.
int compress_state(span<const int> perm, int axis_mask_base3) {
    return lehmer_encode(perm) * 2187 + axis_mask_base3;
}

pair<array<int, 7>, int> uncompress_state(int state) {
    int perm_rank = state / 2187;
    int axis_mask_base3 = state % 2187;
    return {lehmer_decode(perm_rank), axis_mask_base3};
}