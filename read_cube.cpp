#include "read_cube.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <array>
using namespace std;

struct Coords { int x, y, z; };

int id_of_piece(const string& s) {
    // Não está em ordem alfabética porque eu puis de acordo com as coordenadas
    // e os loops para a permutação resolvida ser 0123456.
    array<string, 8> arr = {"GRW", "GOY", "BRY", "GOW", "BRY", "BRW", "BOY", "BOW"};
    int id = find(arr.begin(), arr.end(), s) - arr.begin();
    return id == arr.size() ? -1 : id;
}

int main() {
    cout << "\nInstruções:\n\nSelecione uma face do cubo para ser a frontal\n\n"
         << "Cores: \033[33mamarelo (Y), \033[31mvermelho (R), \033[32mverde (G), \033[38;5;208mlaranja (O), \033[34mazul (B) e \033[mbranco (W)\n\n"
         << "Para inserir as cores de uma face escreva assim: YW GG\n\n"
         << "A ordem é canto superior direito, esquerdo, desce e repete.\n\n";
         
    // Lê as peças numa matriz 3d 2x2x2.
    string cube[2][2][2];
    Coords coords[6][4] = {
        {{0,0,0}, {0,1,0}, {0,0,1}, {0,1,1}},
        {{0,1,0}, {1,1,0}, {0,1,1}, {1,1,1}},
        {{1,1,0}, {1,0,0}, {1,1,1}, {1,0,1}},
        {{1,0,0}, {0,0,0}, {1,0,1}, {0,0,1}},
        {{1,0,0}, {1,1,0}, {0,0,0}, {0,1,0}},
        {{0,0,1}, {0,1,1}, {1,0,1}, {1,1,1}}
    };
    string faces[] = {"frontal", "direita", "traseira", "esquerda", "de cima", "de baixo"};
    
    for (int i = 0; i < 6; ++i) {
        if (i < 4) cout << "Gire o cubo e insira a face ";
        else cout << "Na face frontal insira a face ";
        cout << faces[i] << " >\n";
        string l1, l2; 
        cin >> l1 >> l2;
        cube[coords[i][0].x][coords[i][0].y][coords[i][0].z] += l1[0];
        cube[coords[i][1].x][coords[i][1].y][coords[i][1].z] += l1[1];
        cube[coords[i][2].x][coords[i][2].y][coords[i][2].z] += l2[0];
        cube[coords[i][3].x][coords[i][3].y][coords[i][3].z] += l2[1];
    }
    
    Coords GRY = {0, 0, 0};
    
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k) {
                string piece = cube[i][j][k];
                sort(piece.begin(), piece.end());
                if (piece == "GRY") GRY = {i, j, k};
            }
    
    // Fixa a peça GRY como (0, 0, 0).   
    string cube_fixed[2][2][2];
    
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k) {
                int x = (GRY.x + i) & 1;
                int y = (GRY.y + j) & 1;
                int z = (GRY.z + k) & 1;
                
                cube_fixed[x][y][z] = cube[i][j][k];
            }
    
    vector<int> perm(7);
    
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k) {
                if (i == 0 && j == 0 && k == 0) continue;
                perm[i + 2*j + 4*k] = get_id(cube_fixed[i][j][k]);
            }
    
}
