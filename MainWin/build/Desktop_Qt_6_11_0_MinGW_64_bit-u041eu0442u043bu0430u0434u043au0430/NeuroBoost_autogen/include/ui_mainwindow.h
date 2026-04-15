/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *frame;
    QListView *listView;
    QLabel *label;
    QFrame *frame_2;
    QListView *listView_2;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QMenuBar *menubar;
    QMenu *menuNeuroBoot;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(769, 560);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(51, 131, 202);"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(130, 50, 621, 451));
        frame->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"color: rgb(0, 0, 0);\n"
"background-color: rgb(227, 242, 253);"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        listView = new QListView(frame);
        listView->setObjectName("listView");
        listView->setGeometry(QRect(440, 320, 161, 121));
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setGeometry(QRect(110, 40, 211, 41));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        font.setStyleStrategy(QFont::PreferDefault);
        label->setFont(font);
        frame_2 = new QFrame(frame);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(60, 30, 41, 41));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        listView_2 = new QListView(frame);
        listView_2->setObjectName("listView_2");
        listView_2->setGeometry(QRect(20, 340, 341, 91));
        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 290, 211, 41));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        label_2->setFont(font1);
        label_2->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(430, 290, 191, 31));
        label_3->setFont(font1);
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(-20, 110, 141, 41));
        pushButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(-20, 170, 141, 41));
        pushButton_2->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_2->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(-20, 230, 141, 41));
        pushButton_3->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_3->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));
        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(-20, 290, 141, 41));
        pushButton_4->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_4->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));
        pushButton_5 = new QPushButton(centralwidget);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(-20, 350, 141, 41));
        pushButton_5->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_5->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 769, 25));
        menuNeuroBoot = new QMenu(menubar);
        menuNeuroBoot->setObjectName("menuNeuroBoot");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuNeuroBoot->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "SomeText", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\234\320\276\320\271 \320\277\321\200\320\276\320\263\321\200\320\265\321\201\321\201", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\242\320\260\320\261\320\273\320\270\321\206\320\260 \321\200\320\265\320\272\320\276\321\200\320\264\320\276\320\262", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "\320\223\320\273\320\260\320\262\320\275\320\260\321\217", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "\320\230\320\263\321\200\321\213", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "\320\234\320\260\320\263\320\260\320\267\320\270\320\275", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
        menuNeuroBoot->setTitle(QCoreApplication::translate("MainWindow", "NeuroBoost", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
