//
// Created by Gilad on 15-Dec-18.
//



#ifndef OS3_GAME_FIELD_HPP
#define OS3_GAME_FIELD_HPP

#include "../Part1/Headers.hpp"


class Game_field {
public:
    vector<vector<bool>> field;

    explicit Game_field(vector<string> matrix);
    uint get_height(){
        return field.size();
    }
    uint get_width(){
        return field[0].size();
    }
};


#endif //OS3_GAME_FIELD_HPP
