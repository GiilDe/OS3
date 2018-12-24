//
// Created by Gilad on 15-Dec-18.
//

#include "Job.hpp"

int Job::countNeighbors(int i, int j) {
    uint width = current->get_width();
    uint height = current->get_height();
    vector<vector<bool>> &current_matrix = current->field;
    uint neighbors = 0;
    for (int k = -1; k <= 1; ++k) {
        for (int z = -1; z <= 1; ++z) {
            bool in_bounds = i + k >= 0 && i + k < height && j + z >= 0 && j + z < width;
            bool self = (k == 0) && (z == 0);
            if (in_bounds && !self && current_matrix[i + k][j + z]) {
                neighbors++;
            }
        }
    }
    return neighbors;
}

void Job::run() {
    vector<vector<bool>> &next_matrix = next->field;
    vector<vector<bool>> &current_matrix = current->field;

    uint w = current->get_width();
    for (int i = lower_row; i <= upper_row; ++i) {
        for (int j = 0; j < w; ++j) {
            if (i == lower_row && lower_locks[j] != nullptr) {
                lower_locks[j]->lock();
            }
            if (i == upper_row && upper_locks[j] != nullptr) {
                upper_locks[j]->lock();
            }
            int live_neighbours = countNeighbors(i, j);
            if (!current_matrix[i][j] && live_neighbours == 3) {
                next_matrix[i][j] = true;
            } else next_matrix[i][j] = current_matrix[i][j] && (live_neighbours == 2 || live_neighbours == 3);
            if (i == lower_row && lower_locks[j] != nullptr) {
                lower_locks[j]->unlock();
            }
            if (i == upper_row && upper_locks[j] != nullptr) {
                upper_locks[j]->unlock();
            }
        }
    }
}