#include <bits/stdc++.h>
#include "common.hpp"
using namespace std;

const int MAX_PERM     = 7654321;
const int FAC_SEVEN    = 5040;
const int TOTAL_STATES = FAC_SEVEN * (3*3*3*3*3*3*3);

array<int, MAX_PERM + 1> idx;
array<int, FAC_SEVEN> perm;  

void populate_idx_perm() {
    array<int, 7> xs = {1, 2, 3, 4, 5, 6, 7};
    int cnt = 0;
    
    do {
        int num = permutation_to_num(xs);
        perm[cnt] = num;
        idx[num] = cnt;
        ++cnt;
    } while (next_permutation(xs.begin(), xs.end()));
}

int apply_transformation(const array<int, 7>& order, int num) {
    array<int, 7> perm = num_to_permutation(num);
    array<int, 7> new_perm;
    for (int i = 0; i < 7; ++i)
        new_perm[i] = perm[order[i]];
    return permutation_to_num(new_perm);
}

const array<int, 7> ROTATE_X = {4, 1, 2, 3, 5, 6, 7};
const array<int, 7> ROTATE_Y = {1, 5, 2, 4, 6, 3, 7};
const array<int, 7> ROTATE_Z = {1, 2, 6, 3, 5, 7, 4};

int main() {
    populate_idx_perm();
    
    // O estado é assim:
    // perm .. .. .. .. .. .. .. onde .. é uma bitmask pro eixo da i-ésima peça.
    array<int, TOTAL_STATES> pre = {-1};
    array<int, TOTAL_STATES> move;
    
    queue<int> q;
    q.emplace();
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        int cnt = 0;
        for (const auto& order : {ROTATE_X, ROTATE_Y, ROTATE_Z}) {
            int v = apply_transformation(order, u);
            if (pre[v] == -1) {
                pre[v] = u;
                move[v] = cnt;
                q.emplace(v);
            }
            ++cnt;
        }
    }
    
    vector<int> moves;
    int at = 0;
    while (at != 0) {
        
    }
}
