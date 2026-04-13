#include <iostream>
#include <vector>
#include <string>

using namespace std;

int random(int min, int max) {
    return rand() % (max-min+1)+min;
}

void Sequence() {
    srand(time(0));

    cout << "\nПродолжить числовую последовательность\nДля выхода введите 'q'\n";
    int score = 0;
    int total = 0;

    while (true) {
        int start = random(1, 10);
        int rule = random(0, 3);
        int len = random(3, 5);

        vector<int> numbers;
        int current = start;

        cout << "Последовательность: ";
        for (int i = 0; i < len; i++) {
            numbers.push_back(current);
            cout << current;
            if (i < len - 1) {
                cout << ", ";
            }
            switch (rule) {
                case 0: current += 2; break;
                case 1: current *= 2; break;
                case 2: current += 2; break;
                case 3: current *= 2; break;
            
            }
        }
        cout << ", ?/n";
        cout << "ВВод: ";
        string input;
        getline(cin, input);

        if (input == "q") break;

        total++;
        int answer = stoi(input);

        if (answer == current) {
            cout << "Правильно\n";
            score++;
        } else {
            cout << "Неправильно, ответ: " << current << "\n";
        }

        cout << "Счет: " << score << "/" << total << "\n";
    }
    cout << "Итог: " << score << " из " << total << "\n";
}