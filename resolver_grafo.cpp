#include <bits/stdc++.h>
#include "common.hpp"
#include "cubo_para_grafo.hpp"
using namespace std;

const int MAX_PERM        = 7654321;
const int FAC_SEVEN       = 5040;
const int THREE_POW_SEVEN = 3*3*3*3*3*3*3;
// const int TOTAL_STATES = FAC_SEVEN * THREE_POW_SEVEN;
const int TOTAL_SIZE = 17915904;

array<int, MAX_PERM + 1> idx_to_perm;
array<int, FAC_SEVEN> perm_to_idx;

void populate_idx_perm() {
    array<int, 7> xs = {1, 2, 3, 4, 5, 6, 7};
    int cnt = 0;

    do {
        int num = permutation_to_num(xs);
        perm_to_idx[cnt] = num;
        idx_to_perm[num] = cnt;
        ++cnt;
    } while (next_permutation(xs.begin(), xs.end()));
}

vector<int> to_base(int x, int b) {
    assert(b > 1);
    vector<int> res;
    while (x) res.emplace_back(x % b), x /= b;
    reverse(res.begin(), res.end());
    return res;
}

int apply_transformation(const array<int, 7>& order, int idx, int axis) {
    int perm_idx = idx & ((1 << 13) - 1);
    int axis_idx = idx - perm_idx;
    // cout << perm_idx << '\n';
    // cout << axis_idx << '\n';

    int num = perm_to_idx[perm_idx];
    array<int, 7> perm = num_to_permutation(num);
    array<int, 7> new_perm;

    auto base3 = to_base(axis_idx, 3);
    // cout << base3.size() << '\n';
    int mask_axis = 0;
    int p3 = 1;

    for (int i = 0; i < 7; ++i) {
        // cout << new_perm.size() << '\n';
        // cout << perm.size() << '\n';
        // cout << "order " << order[i] << '\n';
        new_perm[i] = perm[order[i] - 1];
        // cout << "AAAA\n";
        // cout << i << '\n';

        // cout << "base3.size(): " << base3.size() << '\n';
        if (axis == 0 && i < base3.size() && base3[i] != 0)
            base3[i] ^= 3;
        else if (axis == 1 && i < base3.size() && base3[i] != 1)
            base3[i] ^= 2;
        else if (axis == 2 && i < base3.size() && base3[i] != 2)
            base3[i] ^= 1;

        if (i < base3.size())
            mask_axis += (base3[i] << 13) * p3;
        else {
            if (axis == 1) 
                mask_axis += (2 << 13) * p3;
            else 
                mask_axis += (1 << 13) * p3;
        }
        
        // cout << mask_axis << '\n';
        p3 *= 3;
    }

    // cout << mask_axis << '\n';
    return idx_to_perm[permutation_to_num(new_perm)] + mask_axis;
}

const array<int, 7> ROTATE_X = {4, 1, 2, 3, 5, 6, 7};
const array<int, 7> ROTATE_Y = {1, 5, 2, 4, 6, 3, 7};
const array<int, 7> ROTATE_Z = {1, 2, 6, 3, 5, 7, 4};

int main() {
    cout << "Alou" << endl;
    populate_idx_perm();

    int initial = read_cube();

    // O estado é assim:
    // .. .. .. .. .. .. .. perm onde .. é uma bitmask em potências de 3 pro eixo da i-ésima peça.
    vector<int> pre(TOTAL_SIZE, -1);
    vector<int> move(TOTAL_SIZE);

    queue<int> q;
    q.emplace(initial);
    int states = 0;
    // cout << initial << '\n';
    while (!q.empty()) {
        ++states;
        int u = q.front();
        q.pop();
        int cnt = 0;
        for (const auto& order : {ROTATE_X, ROTATE_Y, ROTATE_Z}) {
            int v = apply_transformation(order, u, cnt);
            if (pre[v] == -1) {
                pre[v] = u;
                move[v] = cnt;
                q.emplace(v);
            }
            ++cnt;
            // cout << cnt << endl;
        }
    }

    vector<int> moves;
    int at = 7654321;
    while (at != initial) {
        moves.emplace_back(move[at]);
        at = pre[at];
    }
    reverse(moves.begin(), moves.end());

    cout << "Serão " << moves.size() << " passos:\n";
    for (int move : moves) {
        if (move == 0) cout << "Rotacione o eixo X\n";
        else if (move == 1) cout << "Rotacione o eixo Y\n";
        else cout << "Rotacione o eixo Z\n";
    }

    cout << "estados: " << states << '\n';
}
