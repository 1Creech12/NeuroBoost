#ifndef MEMORY_H
#define MEMORY_H

#include "core.h"
#include <vector>
#include <string>

using namespace std;

class Memory {
private:
    vector<char> history;
    char generateRandomSymbol();
    
public:
    Memory();
    bool play(GameState& state);
};

#endif