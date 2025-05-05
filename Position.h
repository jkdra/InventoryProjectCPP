//
// Created by Jawad Khadra on 5/5/25.
//

#ifndef POSITION_H
#define POSITION_H

#include "project.h"

using namespace std;

class Position {
private:
    int row;
    int col;
    
public:
    Position(int row, int col) : row(row), col(col) {}
    
    // Getters
    int getRow() const { return row; }
    int getCol() const { return col; }
    
    // Equality operator for comparing positions
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
    
    // Check if position is valid for a library with 3 shelves and 15 compartments per shelf
    bool isValid() const {
        return row >= 0 && row < 3 && col >= 0 && col < 15;
    }
};

#endif //POSITION_H
