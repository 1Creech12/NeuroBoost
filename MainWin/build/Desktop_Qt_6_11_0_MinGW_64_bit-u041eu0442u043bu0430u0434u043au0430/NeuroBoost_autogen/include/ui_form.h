/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QWidget *widget;
    QPushButton *pushButton_5;
    QPushButton *pushButton_2;
    QPushButton *pushButton_4;
    QPushButton *pushButton_3;
    QPushButton *pushButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName("Form");
        Form->resize(608, 527);
        Form->setStyleSheet(QString::fromUtf8("background-color: rgb(51, 131, 202);"));
        widget = new QWidget(Form);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(130, 10, 461, 501));
        widget->setStyleSheet(QString::fromUtf8("background-color: rgb(227, 242, 253);\n"
"border-radius: 20px;"));
        pushButton_5 = new QPushButton(Form);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(-20, 310, 141, 41));
        pushButton_5->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_5->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));
        pushButton_2 = new QPushButton(Form);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(-20, 130, 141, 41));
        pushButton_2->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_2->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));
        pushButton_4 = new QPushButton(Form);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(-20, 250, 141, 41));
        pushButton_4->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_4->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));
        pushButton_3 = new QPushButton(Form);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(-20, 190, 141, 41));
        pushButton_3->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton_3->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));
        pushButton = new QPushButton(Form);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(-20, 70, 141, 41));
        pushButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pushButton->setStyleSheet(QString::fromUtf8("border-radius: 20px;\n"
"background-color: rgb(35, 109, 174);\n"
"color: rgb(255, 255, 255);\n"
"font: 600 9pt \"Segoe UI\";"));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        pushButton_5->setText(QCoreApplication::translate("Form", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Form", "\320\230\320\263\321\200\321\213", nullptr));
        pushButton_4->setText(QCoreApplication::translate("Form", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Form", "\320\234\320\260\320\263\320\260\320\267\320\270\320\275", nullptr));
        pushButton->setText(QCoreApplication::translate("Form", "\320\223\320\273\320\260\320\262\320\275\320\260\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
