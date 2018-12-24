//
// Created by Gilad on 15-Dec-18.
//

#include "Job.hpp"

Job::Job(uint lower, uint upper, GameField *current, GameField *next, Lock *lower_s, Lock *upper_s)  {
    lower_row = lower;
    upper_row = upper;
    this->current = current;
    this->next = next;
    this->lower_lock = lower_s;
    this->upper_lock = upper_s;
}

int Job::countNeighbors(int i, int j) {
    int live_neighbors = 0;
    uint width = current->get_width();
    uint height = current->get_height();
    vector<vector<bool>> &current_matrix = current->field;
    for (int k = -1; k <= 1; ++k) {
        for (int z = -1; z <= 1; ++z) {
            bool in_bounds = i + k >= 0 && i + k < height && j + z >= 0 && j + z < width;
            bool self = (k == 0) && (z == 0);
            if (!self && in_bounds && current_matrix[i + k][j + z]) {
                live_neighbors++;
            }
        }
    }
    return live_neighbors;
}

void Job::run() {
    vector<vector<bool>> &next_matrix = next->field;
    vector<vector<bool>> &current_matrix = current->field;

    for (int i = lower_row; i <= upper_row; ++i) {
        if (i == lower_row && lower_lock != nullptr) lower_lock->lock();
        if (i == upper_row && upper_lock != nullptr) upper_lock->lock();

        int w = current->get_width();
        for (int j = 0; j < w; ++j) {
            int live_neighbors = countNeighbors(i, j);
            next_matrix[i][j] = false;
            if (!current_matrix[i][j] && live_neighbors == 3) {
                // Birth
                next_matrix[i][j] = true;
            } else {
                // Survival/Loneliness
                next_matrix[i][j] = current_matrix[i][j] && (live_neighbors == 2 || live_neighbors == 3);
            }
        }
        if (i == lower_row && lower_lock != nullptr) lower_lock->unlock();
        if (i == upper_row && upper_lock != nullptr) upper_lock->unlock();
    }
}