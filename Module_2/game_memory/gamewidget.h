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
#include "databaseqt.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GameWidget;
}
QT_END_NAMESPACE

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr, PlayerDatabase* db = nullptr, PlayerData* player = nullptr);
    ~GameWidget();

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
    void onLevelUp(int newLevel);

private:
    Ui::GameWidget *ui;

    // Игра
    ColorMemory *game;

    // Таймеры
    QTimer *showSequenceTimer;
    QTimer *hideDelayTimer;
    int currentShowIndex;
    QVector<QString> currentSequence;

    PlayerDatabase* m_db;
    PlayerData* m_currentPlayer;

    // Методы
    void disableColorButtons(bool disable);
    void startShowingSequence();
    void showNextColor();
    void hideDisplay();
    void setDisplayColor(const QString& color);
    int getShowInterval() const;
    int getHideInterval() const;
};

#endif // GAMEWIDGET_H