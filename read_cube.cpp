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
    // Não está em ordem alfabética porque eu puis de acordo com as coordenadas
    // e os loops para a permutação resolvida ser 0123456.
    array<string, 7> arr = {"GRW", "GOY", "GOW", "BRY", "BRW", "BOY", "BOW"};
    int id = find(arr.begin(), arr.end(), s) - arr.begin();
    return id == arr.size() ? -1 : id;
}

int read_cube() {
    cout << "\nInstruções:\n\nSelecione uma face do cubo para ser a frontal\n\n"
         << "Cores: \033[33mamarelo (Y), \033[31mvermelho (R), \033[32mverde (G), \033[38;5;208mlaranja (O), \033[34mazul (B) e \033[mbranco (W)\n\n"
         << "Para inserir as cores de uma face escreva assim: YW GG\n\n"
         << "A ordem é canto superior direito, esquerdo, desce e repete.\n\n";
         
    // Lê as peças numa matriz 3d 2x2x2.
    string colors[2][2][2];
    vector<int> axis[2][2][2];
    
    Coords coords[6][4] = {
        {{0,0,0}, {0,1,0}, {0,0,1}, {0,1,1}},
        {{0,1,0}, {1,1,0}, {0,1,1}, {1,1,1}},
        {{1,1,0}, {1,0,0}, {1,1,1}, {1,0,1}},
        {{1,0,0}, {0,0,0}, {1,0,1}, {0,0,1}},
        {{1,0,0}, {1,1,0}, {0,0,0}, {0,1,0}},
        {{0,0,1}, {0,1,1}, {1,0,1}, {1,1,1}}
    };
    string faces[] = {"frontal", "direita", "traseira", "esquerda", "de cima", "de baixo"};
    int axis_id[]  = {0, 1, 0, 1, 2, 2};
    
    for (int i = 0; i < 6; ++i) {
        if (i < 4) cout << "Gire o cubo e insira a face ";
        else cout << "Na face frontal insira a face ";
        cout << faces[i] << " >\n";
        string l1, l2; 
        cin >> l1 >> l2;
        colors[coords[i][0].x][coords[i][0].y][coords[i][0].z] += l1[0];
        colors[coords[i][1].x][coords[i][1].y][coords[i][1].z] += l1[1];
        colors[coords[i][2].x][coords[i][2].y][coords[i][2].z] += l2[0];
        colors[coords[i][3].x][coords[i][3].y][coords[i][3].z] += l2[1];
        axis[coords[i][0].x][coords[i][0].y][coords[i][0].z].emplace_back(axis_id[i]);
        axis[coords[i][1].x][coords[i][1].y][coords[i][1].z].emplace_back(axis_id[i]);
        axis[coords[i][2].x][coords[i][2].y][coords[i][2].z].emplace_back(axis_id[i]);
        axis[coords[i][3].x][coords[i][3].y][coords[i][3].z].emplace_back(axis_id[i]);
    }
    
    Coords GRY = {0, 0, 0};
    
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k) {
                string piece = colors[i][j][k];
                sort(piece.begin(), piece.end());
                if (piece == "GRY") GRY = {i, j, k};
            }
    
    // Fixa a peça GRY como (0, 0, 0), arruma as outras.  
    // Ela sempre tá na face frontal, mas não necessariamente no (0, 0, 0). 
    string colors_fixed[2][2][2];
    vector<int> axis_fixed[2][2][2];
    vector<int> transform_axis = axis[GRY.x][GRY.y][GRY.z];
    
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k) {
                int x = (GRY.x + i) & 1;
                int y = (GRY.y + j) & 1;
                int z = (GRY.z + k) & 1;
                
                colors_fixed[x][y][z] = colors[i][j][k];
                axis_fixed[x][y][z] = vector<int>(3);
                for (int ax = 0; ax < 3; ++ax)
                    axis_fixed[x][y][z][ax] = transform_axis[axis[i][j][k][ax]];
            }

    // Codifica o estado do cubo num inteiro que tem a permutação
    // das peças (comprimido pelo índice da permutação) e a
    // orientação delas (comprimido por base 3).
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
                axis_mask_base3 = axis_fixed[i][j][k][main_axis] * cur_base3;
                cur_base3 *= 3;
                
                sort(piece.begin(), piece.end());
                perm[4*i + 2*j + k - 1] = id_of_piece(piece);
            }

    // Comprime as informações nesse número de base 2187
    // porque 3^7 = 2187 que é o maior valor pro axis_mask_base3.
    return pos_of_perm(perm) * 2187 + axis_mask_base3;
}
