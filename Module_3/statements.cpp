#include "statements.h"

Statements::Statements() {
    srand(time(0));
}

Statements::Statement Statements::generateStatement(int difficulty) {
    Statement st;
    
    vector<Statement> statements = {
        {"Солнце встает на востоке", true},
        {"2 + 2 = 5", false},
        {"Зимой идет снег", true},
        {"Рыбы живут в воде", true},
        {"Птицы не умеют летать", false},
        {"Земля плоская", false},
        {"В неделе 8 дней", false},
        {"Яблоко - это фрукт", true},
        {"9 × 9 = 81", true},
        {"Луна сделана из сыра", false},
        {"Вода кипит при 100°C", true},
        {"Собаки умеют мяукать", false},
        {"5 × 5 = 25", true},
        {"7 + 3 × 2 = 20", false},
        {"Квадрат имеет 4 угла", true},
        {"100 делится на 3 без остатка", false},
        {"Число 2 - единственное четное простое число", true},
        {"0 делится на любое число", true},
        {"Отрицательное число в квадрате всегда положительное", true},
        {"Сумма углов треугольника равна 180 градусов", true},
        {"Деление на ноль возможно", false},
        {"Число π равно 3.14", true},
        {"У пауков 6 ног", false},
        {"Дельфины - это млекопитающие", true},
        {"Пчелы умирают после укуса", true},
        {"Верблюды хранят воду в горбах", false},
        {"Слоны боятся мышей", false},
        {"У осьминога три сердца", true},
        {"Ленивцы спят до 20 часов в сутки", true},
        {"Белые медведи имеют черную кожу", true},
        {"Крокодилы могут высовывать язык", false},
        {"Хамелеоны меняют цвет только для маскировки", false},
        {"Телефон изобрел Александр Белл", true},
        {"Лампочку изобрел Томас Эдисон", true},
        {"Первый компьютер весил более 1 тонны", true},
        {"Wi-Fi расшифровывается как Wireless Fidelity", false},
        {"Роботы не могут распознавать лица", false}
    };
    
    // Сложн
    if (difficulty >= 2) {
        int a = rand() % 20 + 1;
        int b = rand() % 20 + 1;
        int c = rand() % 20 + 1;
        
        vector<Statement> mathStatements = {
            {to_string(a) + " + " + to_string(b) + " = " + 
             to_string(a + b), true},
            {to_string(a) + " × " + to_string(b) + " < " + 
             to_string(a * b + 1), true},
            {to_string(a) + " > " + to_string(b) + " и " + 
             to_string(b) + " > " + to_string(c), 
             (a > b) && (b > c)}
        };
        
        statements.insert(statements.end(), mathStatements.begin(), mathStatements.end());
    }
    
    st = statements[rand() % statements.size()];
    return st;
}

bool Statements::play(GameState& state) {
    clearScreen();
    printHeader("Верно ли утверждение?");
    
    Statement st = generateStatement(state.difficulty);
    
    int timeLimit = (state.difficulty == 1) ? time_limit_easy : (state.difficulty == 2) ? time_limit_medium : time_limit_hard;
    
    cout << "\nВерно ли утверждение?\n\n";
    cout << "   «" << st.text << "»\n\n";
    cout << "Введите 'да' или 'нет': ";
    
    string correctAnswer = st.isTrue ? "да" : "нет";
    
    int attempts = 0;
    bool answered = false;
    
    while (attempts < max_attempts && !answered) {
        if (attempts > 0) {
            cout << "\nПопытка " << attempts + 1 << " из " << max_attempts << ": ";
        }
        
        string userAnswer;
        auto start = chrono::steady_clock::now();
        bool inTime = waitForAnswerWithTimer(timeLimit, userAnswer);
        
        if (!inTime) {
            attempts++;
            if (attempts >= max_attempts) {
                cout << "\nПравильный ответ: " << correctAnswer << "\n";
                state.currentStreak = 0;
            }
            continue;
        }
        
        auto end = chrono::steady_clock::now();
        int timeLeft = timeLimit - chrono::duration_cast<chrono::seconds> (end - start).count();
        
        // нижний рег
        for (char& c : userAnswer) c = tolower(c);
        
        if (userAnswer == correctAnswer || 
            (userAnswer == "yes" && correctAnswer == "да") ||
            (userAnswer == "no" && correctAnswer == "нет")) {
            int earned = calculatePoints(base_point, state.difficulty, timeLeft, true);
            state.score += earned;
            state.correctAnswers++;
            state.totalQuestions++;
            state.currentStreak++;
            cout << "\nЙоу +" << earned << " очков!\n";
            answered = true;
        } else {
            attempts++;
            state.totalQuestions++;
            if (attempts >= max_attempts) {
                cout << "\nЭто утверждение " << (st.isTrue ? "верно" : "неверно") << "\n";
                state.currentStreak = 0;
            }
        }
    }
    
    updateDifficulty(state);
    this_thread::sleep_for(chrono::seconds(2));
    return answered;
}