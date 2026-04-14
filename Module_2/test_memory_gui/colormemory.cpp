#include "colormemory.h"
#include "./ui_colormemory.h"

ColorMemory::ColorMemory(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ColorMemory)
{
    ui->setupUi(this);
}

ColorMemory::~ColorMemory()
{
    delete ui;
}
