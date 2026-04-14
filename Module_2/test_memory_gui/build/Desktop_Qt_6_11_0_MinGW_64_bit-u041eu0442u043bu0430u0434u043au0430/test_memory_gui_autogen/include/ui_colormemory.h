/********************************************************************************
** Form generated from reading UI file 'colormemory.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORMEMORY_H
#define UI_COLORMEMORY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ColorMemory
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ColorMemory)
    {
        if (ColorMemory->objectName().isEmpty())
            ColorMemory->setObjectName("ColorMemory");
        ColorMemory->resize(800, 600);
        centralwidget = new QWidget(ColorMemory);
        centralwidget->setObjectName("centralwidget");
        ColorMemory->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ColorMemory);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        ColorMemory->setMenuBar(menubar);
        statusbar = new QStatusBar(ColorMemory);
        statusbar->setObjectName("statusbar");
        ColorMemory->setStatusBar(statusbar);

        retranslateUi(ColorMemory);

        QMetaObject::connectSlotsByName(ColorMemory);
    } // setupUi

    void retranslateUi(QMainWindow *ColorMemory)
    {
        ColorMemory->setWindowTitle(QCoreApplication::translate("ColorMemory", "ColorMemory", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ColorMemory: public Ui_ColorMemory {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORMEMORY_H
