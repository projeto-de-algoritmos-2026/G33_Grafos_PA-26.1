#include <bits/stdc++.h>
using namespace std;

const int TOTAL = 11022480;
const int SOLVED = 1312200;

int get_id(const string& s) {
    const string p[8] = {"BOW", "BOY", "BRW", "BRY", "GOW", "GOY", "GRW", "GRY"};
    for (int i = 0; i < 8; ++i) 
        if (s == p[i]) return i;
    return -1;
}

int read_cube() {
    string cube[2][2][2];
    int coords[6][4][3] = {
        {{0,0,0}, {0,1,0}, {0,0,1}, {0,1,1}},
        {{0,1,0}, {1,1,0}, {0,1,1}, {1,1,1}},
        {{1,1,0}, {1,0,0}, {1,1,1}, {1,0,1}},
        {{1,0,0}, {0,0,0}, {1,0,1}, {0,0,1}},
        {{1,0,0}, {1,1,0}, {0,0,0}, {0,1,0}},
        {{0,0,1}, {0,1,1}, {1,0,1}, {1,1,1}}
    };
    string names[] = {"frente", "direita", "tras", "esquerda", "cima", "baixo"};

    for (int i = 0; i < 6; ++i) {
        cout << names[i] << ": ";
        string l1, l2; 
        cin >> l1 >> l2;
        cube[coords[i][0][0]][coords[i][0][1]][coords[i][0][2]] += l1[0];
        cube[coords[i][1][0]][coords[i][1][1]][coords[i][1][2]] += l1[1];
        cube[coords[i][2][0]][coords[i][2][1]][coords[i][2][2]] += l2[0];
        cube[coords[i][3][0]][coords[i][3][1]][coords[i][3][2]] += l2[1];
    }

    char cmap[256] = {0};
    cmap[(unsigned char)cube[0][1][1][0]] = 'G';
    cmap[(unsigned char)cube[0][1][1][1]] = 'R';
    cmap[(unsigned char)cube[0][1][1][2]] = 'Y';
    cmap[(unsigned char)cube[1][0][0][0]] = 'B';
    cmap[(unsigned char)cube[1][0][0][1]] = 'O';
    cmap[(unsigned char)cube[1][0][0][2]] = 'W';

    array<int, 7> perm{};
    int ori_base3 = 0, p3 = 1, cnt = 0;

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k) {
                if (i == 0 && j == 1 && k == 1) continue; 
                
                string part = "";
                for (char c : cube[i][j][k]) part += cmap[(unsigned char)c];
                string orig = part;
                sort(part.begin(), part.end());

                int ori = 0;
                for (int ax = 0; ax < 3; ++ax)
                    if (orig[ax] == 'B' || orig[ax] == 'G') ori = ax;

                if ((perm[cnt] = get_id(part)) == -1) exit(1);
                ori_base3 += ori * p3; 
                p3 *= 3;
                cnt++;
            }

    return pos_of_perm(perm) * 2187 + ori_base3;
}

int apply_move(int id, int move_idx) {
    auto p = perm_at_pos(id / 2187);
    int o_id = id % 2187, new_o = 0, p3 = 1;
    array<int, 7> o;
    
    for (int i = 0; i < 7; ++i) {
        o[i] = o_id % 3;
        o_id /= 3;
    }

    int face = move_idx / 3, times = (move_idx % 3) + 1; 
    int cycles[3][4] = {{3, 5, 2, 0}, {3, 0, 1, 4}, {5, 3, 4, 6}};
    int swap_a[3] = {0, 0, 1}, swap_b[3] = {1, 2, 2};

    for (int t = 0; t < times; ++t) {
        int* cyc = cycles[face];
        int tmp_p = p[cyc[3]], tmp_o = o[cyc[3]];
        
        for (int i = 3; i > 0; --i) {
            p[cyc[i]] = p[cyc[i-1]];
            o[cyc[i]] = o[cyc[i-1]];
        }
        p[cyc[0]] = tmp_p;
        o[cyc[0]] = tmp_o;

        for (int i = 0; i < 4; ++i)
            if (o[cyc[i]] == swap_a[face]) o[cyc[i]] = swap_b[face];
            else if (o[cyc[i]] == swap_b[face]) o[cyc[i]] = swap_a[face];
    }

    for (int i = 0; i < 7; ++i, p3 *= 3)
        new_o += o[i] * p3;

    return pos_of_perm(p) * 2187 + new_o;
}

int main() {
    int initial = read_cube();
    if (initial == SOLVED) {
        cout << "Resolvido.\n";
        return 0;
    }

    vector<int> pre(TOTAL, -1);
    vector<uint8_t> move(TOTAL);
    queue<int> q;

    q.push(initial);
    pre[initial] = initial;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (u == SOLVED) break;

        for (int m = 0; m < 9; ++m) {
            int v = apply_move(u, m);
            if (pre[v] == -1) {
                pre[v] = u;
                move[v] = m;
                q.push(v);
            }
        }
    }

    if (pre[SOLVED] == -1) {
        cout << "Impossivel.\n";
        return 0;
    }

    vector<int> path;
    for (int at = SOLVED; at != initial; at = pre[at])
        path.push_back(move[at]);
    
    string names[9] = {"U", "U2", "U'", "L", "L2", "L'", "B", "B2", "B'"};
    for (int i = path.size() - 1; i >= 0; --i)
        cout << names[path[i]] << " ";
    cout << '\n';
}