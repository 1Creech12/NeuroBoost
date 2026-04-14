#include "colormemory.h"

ColorMemory::ColorMemory(QObject *parent)
    : QObject(parent)
    , currentIndex(0), gameActive(false) {}


void ColorMemory::addColor() {
    srand(time(0));
    QVector<QString> colors = {"red", "blue", "yellow", "green"};
    int index = rand() % 4;
    sequence.push_back(colors[index]);
}

void ColorMemory::startNewRound() {
    addColor();

    gameActive = true;
    currentIndex = 0;

    emit updateColors(sequence.join(' '));
}

void ColorMemory::checkColor(const QString& playerColor) {
    if (!gameActive) return;

    if (sequence.size() <= currentIndex) return;

    if (sequence[currentIndex] == playerColor) {
        emit correctGuess();
        currentIndex++;

        if (sequence.size() <= currentIndex) {
            gameActive = false;
            emit roundComplete();
        }
    } else {
        gameActive = false;
        emit uncorrectGuess();
        emit gameOver();
    }
}

void ColorMemory::reset() {
    sequence.clear();
    currentIndex = 0;
    gameActive = false;
    emit updateColors("");
}

