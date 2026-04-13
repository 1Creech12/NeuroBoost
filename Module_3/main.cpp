#include "core.h"
#include "sequence.h"
#include "excess.h"
#include "compare.h"
#include "statements.h"
#include "memory.h"

void showMainMenu() {
    clearScreen();

    std::cout << "║  1. Числовые последовательности        ║\n";
    std::cout << "║  2. Найди лишнее                        ║\n";
    std::cout << "║  3. Сравни выражения                    ║\n";
    std::cout << "║  4. Верно или нет?                      ║\n";
    std::cout << "║  5. Память символов                     ║\n";
    std::cout << "║  6. Случайная игра                      ║\n";
    std::cout << "║  0. Выход                               ║\n";
    std::cout << "╚════════════════════════════════════════╝\n";
    std::cout << "\nВаш выбор: ";
}

void showFinalStats(const GameState& state) {
    clearScreen();
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║          ФИНАЛЬНАЯ СТАТИСТИКА          ║\n";
    std::cout << "╠════════════════════════════════════════╣\n";
    std::cout << "║ Всего очков: " << state.score;
    for(int i = 0; i < 27 - std::to_string(state.score).length(); i++) 
        std::cout << " ";
    std::cout << "║\n";
    std::cout << "║ Правильных ответов: " << state.correctAnswers;
    for(int i = 0; i < 21 - std::to_string(state.correctAnswers).length(); i++) 
        std::cout << " ";
    std::cout << "║\n";
    std::cout << "║ Всего вопросов: " << state.totalQuestions;
    for(int i = 0; i < 23 - std::to_string(state.totalQuestions).length(); i++) 
        std::cout << " ";
    std::cout << "║\n";
    
    if (state.totalQuestions > 0) {
        float accuracy = (float)state.correctAnswers / state.totalQuestions * 100;
        std::cout << "║ Точность: " << accuracy << "%";
        for(int i = 0; i < 24 - std::to_string(accuracy).length(); i++) 
            std::cout << " ";
        std::cout << "║\n";
    }
    
    std::cout << "║ Макс. серия: " << state.currentStreak;
    for(int i = 0; i < 26 - std::to_string(state.currentStreak).length(); i++) 
        std::cout << " ";
    std::cout << "║\n";
    std::cout << "╚════════════════════════════════════════╝\n";
}

int main() {
    srand(time(0));
    
    GameState gameState;
    Sequence seqGame;
    Excess oddGame;
    Compare compGame;
    Statements stmtGame;
    Memory memGame;
    
    std::cout << "\n🎮 Добро пожаловать в модуль 'Логика и Внимание'!\n";
    std::cout << "   У вас " << max_attempts << " попытки на каждый вопрос\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    bool running = true;
    while (running) {
        showMainMenu();
        printScore(gameState);
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "0") {
            running = false;
            showFinalStats(gameState);
            std::cout << "\n👋 Спасибо за игру! До встречи!\n\n";
            break;
        }
        
        int gameChoice;
        try {
            gameChoice = std::stoi(choice);
        } catch(...) {
            std::cout << "❌ Неверный ввод. Попробуйте снова.\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        
        // Если выбрана случайная игра
        if (gameChoice == 6) {
            gameChoice = rand() % 5 + 1;
        }
        
        switch(gameChoice) {
            case 1:
                seqGame.play(gameState);
                break;
            case 2:
                oddGame.play(gameState);
                break;
            case 3:
                compGame.play(gameState);
                break;
            case 4:
                stmtGame.play(gameState);
                break;
            case 5:
                memGame.play(gameState);
                break;
            default:
                std::cout << "❌ Неверный выбор. Попробуйте снова.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
    return 0;
}