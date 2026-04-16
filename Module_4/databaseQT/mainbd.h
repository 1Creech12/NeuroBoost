#ifndef MAINBD_H
#define MAINBD_H

#include <QMainWindow>
#include <QStandardItemModel>  // ← ДОБАВИТЬ!
#include "databaseqt.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();        // Внести данные
    void on_pushButton_2_clicked();      // Начислить очки (тест)
    void on_top100Button_clicked();      // Обновить топ-100
    void on_top20PointsButton_clicked(); // Топ-20 по очкам
    void on_top20DiamondsButton_clicked(); // Топ-20 по алмазам
    void on_showLeaderboardButton_clicked(); // ← Показать таблицу лидеров в ListView

private:
    Ui::MainWindow *ui;
    PlayerDatabase *m_playerDb;
    QStandardItemModel *m_leaderboardModel;  // ← Модель для ListView

    void clearInputs();
    void updateLeaderboard();  // ← Обновить ListView
};

#endif // MAINBD_H