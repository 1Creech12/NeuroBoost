#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "core.h"
#include <string>

using namespace std;

class Statements {
private:
    struct Statement {
        string text;
        bool isTrue;
    };
    
    Statement generateStatement(int difficulty);
    
public:
    Statements();
    bool play(GameState& state);
};

#endif