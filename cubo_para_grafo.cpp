#include <bits/stdc++.h>
#include "common.hpp"
using namespace std;

array<int, 7654322> idx_map;

void populate_idx_map() {
    array<int, 7> xs = {1, 2, 3, 4, 5, 6, 7};
    int cnt = 0;   
    do {
        int num = permutation_to_num(xs);
        idx_map[num] = cnt;
        ++cnt;
    } while (next_permutation(xs.begin(), xs.end()));
}

int main() {
    cout << "Para descrever uma face do seu cubo escreva assim:\n";
    cout << "YW\n";
    cout << "WW\n";
    cout << "Esse é um exemplo que a face tem um quadrado amarelo e três brancos\n";
    cout << "Sempre na ordem\n";
    cout << "12\n";
    cout << "34\n\n";
    
    string cube[2][2][2];
    string l1, l2;
    
    cout << "Descreva a face da frente do seu cubo >\n";
    cin >> l1 >> l2;
    cube[0][0][0] += l1[0];
    cube[0][1][0] += l1[1];
    cube[0][0][1] += l2[0];
    cube[0][1][1] += l2[1];
    
    cout << "Gire o cubo e descreva a face da direita >\n";
    cin >> l1 >> l2;
    cube[0][1][0] += l1[0];
    cube[1][1][0] += l1[1];
    cube[0][1][1] += l2[0];
    cube[1][1][1] += l2[1];
    
    cout << "Gire o cubo e descreva a face de trás >\n";
    cin >> l1 >> l2;
    cube[1][1][0] += l1[0];
    cube[1][0][0] += l1[1];
    cube[1][1][1] += l2[0];
    cube[1][0][1] += l2[1];
    
    cout << "Gire o cubo e descreva a face da esquerda >\n";
    cin >> l1 >> l2;
    cube[1][0][0] += l1[0];
    cube[0][0][0] += l1[1];
    cube[1][0][1] += l2[0];
    cube[0][0][1] += l2[1];
    
    cout << "Gire o cubo para voltar para a face da frente e descreva a face de cima >\n";
    cin >> l1 >> l2;
    cube[1][0][0] += l1[0];
    cube[1][1][0] += l1[1];
    cube[0][0][0] += l2[0];
    cube[0][1][0] += l2[1];
    
    cout << "Agora descreva a face de baixo do seu cubo >\n";
    cin >> l1 >> l2;
    cube[0][0][1] += l1[0];
    cube[0][1][1] += l1[1];
    cube[1][0][1] += l2[0];
    cube[1][1][1] += l2[1];
    
    map<string, int> mp = {
        {"BOW", 1},
        {"BOY", 2},
        {"BRW", 3},
        {"BRY", 4},
        {"GOW", 5},
        {"GOY", 6},
        {"GRW", 7},
        {"GRY", 8},
    };
    
    int axis[2][2][2];
    array<int, 7> perm;
    
    int cnt = 0, ori=0, p3=1; 
    for (int i = 0; i < 2; ++i){
        for (int j = 0; j < 2; ++j){
            for (int k = 0; k < 2; ++k) {
                for (int ax = 0; ax < 3; ++ax){
                    if (cube[i][j][k][ax] == 'B' || cube[i][j][k][ax] == 'G')
                        axis[i][j][k] = ax;
                }
                sort(cube[i][j][k].begin(), cube[i][j][k].end());
                if(cube[i][j][k]!="BOW"){
                    perm[cnt-1] = mp[cube[i][j][k]];
                    cnt++;
                    p3*=3; 
                    ori+=axis[i][j][k]*p3;
                }
            }
        }
    }
   
    cout << idx_map[permutation_to_num(perm)]*2187 + ori << '\n'; 
}

