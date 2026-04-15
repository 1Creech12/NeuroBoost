#include "colormemory.h"

ColorMemory::ColorMemory(QObject *parent)
    : QObject(parent)
    , currentIndex(0), gameActive(false), isProcessing(false) {
    srand(static_cast<unsigned int>(time(0)));
}


void ColorMemory::addColor() {
    QVector<QString> colors = {"red", "blue", "yellow", "green"};
    int index = rand() % 4;
    sequence.push_back(colors[index]);
    qDebug() << "=== ADD COLOR === New sequence:" << sequence;  // Отладка
}

void ColorMemory::startNewRound() {
    addColor();

    gameActive = true;
    currentIndex = 0;
    isProcessing = false;

    emit updateColors(sequence.join(' '));
    qDebug() << "=== NEW ROUND === Sequence:"
             << sequence << "Length:" << sequence.size();
}

void ColorMemory::checkColor(const QString& playerColor) {
    qDebug() << "=== CHECK ==="
             << "Active:" << gameActive
             << "Index:" << currentIndex
             << "Need:" << (currentIndex < sequence.size() ? sequence[currentIndex] : "N/A")
             << "Got:" << playerColor;

    if (isProcessing) {
        qDebug() << "=== CHECK === BLOCKED: Processing another click";
        return;
    }


    if (!gameActive) return;

    isProcessing = true;

    if (sequence.size() <= currentIndex) return;

    if (sequence[currentIndex] == playerColor) {
        emit correctGuess();
        currentIndex++;

        if (sequence.size() <= currentIndex) {
            gameActive = false;
            isProcessing = false;
            qDebug() << "=== ROUND COMPLETE ===";
            emit roundComplete();
        } else {
            isProcessing = false;
        }
    } else {
        gameActive = false;
        isProcessing = false;
        emit uncorrectGuess();
        emit gameOver();
    }
}

void ColorMemory::reset() {
    sequence.clear();
    currentIndex = 0;
    gameActive = false;
    isProcessing = false;
    emit updateColors("");
}

