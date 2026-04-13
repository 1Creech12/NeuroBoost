#include "core.h"


void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void printHeader(const std::string& title) {
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║ " << title;
    for(int i = title.length(); i < 38; i++) std::cout << " ";
    std::cout << "║\n";
    std::cout << "╚════════════════════════════════════════╝\n";
}

void printScore(const GameState& state) {
    std::cout << "\n┌─────────────── ТЕКУЩИЙ СЧЕТ ───────────────┐\n";
    std::cout << "│ Очки: " << state.score;
    for(int i = 0; i < 35 - std::to_string(state.score).length(); i++) 
        std::cout << " ";
    std::cout << "│\n";
    std::cout << "│ Сложность: " << (state.difficulty == 1 ? "Легкая" : 
                                   state.difficulty == 2 ? "Средняя" : "Сложная");
    for(int i = 0; i < 30; i++) std::cout << " ";
    std::cout << "│\n";
    std::cout << "│ Серия правильных: " << state.currentStreak;
    for(int i = 0; i < 25 - std::to_string(state.currentStreak).length(); i++) 
        std::cout << " ";
    std::cout << "│\n";
    std::cout << "└────────────────────────────────────────────┘\n";
}

bool checkAnswer(const std::string& userAnswer, const std::string& correctAnswer) {
    return userAnswer == correctAnswer;
}

void updateDifficulty(GameState& state) {
    // Динамическое изменение сложности на основе правильных ответов
    if (state.correctAnswers >= 10 && state.difficulty == 1) {
        state.difficulty = 2;
        std::cout << "\n🌟 ПОЗДРАВЛЯЕМ! Сложность повышена до СРЕДНЕЙ!\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
    } else if (state.correctAnswers >= 20 && state.difficulty == 2) {
        state.difficulty = 3;
        std::cout << "\n🔥 ВПЕЧАТЛЯЕТ! Сложность повышена до СЛОЖНОЙ!\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int calculatePoints(int basePoints, int difficulty, int timeLeft, bool isCorrect) {
    if (!isCorrect) return 0;
    
    int multiplier = difficulty;
    int timeBonus = timeLeft * 2;
    return basePoints * multiplier + timeBonus;
}

bool waitForAnswerWithTimer(int timeLimit, std::string& userAnswer) {
    auto start = std::chrono::steady_clock::now();
    std::cout << "⏱️  У вас есть " << timeLimit << " секунд!\n";
    std::cout << "Ваш ответ: ";
    
    std::getline(std::cin, userAnswer);
    
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    
    if (elapsed > timeLimit) {
        std::cout << "\n⏰ ВРЕМЯ ВЫШЛО!\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return false;
    }
    
    return true;
}