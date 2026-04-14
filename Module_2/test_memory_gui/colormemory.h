#ifndef COLORMEMORY_H
#define COLORMEMORY_H

#include <QObject>

class ColorMemory : public QObject
{
    Q_OBJECT
private:
    QVector<QString> sequence; // последовательность
    int currentIndex; // какой цвет сейчас жмем (индекс)
    bool gameActive;    // активна ли игра
public:
    explicit ColorMemory(QObject *parent = nullptr);

    void addColor(); // добавляет цвет в последовательность
    void checkColor(const QString& playerColor); //  проверяет правильно ли игрок угадал последовательность
    const QVector<QString>& getSequence() const {return sequence;}
    void startNewRound();
    void reset();

signals:
    void updateColors(const QString& colorsText); // Обновить цвета на экране
    void correctGuess(); // правильный выбор
    void uncorrectGuess(); // не приавльный выбор
    void roundComplete(); // раунд пройден
    void gameOver(); // игра окончена
};
#endif // COLORMEMORY_H
