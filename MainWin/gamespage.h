#ifndef GAMESPAGE_H
#define GAMESPAGE_H

#include <QWidget>
#include "gamewidget.h"
#include "maingames.h"


namespace Ui {
class gamesPage;
}

class gamesPage : public QWidget
{
    Q_OBJECT

public:
    explicit gamesPage(QWidget *parent = nullptr);
    ~gamesPage();

private:
    Ui::gamesPage *ui;

    GameWidget *gameWidget;
    MainGames *mainGames;
};

#endif // GAMESPAGE_H
