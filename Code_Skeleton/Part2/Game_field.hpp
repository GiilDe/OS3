//
// Created by Gilad on 15-Dec-18.
//



#ifndef OS3_GAME_FIELD_HPP
#define OS3_GAME_FIELD_HPP

#include "../Part1/Headers.hpp"
#include <vector>


class Game_field {
public:
    Game_field(vector<string> matrix);
    vector<vector<bool>> field;
};


#endif //OS3_GAME_FIELD_HPP
