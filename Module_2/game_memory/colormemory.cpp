#include "colormemory.h"

ColorMemory::ColorMemory(QObject *parent)
    : QObject{parent}
    , currentIndex(0)
    , gameActive(false)
    , currentLevel(1)
    , baseSequenceLength(3)
    , totalScore(0)
    , roundsCompletedInLevel(0)
{}

void ColorMemory::addColor()
{
    static const QVector<QString> colors = {"red", "blue", "yellow", "green"};
    int index = QRandomGenerator::global()->bounded(colors.size());
    sequence.append(colors[index]);
}

void ColorMemory::startNewRound()
{
    sequence.clear();
    for (int i = 0; i < baseSequenceLength; i++) {
        addColor();
    }

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

            int pointsEarned = (currentLevel > 5) ? 5 : currentLevel;

            totalScore += pointsEarned;
            roundsCompletedInLevel++;

            emit roundComplete();

            if (currentLevel <= MAX_LEVEL && roundsCompletedInLevel >= ROUNDS_PER_LEVEL) {
                nextLevel();
            }
        }
    } else {
        gameActive = false;
        emit incorrectGuess();
        emit gameOver();
    }
}

void ColorMemory::nextLevel() {
    if (currentLevel < MAX_LEVEL) {
        currentLevel++;
        baseSequenceLength++;
    } else if (currentLevel == MAX_LEVEL) {
        currentLevel++;
        baseSequenceLength++;
    } else {
        baseSequenceLength++;
    }

    roundsCompletedInLevel = 0;
    emit levelUp(currentLevel);
}

void ColorMemory::reset()
{
    sequence.clear();
    currentIndex = 0;
    gameActive = false;
    currentLevel = 1;
    baseSequenceLength = 3;
    totalScore = 0;
    roundsCompletedInLevel = 0;
    emit sequenceUpdated(sequence);
}
