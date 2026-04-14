#include "sequence.h"
#include <sstream>

Sequence::Sequence() {
    srand(time(0));// генератор случ. чисел
}

// Арифметическая последовательность
vector<int> Sequence::generateArithmeticSequence(int start, int step, int count) {
    vector<int> seq;
    for (int i = 0; i < count; i++) {
        seq.push_back(start + i * step);
    }
    return seq;
}

// Геометрическая последовательность
vector<int> Sequence::generateGeometricSequence(int start, int ratio, int count) {
    vector<int> seq;
    int current = start;
    for (int i = 0; i < count; i++) {
        seq.push_back(current);
        current *= ratio;
    }
    return seq;
}

// Фибоначичи
vector<int> Sequence::generateFibonacciSequence(int count) {
    vector<int> seq;
    if (count >= 1) seq.push_back(1);
    if (count >= 2) seq.push_back(1);
    
    for (int i = 2; i < count; i++) {
        seq.push_back(seq[i-1] + seq[i-2]);
    }
    return seq;
}

// Случ посследов
vector<int> Sequence::generateCustomSequence(int difficulty) {
    int count = 4 + difficulty; // Показываем 4-6 чисел
    
    switch(rand() % 3) {
        case 0: // Арифметическая
            return generateArithmeticSequence(
                rand() % 10 + 1, 
                rand() % 5 + 1 + difficulty, 
                count
            );
        case 1: // Геометрическая
            return generateGeometricSequence(
                rand() % 5 + 1,
                2 + (difficulty > 2 ? 1 : 0),
                count
            );
        case 2: // Фибоначчи
            return generateFibonacciSequence(count);
    }
    return vector<int>();
}

// Преобразова в строку
string Sequence::vectorToString(const std::vector<int>& vec) {
    std::stringstream ss;
    for (size_t i = 0; i < vec.size() - 1; i++) {
        ss << vec[i] << ", ";
    }
    ss << "?";
    return ss.str();
}

// запускает 
bool Sequence::play(GameState& state) {
    clearScreen();
    printHeader("Продолжи последовательность");
    
    vector<int> sequence = generateCustomSequence(state.difficulty);
    int correctNext = sequence.back();
    sequence.pop_back(); // Убираем последнее число
    
    int timeLimit = (state.difficulty == 1) ? time_limit_easy : (state.difficulty == 2) ? time_limit_medium : time_limit_hard;
    
    Question q;
    q.text = "Последовательность: " + vectorToString(sequence) + "\nКакое число должно быть следующим?";
    q.correctAnswer = to_string(correctNext);
    q.points = base_point * state.difficulty;
    q.timeLimit = timeLimit;
    
    cout << "\n" << q.text << "\n";
    
    int attempts = 0;
    bool answered = false;
    
    while (attempts < max_attempts && !answered) {
        if (attempts > 0) {
            cout << "\n  Неправильно, попытка " << attempts + 1 << " из " << max_attempts << ")\n";
        }
        
        string userAnswer;
        auto start = chrono::steady_clock::now();
        bool inTime = waitForAnswerWithTimer(q.timeLimit, userAnswer);
        
        if (!inTime) {
            attempts++;
            if (attempts >= max_attempts) {
                cout << "\nПравильный ответ: " << q.correctAnswer << "\n";
                state.currentStreak = 0;
            }
            continue;
        }
        
        auto end = chrono::steady_clock::now();
        int timeLeft = q.timeLimit - chrono::duration_cast<chrono::seconds>
                      (end - start).count();
        
        if (checkAnswer(userAnswer, q.correctAnswer)) {
            int earned = calculatePoints(q.points, state.difficulty, timeLeft, true);
            state.score += earned;
            state.correctAnswers++;
            state.totalQuestions++;
            state.currentStreak++;
            cout << "\nПравильно +" << earned << " очков\n";
            if (timeLeft > 2) {
                cout << "Бонус за скорость: +" << timeLeft * 2 << " очков\n";
            }
            answered = true;
        } else {
            attempts++;
            state.totalQuestions++;
            if (attempts >= max_attempts) {
                cout << "\nНеправильно, правильный ответ: " << q.correctAnswer << "\n";
                state.currentStreak = 0;
            }
        }
    }
    
    updateDifficulty(state);
    this_thread::sleep_for(chrono::seconds(2));
    return answered;
}