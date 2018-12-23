//
// Created by Gilad on 15-Dec-18.
//

#include "Job.hpp"

void Job::run() {
    uint width = current->get_width();
    uint height = current->get_height();
    uint temp_size = upper_row-lower_row+1;
    vector<vector<bool>>& nex_matrix = next->field;
    vector<vector<bool>>& current_matrix = current->field;

    for (int i = lower_row; i <= upper_row; ++i) {
        if(i == lower_row && lower_lock != NULL){
            lower_lock->lock();
        }
        if(i == upper_row && upper_lock != NULL){
            upper_lock->lock();
        }
        for (int j = 0; j < width; ++j) {
            int live_neighbours = 0;
            for (int k = -1; k <= 1; ++k) {
                for (int z = -1; z <= 1; ++z) {
                    if(i+k >= 0 && i+k < height && j+z >= 0 && j+z < width
                            && current_matrix[i+k][j+z] == false){
                        live_neighbours++;
                    }
                }
            }
            if(!current_matrix[i][j] && live_neighbours == 3){
                nex_matrix[i][j] = true;
            }
            else if(current_matrix[i][j] && (live_neighbours == 2 ||
                    live_neighbours == 3)){
                nex_matrix[i][j] = true;
            }
            else{
                nex_matrix[i][j] = false;
            }
        }
        if(i == lower_row && lower_lock != NULL){
            lower_lock->unlock();
        }
        if(i == upper_row && upper_lock != NULL) {
            upper_lock->unlock();
        }
    }
}