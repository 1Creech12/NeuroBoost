#ifndef COLORMEMORY_H
#define COLORMEMORY_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class ColorMemory;
}
QT_END_NAMESPACE

class ColorMemory : public QMainWindow
{
    Q_OBJECT

public:
    explicit ColorMemory(QWidget *parent = nullptr);
    ~ColorMemory() override;

private:
    Ui::ColorMemory *ui;
};
#endif // COLORMEMORY_H
