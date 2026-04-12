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

/*
int apply_transformation(const array<int, 7>& order, int num) {
    array<int, 7> perm = num_to_permutation(num);
    array<int, 7> new_perm;
    for (int i = 0; i < 7; ++i)
        new_perm[i] = perm[order[i]];
    return permutation_to_num(new_perm);
}*/

int apply_transformation(int state, const array<int, 7>& pos, const array<int, 7>& ori) {
    int curmask = state%2187;
    int curperm = state/2187;
    
    int idx = perm[curperm]; 
    int tmp = curmask; 
    array<int, 7> p = num_to_permutation(idx);
    array<int, 7> o;
    for (int i = 0; i < 7; ++i){
        o[i] = tmp%3;
        tmp/=3; 
    }  

    array<int, 7> next_p, next_o;
    for (int i = 0; i < 7; ++i){
        next_p[i] = p[pos[i]];
        next_o[i] = (o[pos[i]] + ori[i])%3;
    }

    int next_p_num = permutation_to_num(next_p);
    int next_perm_idx = pos[next_p_num]; 

    int next_mask = 0; 
    int p3 = 1; 
    for(int i = 0; i < 7; ++i){
        next_mask += next_o[i]*p3;
        p3*=3;
    }

    return (next_perm_idx*2187)+ next_mask;

}

const array<int, 7> ROTATE_X = {4, 1, 2, 3, 5, 6, 7};
const array<int, 7> ROTATE_Y = {1, 5, 2, 4, 6, 3, 7};
const array<int, 7> ROTATE_Z = {1, 2, 6, 3, 5, 7, 4};

const array<int, 7> ORI_X = {2, 0, 0, 0, 1, 2, 1};
const array<int, 7> ORI_Y = {0, 1, 0, 2, 0, 1, 2};
const array<int, 7> ORI_Z = {0, 0, 1, 2, 0, 1, 2};  


int main() {
    populate_idx_perm();
    
    // O estado é assim:
    // perm .. .. .. .. .. .. .. onde .. é uma bitmask pro eixo da i-ésima peça.
    vector<int> pre(TOTAL_STATES, -1);
    vector<int> move(TOTAL_STATES, 0);
    
    queue<int> q;
    q.emplace(0);
    pre[0] = 0; 
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int i = 0; i < 3; ++i) {
            int v = apply_transformation(u, (i == 0 ? ROTATE_X : (i == 1 ? ROTATE_Y : ROTATE_Z)), (i == 0 ? ORI_X : (i == 1 ? ORI_Y : ORI_Z)));
            if (pre[v] == -1) {
                pre[v] = u;
                move[v] = i;
                q.emplace(v);
            }
        }
    }
    
    int at; 
    cout << "Digite o numero gerado>\n";
    cin >> at;
    if(pre[at] == -1){
        cout << "Estado inalcançável\n";
        return 0;
    }else{
        cout << "Movimentos para resolver o cubo:\n";
        while(at!=0){
            if(move[at] == 0) cout << "Rotacionar em X\n";
            else if(move[at] == 1) cout << "Rotacionar em Y\n";
            else cout << "Rotacionar em Z\n";
            at = pre[at];
        }
        cout << endl;

    }
    return 0;
}
