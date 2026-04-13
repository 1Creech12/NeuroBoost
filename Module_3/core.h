#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

const int max_attempts = 3;           // Количество попыток
const int target_age = 10;
const int time_limit_easy = 10;       // Время на ответ сек
const int time_limit_medium = 7;      // Средний уровень
const int time_limit_hard = 5;        // Сложный уровень
const int base_point = 10;           // Базовые очки за правильный ответ

// Хранения состояния игры
struct GameState {
    int score = 0;
    int difficulty = 1;
    int correctAnswers = 0;
    int totalQuestions = 0;
    int currentStreak = 0;            // Серия правильных ответов
};

struct Question {
    std::string text;
    std::string correctAnswer;
    int points;
    int timeLimit;
};

// Общие функции
void clearScreen();
void printHeader(const std::string& title);
void printScore(const GameState& state);
bool checkAnswer(const std::string& userAnswer, const std::string& correctAnswer);
void updateDifficulty(GameState& state);
int calculatePoints(int basePoints, int difficulty, int timeLeft, bool isCorrect);
bool waitForAnswerWithTimer(int timeLimit, std::string& userAnswer);

#endif