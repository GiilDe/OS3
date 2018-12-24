//
// Created by Gilad on 15-Dec-18.
//
#ifndef OS3_GAME_FIELD_HPP
#define OS3_GAME_FIELD_HPP

#include "Headers.hpp"

class GameField {
public:
    vector<vector<bool>> field;

    explicit GameField(vector<string> matrix);
    uint get_height();
    uint get_width();
    vector<bool>& operator[](int i);
};

#endif //OS3_GAME_FIELD_HPP
