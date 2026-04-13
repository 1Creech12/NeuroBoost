#include <iostream>
#include <random>

using namespace std;
enum class Difficulty_level {
    EASY,
    MEDIUM,
    HARD
};
int main() {
    mt19937 generate_number(random_device{}());
    uniform_int_distribution<int> dist(1, 100);
    int choice;
    cin >> choice;
    Difficulty_level input = static_cast<Difficulty_level>(choice);
    switch (input)
    {
    case Difficulty_level::EASY:
        /* code */
        break;
    case Difficulty_level::MEDIUM:
        /* code */
        break;
    case Difficulty_level::HARD:
        /* code */
        break;
    default:
        break;
    }

    cout << dist(generate_number) << endl;
    cout << dist(generate_number)<<endl;


    return 0;
}