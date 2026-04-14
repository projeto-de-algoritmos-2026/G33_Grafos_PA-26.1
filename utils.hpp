#pragma once

#include <span>
#include <array>
using namespace std;

int compress_state(span<const int> perm, int axis_mask_base3);
pair<array<int, 7>, int> uncompress_state(int state);