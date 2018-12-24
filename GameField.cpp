//
// Created by Gilad on 15-Dec-18.
//

#include "GameField.hpp"
#include "utils.hpp"

GameField::GameField(vector<string> matrix) :
        field(matrix.size(), vector<bool>((matrix[0].size()+1)/2)){

    for (int i = 0; i < matrix.size(); ++i) {
        vector<string> line = utils::split(matrix[i], ' ');
        int j = 0;
        for(string str : line) {
            int bit = stoi(str, nullptr, 10);
            bool flag = (bool)bit;
            field[i][j] = flag;
            j++;
        }
    }

}

uint GameField::get_height() {
    return (uint)field.size();
}

uint GameField::get_width(){
    return (uint)field[0].size();
}

vector<bool>& GameField::operator[](int i) {
    return field[i];
}