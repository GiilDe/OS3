//
// Created by Gilad on 15-Dec-18.
//

#include "Job.hpp"

Job::Job(uint lower, uint upper, bool_mat *current, bool_mat *next) :
    lower_row(lower),
    upper_row(upper),
    current(current),
    next(next) {}

int Job::countNeighbors(int i, int j) {
    uint width = (uint)current[0].size();
    uint height = (uint)current->size();
    uint neighbors = 0;
    for (int k = -1; k <= 1; ++k) {
        for (int z = -1; z <= 1; ++z) {
            bool in_bounds = i + k >= 0 && i + k < height && j + z >= 0 && j + z < width;
            bool self = (k == 0) && (z == 0);
            if (in_bounds && !self && (*current)[i + k][j + z]) {
                neighbors++;
            }
        }
    }
    return neighbors;
}

void Job::run() {

    uint w = current[0].size();
    for (int i = lower_row; i <= upper_row; ++i) {
        for (int j = 0; j < w; ++j) {
            int live_neighbors = countNeighbors(i, j);
            bool birth = !(*current)[i][j] && live_neighbors == 3;
            bool survival = (*current)[i][j] && (live_neighbors == 2 || live_neighbors == 3);
            (*next)[i][j] = birth || survival;
        }
    }
}