#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "core.h"
#include <vector>
#include <string>

using namespace std;

class Sequence {
private:
    // Последовательности йоу
    vector<int> generateArithmeticSequence(int start, int step, int count);
    vector<int> generateGeometricSequence(int start, int ratio, int count);
    vector<int> generateFibonacciSequence(int count);
    vector<int> generateCustomSequence(int difficulty);
    
public:
    Sequence();
    bool play(GameState& state);
    string vectorToString(const vector<int>& vec);
};

#endif