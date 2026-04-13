#include "read_cube.hpp"
#include "dbg/dbg.h"
#include "utils.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <array>
using namespace std;

struct Coords { int x, y, z; };

int id_of_piece(const string& s) {
    static const array<string, 7> arr = {"GRW", "GOY", "GOW", "BRY", "BRW", "BOY", "BOW"};
    auto it = find(arr.begin(), arr.end(), s);
    return it == arr.end() ? -1 : (int)(it - arr.begin());
}

const Coords COORDS[6][4] = {
    {{0,0,0}, {0,1,0}, {0,0,1}, {0,1,1}},
    {{0,1,0}, {1,1,0}, {0,1,1}, {1,1,1}},
    {{1,1,0}, {1,0,0}, {1,1,1}, {1,0,1}},
    {{1,0,0}, {0,0,0}, {1,0,1}, {0,0,1}},
    {{1,0,0}, {1,1,0}, {0,0,0}, {0,1,0}},
    {{0,0,1}, {0,1,1}, {1,0,1}, {1,1,1}}
};
const string FACES[] = {"frontal", "direita", "traseira", "esquerda", "de cima", "de baixo"};
const int AXIS_ID[]  = {0, 1, 0, 1, 2, 2};

int read_cube() {
    cout << "\nInstruções:\n\nSelecione uma face do cubo para ser a frontal\n\n"
         << "Cores: \033[33mamarelo (Y), \033[31mvermelho (R), \033[32mverde (G), \033[38;5;208mlaranja (O), \033[34mazul (B) e \033[mbranco (W)\n\n"
         << "Para inserir as cores de uma face escreva assim: YW GG\n\n"
         << "A ordem é canto superior direito, esquerdo, desce e repete.\n\n";
         
    string colors[2][2][2];
    vector<int> axis[2][2][2];
    
    for (int i = 0; i < 6; ++i) {
        if (i < 4) cout << "Gire o cubo e insira a face ";
        else cout << "Na face frontal insira a face ";
        cout << FACES[i] << " >\n";
        
        string l1, l2; 
        cin >> l1 >> l2;
        string input = l1 + l2;
        
        for (int p = 0; p < 4; ++p) {
            auto [cx, cy, cz] = COORDS[i][p];
            colors[cx][cy][cz] += input[p];
            axis[cx][cy][cz].emplace_back(AXIS_ID[i]);
        }
    }
    
    Coords GRY = {0, 0, 0};
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k) {
                string p = colors[i][j][k];
                sort(p.begin(), p.end());
                if (p == "GRY") GRY = {i, j, k};
            }
    
    string colors_fixed[2][2][2];
    int axis_fixed[2][2][2][3];
    int trans[3]; 

    for (int ax = 0; ax < 3; ++ax) {
        char c = colors[GRY.x][GRY.y][GRY.z][ax];
        int a = axis[GRY.x][GRY.y][GRY.z][ax];
        trans[a] = (c == 'G') ? 0 : (c == 'R' ? 1 : 2);
    }
    
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k) {
                int p[3] = {i, j, k};
                int g[3] = {GRY.x, GRY.y, GRY.z};
                int l[3]; 

                for(int ax = 0; ax < 3; ++ax) 
                    l[trans[ax]] = p[ax] ^ g[ax];

                colors_fixed[l[0]][l[1]][l[2]] = colors[i][j][k];
                for (int ax = 0; ax < 3; ++ax) 
                    axis_fixed[l[0]][l[1]][l[2]][ax] = trans[axis[i][j][k][ax]];
            }

    vector<int> perm(7);
    int axis_mask_base3 = 0;
    int cur_base3 = 1;
    
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k) {
                if (i == 0 && j == 0 && k == 0) continue;
                string piece = colors_fixed[i][j][k];
                
                int main_axis = -1;
                for (int ax = 0; ax < 3; ++ax)
                    if (piece[ax] == 'B' || piece[ax] == 'G')
                        main_axis = ax;
                axis_mask_base3 += axis_fixed[i][j][k][main_axis] * cur_base3;
                cur_base3 *= 3;
                
                sort(piece.begin(), piece.end());
                perm[4*i + 2*j + k - 1] = id_of_piece(piece);
            }

    return compress_state(perm, axis_mask_base3);
}