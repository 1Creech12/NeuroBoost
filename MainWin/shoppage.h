#ifndef SHOPPAGE_H
#define SHOPPAGE_H

#include <QWidget>

namespace Ui {
class shopPage;
}

class shopPage : public QWidget
{
    Q_OBJECT

public:
    explicit shopPage(QWidget *parent = nullptr);
    ~shopPage();

private:
    Ui::shopPage *ui;
};

#endif // SHOPPAGE_H
