/********************************************************************************
** Form generated from reading UI file 'games.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMES_H
#define UI_GAMES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_games
{
public:
    QFrame *frame;
    QLabel *label;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;

    void setupUi(QWidget *games)
    {
        if (games->objectName().isEmpty())
            games->setObjectName("games");
        games->resize(736, 513);
        games->setStyleSheet(QString::fromUtf8("background-color: rgb(51, 131, 202);"));
        frame = new QFrame(games);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(180, 19, 541, 481));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 242, 253);\n"
"color: rgb(0, 0, 0);\n"
"border-radius: 20px"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setGeometry(QRect(200, 10, 131, 51));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        label->setFont(font);
        pushButton_2 = new QPushButton(games);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(-20, 100, 141, 41));
        pushButton_2->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_2->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));
        pushButton_3 = new QPushButton(games);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(-20, 160, 141, 41));
        pushButton_3->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_3->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));
        pushButton_4 = new QPushButton(games);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(-20, 220, 141, 41));
        pushButton_4->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_4->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));
        pushButton_5 = new QPushButton(games);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(-20, 280, 141, 41));
        pushButton_5->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_5->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));
        pushButton_6 = new QPushButton(games);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(-20, 340, 141, 41));
        pushButton_6->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_6->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));

        retranslateUi(games);

        QMetaObject::connectSlotsByName(games);
    } // setupUi

    void retranslateUi(QWidget *games)
    {
        games->setWindowTitle(QCoreApplication::translate("games", "Form", nullptr));
        label->setText(QCoreApplication::translate("games", "\320\241\320\277\320\270\321\201\320\276\320\272 \320\270\320\263\321\200", nullptr));
        pushButton_2->setText(QCoreApplication::translate("games", "\320\223\320\273\320\260\320\262\320\275\320\260\321\217", nullptr));
        pushButton_3->setText(QCoreApplication::translate("games", "\320\230\320\263\321\200\321\213", nullptr));
        pushButton_4->setText(QCoreApplication::translate("games", "\320\234\320\260\320\263\320\260\320\267\320\270\320\275", nullptr));
        pushButton_5->setText(QCoreApplication::translate("games", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", nullptr));
        pushButton_6->setText(QCoreApplication::translate("games", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class games: public Ui_games {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMES_H
