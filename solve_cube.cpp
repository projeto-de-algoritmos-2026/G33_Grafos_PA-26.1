#include "read_cube.hpp"
#include "dbg/dbg.h"
#include "utils.hpp"

#include <iostream>
#include <array>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

constexpr int NODES = 11022480;  // 7! . 3⁷

// Novas posições da permutação depois de rotacionar os eixos.
constexpr array<array<int, 7>, 6> ROTATION_TRANSFORMATIONS = {{
    {0,1,2,5,3,6,4}, {0,2,6,3,4,1,5}, {4,1,0,3,6,5,2},
    {0,1,2,4,6,3,5}, {0,5,1,3,4,6,2}, {2,1,6,3,0,5,4},
}};

constexpr array<string, 6> ROTATION_NAMES = {
    "X Anti-Horário", "Y Anti-Horário", "Z Anti-Horário",
    "X Horário",      "Y Horário",      "Z Horário",
};

// Novas eixos depois de rotacionar os eixos.
constexpr array<int, 3> AXIS_TRANSFORMATIONS = {3, 2, 1}; 

int apply_transformations(int u, const array<int, 7>& rotation_transformation, int axis_transformation) {
    array<int, 7> old_axes, new_perm;
    
    auto [perm, axis_mask_base3] = uncompress_state(u);
    
    for (int i = 0; i < 7; ++i) {
        old_axes[i] = axis_mask_base3 % 3;
        axis_mask_base3 /= 3;
    }
    
    int new_axis_mask_base3 = 0, cur_pow3 = 1;
    
    for (int i = 0; i < 7; ++i) {
        int j = rotation_transformation[i];
        new_perm[i] = perm[j];
        
        int cur_axis = old_axes[j];
        
        if (j != i && (cur_axis + axis_transformation != 3))
            cur_axis ^= axis_transformation;
        
        new_axis_mask_base3 += cur_axis * cur_pow3;
        cur_pow3 *= 3;
    }
    
    return compress_state(new_perm, new_axis_mask_base3);
}

int main() {
    int initial = read_cube();
    
    vector<int> pre(NODES, -1);  // Tem que ser na heap porque é muito grande.
    array<char, NODES> move;
    
    queue<int> q;
    q.emplace(initial);
    pre[initial] = -2;
    
    int states_dbg = 0;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int i = 0; i < 6; ++i) {
            int v = apply_transformations(u, ROTATION_TRANSFORMATIONS[i], AXIS_TRANSFORMATIONS[i % 3]);
            if (pre[v] == -1) {
                q.emplace(v);
                pre[v] = u;
                move[v] = i;
            }
        }
        
        states_dbg++;
    }
    // dbg(states_dbg);
    
    int cur = 0;
    if (pre[cur] == -1) {
        cout << "Cubo impossível\n";
        return 0;
    }
    
    vector<int> rotations;
    while (cur != initial) {
        rotations.emplace_back(move[cur]);
        cur = pre[cur];
    }
    reverse(rotations.begin(), rotations.end());

    cout << "Os eixos que vão ser descritos são os opostos ao da peça GRY\n"
         << "Onde a direção da cor verde é X, a direção da cor vermelha é Y e a direção da cor amarelo é Z.\n\n"
         << "Serão necessários " << rotations.size() << " passos:\n\n";
    for (int rotation : rotations)
        cout << "Rotacione o eixo " << ROTATION_NAMES[rotation] << '\n';
}