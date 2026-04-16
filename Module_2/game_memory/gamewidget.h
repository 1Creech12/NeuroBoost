#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include "colormemory.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GameWidget;
}
QT_END_NAMESPACE

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget() override;

signals:
    void gameFinished(int score);

private slots:
    void on_startButton_clicked();
    void onRedClick();
    void onBlueClick();
    void onYellowClick();
    void onGreenClick();
    void onSequenceUpdated(const QVector<QString>& sequence);
    void onCorrectGuess();
    void onIncorrectGuess();
    void onRoundComplete();
    void onGameOver();

private:
    Ui::GameWidget *ui;

    // Игра
    ColorMemory *game;

    // Таймеры
    QTimer *showSequenceTimer;
    QTimer *hideDelayTimer;
    int currentShowIndex;
    QVector<QString> currentSequence;

    // Методы
    void disableColorButtons(bool disable);
    void startShowingSequence();
    void showNextColor();
    void hideDisplay();
    void setDisplayColor(const QString& color);
};
#endif // GAMEWIDGET_H
