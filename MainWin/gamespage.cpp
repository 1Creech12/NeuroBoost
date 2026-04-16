#include "gamespage.h"
#include "ui_gamespage.h"

gamesPage::gamesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gamesPage)
{
    ui->setupUi(this);

    gameWidget = new class GameWidget;
    mainGames = new class MainGames;

    connect(ui->playFourColor, &QPushButton::clicked, [this]() {
        gameWidget->show();
    });

    connect(ui->playLogic, &QPushButton::clicked, [this]() {
        mainGames->show();
    });
}

gamesPage::~gamesPage()
{
    delete ui;
}
