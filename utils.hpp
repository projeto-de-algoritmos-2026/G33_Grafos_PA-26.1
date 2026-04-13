#pragma once

#include <vector>

long long pos_of_perm(const std::vector<int>& perm);
std::vector<int> perm_at_pos(int n, long long pos);
std::vector<int> to_base(int x, int b);