#include "colormemory.h"

ColorMemory::ColorMemory(QObject *parent)
    : QObject{parent}
    , currentIndex(0)
    , gameActive(false)
{}

void ColorMemory::addColor()
{
    static const QVector<QString> colors = {"red", "blue", "yellow", "green"};
    int index = QRandomGenerator::global()->bounded(colors.size());
    sequence.append(colors[index]);
}

void ColorMemory::startNewRound()
{
    addColor();
    gameActive = true;
    currentIndex = 0;
    emit sequenceUpdated(sequence);
}

void ColorMemory::checkColor(const QString& playerColor)
{
    if (!gameActive) return;
    if (currentIndex >= sequence.size()) return;

    if (sequence[currentIndex] == playerColor) {
        emit correctGuess();
        currentIndex++;

        if (currentIndex >= sequence.size()) {
            gameActive = false;
            emit roundComplete();
        }
    } else {
        gameActive = false;
        emit incorrectGuess();
        emit gameOver();
    }
}

void ColorMemory::reset()
{
    sequence.clear();
    currentIndex = 0;
    gameActive = false;
    emit sequenceUpdated(sequence);
}
