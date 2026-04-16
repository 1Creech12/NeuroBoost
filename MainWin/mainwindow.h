#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "shoppage.h"
#include "homepage.h"
#include "gamespage.h"

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
    ~MainWindow() override;

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    shopPage *shopPage;
    homePage *homePage;
    gamesPage *gamesPage;

    void animateButton(QPushButton* button);
};
#endif // MAINWINDOW_H
