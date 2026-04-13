#include "compare.h"

Compare::Compare() {
    srand(time(0));
}

Compare::Expression Compare::generateExpression(int difficulty) {
    Expression expr;
    
    int maxNum = 10 * difficulty;
    expr.left = rand() % maxNum + 1;
    expr.right = rand() % maxNum + 1;
    
    // Выбор от сложности
    if (difficulty == 1) {
        expr.operation = (rand() % 2) ? '+' : '-';
    } else if (difficulty == 2) {
        int op = rand() % 3;
        expr.operation = (op == 0) ? '+' : (op == 1) ? '-' : '*';
    } else {
        int op = rand() % 4;
        expr.operation = (op == 0) ? '+' : (op == 1) ? '-' : (op == 2) ? '*' : '/';
        if (expr.operation == '/') {
            expr.right = rand() % 10 + 1;
            expr.left = expr.right * (rand() % 10 + 1);
        }
    }
    
    // резул
    switch(expr.operation) {
        case '+': expr.result = expr.left + expr.right; break;
        case '-': expr.result = expr.left - expr.right; break;
        case '*': expr.result = expr.left * expr.right; break;
        case '/': expr.result = expr.left / expr.right; break;
    }
    
    return expr;
}

bool Compare::play(GameState& state) {
    clearScreen();
    printHeader("Сравни выражения");
    
    Expression expr1 = generateExpression(state.difficulty);
    Expression expr2 = generateExpression(state.difficulty);
    
    int timeLimit = (state.difficulty == 1) ? time_limit_easy : (state.difficulty == 2) ? time_limit_medium : time_limit_hard;
    
    cout << "\nКакое выражение больше?\n\n";
    cout << "   1) " << expr1.left << " " << expr1.operation << " " << expr1.right << "\n";
    cout << "   2) " << expr2.left << " " << expr2.operation << " " << expr2.right << "\n";
    cout << "\nВведите '1', '2' или '=' если равны: ";
    
    string correctAnswer;
    if (expr1.result > expr2.result) correctAnswer = 1;
    else if (expr2.result > expr1.result) correctAnswer = 2;
    else correctAnswer = "=";
    
    int attempts = 0;
    bool answered = false;
    
    while (attempts < max_attempts && !answered) {
        if (attempts > 0) {
            cout << "\nЕще раз, попытка " << attempts + 1 << " из " << max_attempts << "): ";
        }
        
        string userAnswer;
        auto start = chrono::steady_clock::now();
        bool inTime = waitForAnswerWithTimer(timeLimit, userAnswer);
        
        if (!inTime) {
            attempts++;
            if (attempts >= max_attempts) {
                cout << "\nПравильный ответ: " << correctAnswer << " (" << expr1.result << " и " << expr2.result << ")\n";
                state.currentStreak = 0;
            }
            continue;
        }
        
        auto end = chrono::steady_clock::now();
        int timeLeft = timeLimit - chrono::duration_cast<chrono::seconds> (end - start).count();
        
        // Верхний рег
        if (!userAnswer.empty()) userAnswer[0] = toupper(userAnswer[0]);
        
        if (userAnswer == correctAnswer) {
            int earned = calculatePoints(base_point, state.difficulty, timeLeft, true);
            state.score += earned;
            state.correctAnswers++;
            state.totalQuestions++;
            state.currentStreak++;
            cout << "\nЙоу +" << earned << " очков!\n";
            cout << "   " << expr1.left << " " << expr1.operation << " " << expr1.right << " = " << expr1.result << "\n";
            cout << "   " << expr2.left << " " << expr2.operation << " " << expr2.right << " = " << expr2.result << "\n";
            answered = true;
        } else {
            attempts++;
            state.totalQuestions++;
            if (attempts >= max_attempts) {
                cout << "\nПравильный ответ: " << correctAnswer << " (" << expr1.result << " и " << expr2.result << ")\n";
                state.currentStreak = 0;
            }
        }
    }
    
    updateDifficulty(state);
    this_thread::sleep_for(chrono::seconds(3));
    return answered;
}