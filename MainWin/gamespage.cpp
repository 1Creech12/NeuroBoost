#include "gamespage.h"
#include "ui_gamespage.h"

gamesPage::gamesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gamesPage)
{
    ui->setupUi(this);

    gameWidget = new class GameWidget;

    connect(ui->playFourColor, &QPushButton::clicked, [this]() {
        gameWidget->show();
    });
}

gamesPage::~gamesPage()
{
    delete ui;
}
