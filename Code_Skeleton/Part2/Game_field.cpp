//
// Created by Gilad on 15-Dec-18.
//

#include "Game_field.hpp"

Game_field::Game_field(vector<string> matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        uint counter = 0;
        for (int j = 0; j < matrix[0].size(); ++j) {
            field[i][j] = matrix[i].at(counter);
            counter += 2;
        }
    }
}