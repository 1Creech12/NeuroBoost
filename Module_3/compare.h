#ifndef COMPARE_H
#define COMPARE_H

#include "core.h"
#include <string>

using namespace std;

class Compare {
private:
    struct Expression {
        int left;
        int right;
        char operation;
        int result;
    };
    
    Expression generateExpression(int difficulty);
    
public:
    Compare();
    bool play(GameState& state);
};

#endif