#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QVariantAnimation>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    shopPage = new class shopPage(this);
    homePage = new class homePage(this);
    gamesPage = new class gamesPage(this);

    ui->stackedWidget->addWidget(homePage);
    ui->stackedWidget->addWidget(gamesPage);
    ui->stackedWidget->addWidget(shopPage);


    connect(ui->btnShop, &QPushButton::clicked, [this]() {
        ui->stackedWidget->setCurrentWidget(shopPage);

        animateButton(ui->btnShop);

        ui->btnHome->setStyleSheet(
            "background-color: rgb(234, 234, 234);"
            "border-radius:20px;"
            );
        ui->btnGames->setStyleSheet(
            "background-color: rgb(234, 234, 234);"
            "border-radius:20px;"
            );
    });

    connect(ui->btnGames, &QPushButton::clicked, [this]() {
        ui->stackedWidget->setCurrentWidget(gamesPage);

        animateButton(ui->btnGames);

        ui->btnHome->setStyleSheet(
            "background-color: rgb(234, 234, 234);"
            "border-radius:20px;"
            );
        ui->btnShop->setStyleSheet(
            "background-color: rgb(234, 234, 234);"
            "border-radius:20px;"
            );
    });

    connect(ui->btnHome, &QPushButton::clicked, [this]() {
        ui->stackedWidget->setCurrentWidget(homePage);

        animateButton(ui->btnHome);

        ui->btnShop->setStyleSheet(
            "background-color: rgb(234, 234, 234);"
            "border-radius:20px;"
            );
        ui->btnGames->setStyleSheet(
            "background-color: rgb(234, 234, 234);"
            "border-radius:20px;"
            );
    });


}

void MainWindow::animateButton(QPushButton* button) {
    QVariantAnimation* anim = new QVariantAnimation(this);
    anim->setDuration(250); // Скорость анимации (0.25 сек)


    anim->setStartValue(QColor(60, 60, 60));
    anim->setEndValue(QColor(0, 120, 215));

    connect(anim, &QVariantAnimation::valueChanged, [button](const QVariant &value) {
        QString col = value.value<QColor>().name();
        // Применяем через Style Sheet
        button->setStyleSheet(QString(
                                  "QPushButton {"
                                  "  background-color: %1;"
                                  "  color: white;"
                                  "  border-radius: 8px;"
                                  "  border: none;"
                                  "  padding: 10px;"
                                  "}"
                                  ).arg(col));
    });

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

MainWindow::~MainWindow()
{
    delete ui;
}



