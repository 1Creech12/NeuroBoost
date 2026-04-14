#include "excess.h"

Excess::Excess() {
    srand(time(0));
}

vector<Excess::Item> Excess::generateItems(int difficulty) {
    vector<Item> items;
    
    // Базы данных по категориям
    vector<vector<Item>> categories = {
        {{"яблоко", "фрукты"}, {"банан", "фрукты"}, {"апельсин", "фрукты"}, {"груша", "фрукты"}, {"морковь", "овощи"}},
        {{"собака", "животные"}, {"кошка", "животные"}, {"корова", "животные"}, {"лошадь", "животные"}, {"стол", "мебель"}},
        {{"красный", "цвета"}, {"синий", "цвета"}, {"зеленый", "цвета"}, {"желтый", "цвета"}, {"круг", "формы"}},
        {{"2", "четные"}, {"4", "четные"}, {"6", "четные"}, {"8", "четные"}, {"3", "нечетные"}},
        {{"лето", "времена года"}, {"зима", "времена года"}, {"весна", "времена года"}, {"осень", "времена года"}, {"понедельник", "дни недели"}}
    };
    
    int categoryIndex = rand() % categories.size();
    items = categories[categoryIndex];
    
    // Мешать
    for (int i = items.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(items[i], items[j]);
    }
    
    // Сложн
    if (difficulty > 1 && categories.size() > categoryIndex + 1) {
        items.push_back(categories[categoryIndex + 1][rand() % 5]);
    }
    
    return items;
}

int Excess::findOddIndex(const vector<Item>& items) {
    // поиск
    string mainCategory;
    for (size_t i = 0; i < items.size(); i++) {
        int count = 0;
        for (size_t j = 0; j < items.size(); j++) {
            if (i != j && items[i].category == items[j].category) {
                count++;
            }
        }
        if (count >= items.size() / 2) {
            mainCategory = items[i].category;
            break;
        }
    }
    
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].category != mainCategory) {
            return i;
        }
    }
    return 0;
}

bool Excess::play(GameState& state) {
    clearScreen();
    printHeader("Найди лишнее");
    
    vector<Item> items = generateItems(state.difficulty);
    int oddIndex = findOddIndex(items);
    
    int timeLimit = (state.difficulty == 1) ? time_limit_easy : (state.difficulty == 2) ? time_limit_medium : time_limit_hard;
    
    cout << "\nКакой элемент лишний?\n\n";
    for (size_t i = 0; i < items.size(); i++) {
        cout << i + 1 << ". " << items[i].name << "\n";
    }
    cout << "\n";
    
    int attempts = 0;
    bool answered = false;
    
    while (attempts < max_attempts && !answered) {
        if (attempts > 0) {
            cout << "\nНеправильно, попытка " << attempts + 1 << " из " << max_attempts << "\n";
        }
        
        string userAnswer;
        auto start = chrono::steady_clock::now();
        bool inTime = waitForAnswerWithTimer(timeLimit, userAnswer);
        
        if (!inTime) {
            attempts++;
            if (attempts >= max_attempts) {
                cout << "\nПравильный ответ: " << oddIndex + 1 
                         << " (" << items[oddIndex].name << ")\n";
                state.currentStreak = 0;
            }
            continue;
        }
        
        auto end = chrono::steady_clock::now();
        int timeLeft = timeLimit - chrono::duration_cast<chrono::seconds> (end - start).count();
        
        int answer = 0;
        try {
            answer = stoi(userAnswer);
        } catch(...) {
            attempts++;
            continue;
        }
        
        if (answer == oddIndex + 1) {
            int earned = calculatePoints(base_point, state.difficulty, timeLeft, true);
            state.score += earned;
            state.correctAnswers++;
            state.totalQuestions++;
            state.currentStreak++;
            cout << "\nЙоу +" << earned << " очков!\n";
            cout << "💡 " << items[oddIndex].name << " не относится к категории '" << items[(oddIndex + 1) % items.size()].category << "'\n";
            answered = true;
        } else {
            attempts++;
            state.totalQuestions++;
            if (attempts >= max_attempts) {
                cout << "\nПравильный ответ: " << oddIndex + 1 
                         << " (" << items[oddIndex].name << ")\n";
                state.currentStreak = 0;
            }
        }
    }
    
    updateDifficulty(state);
    this_thread::sleep_for(chrono::seconds(3));
    return answered;
}