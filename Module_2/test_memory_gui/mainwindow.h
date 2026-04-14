#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>
#include "colormemory.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ColorMemory *game;
    void disableColorButtons(bool disable);
    void nextRound();
    bool canClick;  // можно ли нажимать кнопки
private slots:
    void on_redButton_clicked();
    void on_blueButton_clicked();
    void on_yellowButton_clicked();
    void on_greenButton_clicked();
    void on_startButton_clicked();
    void onUpdateSequence(const QString& text);
    void onCorrectGuess();
    void onUnCorrectGuess();
    void onRoundComplete();
    void onGameOver();
};

#endif // MAINWINDOW_H
