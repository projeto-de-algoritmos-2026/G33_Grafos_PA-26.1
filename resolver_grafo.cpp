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
    int axis_idx = idx >> 13;
    // cout << perm_idx << '\n';
    // cout << axis_idx << '\n';

    int num = perm_to_idx[perm_idx];
    array<int, 7> perm = num_to_permutation(num);
    array<int, 7> new_perm;

    array<int, 7> old_base3 = {0};
    int temp = axis_idx;
    for (int i = 0; i < 7; ++i) {
        old_base3[i] = temp % 3;
        temp /= 3;
    }

    int new_axis = 0;
    int p3 = 1;
    for (int i = 0; i < 7; ++i) {
        new_perm[i] = perm[order[i] - 1];
        int current_axis = old_base3[order[i] - 1];
        if (order[i] != i + 1) {
            if (axis == 0) {
                if (current_axis != 0) current_axis ^= 3;   // troca 1<->2
            } else if (axis == 1) {
                if (current_axis != 1) current_axis ^= 2;   // troca 0<->2
            } else { 
                if (current_axis != 2) current_axis ^= 1;   // troca 0<->1
            }
        }
        new_axis += current_axis * p3;
        p3 *= 3;
    }

    return idx_to_perm[permutation_to_num(new_perm)] + (new_axis << 13);
}    
const array<int, 7> ROTATE_X = {4, 1, 2, 3, 5, 6, 7};
const array<int, 7> ROTATE_Y = {1, 5, 2, 4, 6, 3, 7};
const array<int, 7> ROTATE_Z = {1, 2, 6, 3, 5, 7, 4};

int main() {
    cout << "Alou" << endl;
    populate_idx_perm();

    int initial = read_cube();
    cout << "Estado inicial: " << initial << '\n';

    // O estado é assim:
    // .. .. .. .. .. .. .. perm onde .. é uma bitmask em potências de 3 pro eixo da i-ésima peça.
    vector<int> pre(TOTAL_SIZE, -1);
    vector<int> move(TOTAL_SIZE);

    queue<int> q;
    q.emplace(initial);
    pre[initial] = initial;
    int states = 0;
    // cout << initial << '\n';
    bool flag = false;
    while (!q.empty()) {
        ++states;
        int u = q.front();
        q.pop();
        int cnt = 0;
        if (u == 5039){flag= true; break;}
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

    if (!flag) {
        cout << "Não foi possível encontrar uma solução." << endl;
        return 1;
    }

    vector<int> path;
    int curr = 0;
    while (curr != initial) {
        path.push_back(move[curr]);
        curr = pre[curr];
    }
    reverse(path.begin(), path.end());

    cout << "Resolvido em " << path.size() << " passos:" << endl;
    for (int m : path) {
        if (m == 0) cout << "Gire a face X" << endl;
        else if (m == 1) cout << "Gire a face Y" << endl;
        else cout << "Gire a face Z" << endl;
    }


    cout << "estados: " << states << '\n';
}
