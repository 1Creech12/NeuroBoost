#ifndef COLORMEMORY_H
#define COLORMEMORY_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QRandomGenerator>
class ColorMemory : public QObject
{
    Q_OBJECT
private:
    QVector<QString> sequence;
    int currentIndex;
    bool gameActive;

    // Система сложности
    int currentLevel;           // 1-5 (6 = бесконечный)
    int baseSequenceLength;     // стартовая длина на уровне
    int totalScore;             // общий счёт
    int roundsCompletedInLevel; // сколько раундов пройдено на текущем уровне

    static const int ROUNDS_PER_LEVEL = 3;  // раундов для повышения уровня
    static const int MAX_LEVEL = 5;
signals:
    void sequenceUpdated(const QVector<QString>& sequence);
    void correctGuess();
    void incorrectGuess();
    void roundComplete();
    void gameOver();
    void levelUp(int newLevel);
public:
    explicit ColorMemory(QObject *parent = nullptr);

    void addColor();
    void checkColor(const QString& playerColor);
    void startNewRound();
    void reset();
    void nextLevel();

    const QVector<QString>& getSequence() const { return sequence; }
    int getScore() const { return totalScore; }
    int getLevel() const { return currentLevel; }
    int getBaseLength() const { return baseSequenceLength; }
};

#endif // COLORMEMORY_H