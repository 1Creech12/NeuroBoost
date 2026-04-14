#include "memory.h"

Memory::Memory() {
    srand(time(0));
}

char Memory::generateRandomSymbol() {
    const std::string symbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%&";
    return symbols[rand() % symbols.length()];
}

bool Memory::play(GameState& state) {
    clearScreen();
    printHeader("Запомни символы");
    
    std::cout << "\nПравила: Запомните символы! Совпадает ли\n";
    std::cout << "текущий символ с тем, что был 2 хода назад.\n\n";
    std::cout << "гого\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    history.clear();
    int rounds = 5 + state.difficulty * 2; // 7-11 раундов
    int correctCount = 0;
    
    for (int round = 1; round <= rounds; round++) {
        clearScreen();
        printHeader("Память - раунд " + std::to_string(round) + "/" + std::to_string(rounds));
        printScore(state);
        
        char newSymbol = generateRandomSymbol();
        history.push_back(newSymbol);
        std::cout << "Текущиц символ: " << newSymbol << "\n";
        
        if (history.size() >= 3) {
            char twoStepsBack = history[history.size() - 3];
            bool isSame = (newSymbol == twoStepsBack);
            
            int timeLimit = (state.difficulty == 1) ? time_limit_easy - 2 : (state.difficulty == 2) ? time_limit_medium - 2 : time_limit_hard - 2;
            if (timeLimit < 3) timeLimit = 3;
            
            std::cout << "\nСовпадает символ с тем, что был 2 хода назад?\n";
            std::cout << "('да' или 'нет'): ";
            
            int attempts = 0;
            bool roundAnswered = false;
            
            while (attempts < max_attempts && !roundAnswered) {
                if (attempts > 0) {
                    std::cout << "Попытка " << attempts + 1 << " из " << max_attempts << ": ";
                }
                
                std::string userAnswer;
                auto start = std::chrono::steady_clock::now();
                bool inTime = waitForAnswerWithTimer(timeLimit, userAnswer);
                
                if (!inTime) {
                    attempts++;
                    if (attempts >= max_attempts) {
                        std::cout << "\nВремя вышло, правильный ответ: " << (isSame ? "да" : "нет") << "\n";
                        state.currentStreak = 0;
                    }
                    continue;
                }
                
                auto end = std::chrono::steady_clock::now();
                int timeLeft = timeLimit - std::chrono::duration_cast<std::chrono::seconds> (end - start).count();
                
                for (char& c : userAnswer) c = tolower(c);
                bool userSaysYes = (userAnswer == "да" || userAnswer == "yes");
                
                if (userSaysYes == isSame) {
                    int earned = calculatePoints(base_point / 2, state.difficulty, timeLeft, true);
                    state.score += earned;
                    correctCount++;
                    state.currentStreak++;
                    std::cout << "\nЙоу +" << earned << " очков!\n";
                    roundAnswered = true;
                } else {
                    attempts++;
                    if (attempts >= max_attempts) {
                        std::cout << "\nПравильный ответ: " << (isSame ? "да" : "нет") << "\n";
                        std::cout << "2 хода назад был символ: " << twoStepsBack << "\n";
                        state.currentStreak = 0;
                    }
                }
            }
            
            state.totalQuestions++;
            if (roundAnswered) state.correctAnswers++;
            
            // История для дебилов
            std::cout << "\nИстория символов: ";
            for (size_t i = 0; i < history.size(); i++) {
                if (i == history.size() - 3) std::cout << "[" << history[i] << "] ";
                else if (i == history.size() - 1) std::cout << "→ " << history[i] << " ";
                else std::cout << history[i] << " ";
            }
            std::cout << "\n";
            
            std::this_thread::sleep_for(std::chrono::seconds(2));
        } else {
            std::cout << "\nЗапомните этот символ...\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
    
    clearScreen();
    printHeader("Результаты");
    std::cout << "\nВы ответили правильно на " << correctCount 
             << " из " << (rounds - 2) << " вопросов!\n";
    
    updateDifficulty(state);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return true;
}