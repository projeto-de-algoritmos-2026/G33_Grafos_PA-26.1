#pragma once

#include <vector>
using namespace std;

int compress_state(const vector<int>& perm, int axis_mask_base3);
pair<vector<int>, int> uncompress_state(int state);