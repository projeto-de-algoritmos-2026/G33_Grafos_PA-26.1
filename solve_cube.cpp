#include "read_cube.hpp"
// #include "dbg/dbg.h"
#include "utils.hpp"

#include <iostream>
#include <array>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

constexpr int NODES = 11022480;  // 7! . 3⁷

// Novas posições da permutação depois de rotacionar os eixos.
const array<array<int, 7>, 6> ROTATION_TRANSFORMATIONS = {{
    {0,1,2,5,3,6,4},  // x anti horário
    {0,2,6,3,4,1,5},  // y anti horário
    {4,1,0,3,6,5,2},  // z anti horário
    {0,1,2,4,6,3,5},  // x horário
    {0,5,1,3,4,6,2},  // y horário
    {2,1,6,3,0,5,4}   // z horário
}};

const array<string, 6> ROTATION_NAMES = {
    "X Anti-Horário",
    "Y Anti-Horário",
    "Z Anti-Horário",
    "X Horário",
    "Y Horário",
    "Z Horário",
};

// Novas eixos depois de rotacionar os eixos.
const array<int, 3> AXIS_TRANSFORMATIONS = {{
    3,  // x
    2,  // y
    1,  // z
}}; 

int apply_transformations(int u, const array<int, 7>& rotation_transformation, int axis_transformation) {
    int cur_base3 = 1;
    auto [perm, axis_mask_base3] = uncompress_state(u);
    int new_axis_mask_base3 = axis_mask_base3;
    vector<int> new_perm(7);
    
    for (int i = 0; i < 7; ++i) {
        new_perm[i] = perm[rotation_transformation[i]];
        
        if (rotation_transformation[i] != i) {
            int cur_axis = axis_mask_base3 % 3;
            if (!((cur_axis == 0 && axis_transformation == 3) ||
                (cur_axis == 1 && axis_transformation == 2) ||
                (cur_axis == 2 && axis_transformation == 1)
            )) {
                new_axis_mask_base3 -= cur_axis * cur_base3;
                new_axis_mask_base3 += (cur_axis ^ axis_transformation) * cur_base3;
            }
        }
        
        axis_mask_base3 /= 3;
        cur_base3 *= 3;
    }
    
    return compress_state(new_perm, new_axis_mask_base3);
}

int main() {
    int initial = read_cube();
    
    vector<int> pre(NODES, -1), move(NODES);
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
    }
    
    
    vector<int> rotations;
    int cur = 0;
    if (pre[cur] == -1) {
        cout << "Cubo impossível\n";
        return 0;
    }
    
    while (cur != initial) {
        rotations.emplace_back(move[cur]);
        cur = pre[cur];
    }
    reverse(rotations.begin(), rotations.end());

    cout << "Serão necessários " << rotations.size() << " passos:\n";
    for (int rotation : rotations)
        cout << "Rotacione o eixo " << ROTATION_NAMES[rotation] << '\n';
}