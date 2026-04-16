/********************************************************************************
** Form generated from reading UI file 'mainModule_1.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINMODULE_1_H
#define UI_MAINMODULE_1_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainModule_1
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QLabel *label_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainModule_1)
    {
        if (MainModule_1->objectName().isEmpty())
            MainModule_1->setObjectName("MainModule_1");
        MainModule_1->resize(800, 600);
        centralwidget = new QWidget(MainModule_1);
        centralwidget->setObjectName("centralwidget");
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(240, 170, 284, 118));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName("label");

        verticalLayout_2->addWidget(label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName("lineEdit");

        verticalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName("pushButton");

        verticalLayout->addWidget(pushButton);


        verticalLayout_2->addLayout(verticalLayout);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName("label_2");

        verticalLayout_2->addWidget(label_2);

        MainModule_1->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainModule_1);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 23));
        MainModule_1->setMenuBar(menubar);
        statusbar = new QStatusBar(MainModule_1);
        statusbar->setObjectName("statusbar");
        MainModule_1->setStatusBar(statusbar);

        retranslateUi(MainModule_1);

        QMetaObject::connectSlotsByName(MainModule_1);
    } // setupUi

    void retranslateUi(QMainWindow *MainModule_1)
    {
        MainModule_1->setWindowTitle(QCoreApplication::translate("MainModule_1", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainModule_1", "\320\222\320\260\321\210\320\260 \320\267\320\260\320\264\320\260\321\207\320\260 \321\200\320\265\321\210\320\270\321\202\321\214 \321\203\321\200\320\260\320\262\320\275\320\265\320\275\320\270\321\217 \320\275\320\260 \321\201\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
        pushButton->setText(QCoreApplication::translate("MainModule_1", "\320\275\320\260\321\207\320\260\321\202\321\214", nullptr));
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainModule_1: public Ui_MainModule_1 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINMODULE_1_H
