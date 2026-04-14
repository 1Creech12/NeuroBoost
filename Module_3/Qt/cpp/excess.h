#ifndef EXCESS_H
#define EXCESS_H

#include "core.h"
#include <vector>
#include <string>

using namespace std;

class Excess {
private:
    struct Item {
        string name;
        string category;
    };
    
    vector<Item> generateItems(int difficulty);
    int findOddIndex(const vector<Item>& items);
    
public:
    Excess();
    bool play(GameState& state);
};

#endif