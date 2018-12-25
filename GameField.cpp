//
// Created by Gilad on 15-Dec-18.
//

#include "GameField.hpp"
#include "utils.hpp"

GameField::GameField(vector<vector<bool>>& matrix, uint width, uint height) :
        field(matrix), width(width), height(height) {

}

uint GameField::get_height() {
    return height;
}

uint GameField::get_width(){
    return width;
}

vector<bool>& GameField::operator[](int i) {
    return field[i];
}