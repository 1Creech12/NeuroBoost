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
signals:
    void sequenceUpdated(const QVector<QString>& sequence);
    void correctGuess();
    void incorrectGuess();
    void roundComplete();
    void gameOver();
public:
    explicit ColorMemory(QObject *parent = nullptr);

    void addColor();
    void checkColor(const QString& playerColor);
    void startNewRound();
    void reset();

    const QVector<QString>& getSequence() const { return sequence; }
    int getScore() const { return sequence.isEmpty() ? 0 : sequence.size() - 1; }
};

#endif // COLORMEMORY_H