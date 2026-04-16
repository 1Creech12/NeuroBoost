#include "shoppage.h"
#include "ui_shoppage.h"

shopPage::shopPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::shopPage)
{
    ui->setupUi(this);
}

shopPage::~shopPage()
{
    delete ui;
}
