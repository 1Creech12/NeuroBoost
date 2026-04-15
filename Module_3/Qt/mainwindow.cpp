#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_seqTimer = new Timer;
    m_excessTimer = new Timer;
    m_compareTimer = new Timer;
    m_statementTimer = new Timer;
    m_memoryTimer = new Timer;

    ui->stackedWidget->setCurrentIndex(0);

    updateAllStatsLabels();
}

MainWindow::~MainWindow()
{
    m_gameActive = false;
    stopAllTimers();
    delete m_seqTimer;
    delete m_excessTimer;
    delete m_compareTimer;
    delete m_statementTimer;
    delete m_memoryTimer;
    delete ui;
}

template<typename T>
void shuffleVector(std::vector<T>& vec) {
    for (int i = vec.size() - 1; i > 0; i--) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        std::swap(vec[i], vec[j]);
    }
}

void MainWindow::on_sequenceBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    m_gameActive = true;
    generateSequence();

    m_seqTimer->reset();

    // Запуск таймера (время зависит от сложности)
    m_timeLimit = 30 - (m_difficulty * 3);
    if (m_timeLimit < 10) m_timeLimit = 10;
    ui->labelSeqTimer->setText("Время: " + QString::number(m_timeLimit) + "с");
    startTimerMonitoring();

}

void MainWindow::on_excessBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    m_currentGame = 2;
    generateExcess();
    startGameTimer(2);
}


void MainWindow::on_compareBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    m_currentGame = 3;
    generateCompare();
    startGameTimer(3);
}


void MainWindow::on_statermentsBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    m_currentGame = 4;
    generateStatement();
    startGameTimer(4);
}


void MainWindow::on_memoryBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    m_currentGame = 5;
    generateMemory();
    startGameTimer(5);
}

void MainWindow::startTimerMonitoring()
    {
        // Запускаем мониторинг в отдельном потоке
        QThread *monitorThread = QThread::create([this]() {
            int lastSeconds = 0;

            while (m_gameActive) {
                int currentSeconds = m_seqTimer->get();

                if (currentSeconds != lastSeconds) {
                    lastSeconds = currentSeconds;

                    // Обновляем UI через главный поток
                    QMetaObject::invokeMethod(this, [this, currentSeconds]() {
                        int timeLeft = m_timeLimit - currentSeconds;

                        if (m_gameActive) {
                            ui->labelSeqTimer->setText("Время: " + QString::number(timeLeft) + "с");

                            // Проверка на окончание времени
                            if (timeLeft <= 0) {
                                m_gameActive = false;
                                ui->labelSeqFeedback->setText("⏰ Время вышло! Ответ: " + QString::number(m_correctAnswer));
                                updateCombo(false);

                                QTimer::singleShot(2000, this, [this]() {
                                    if (ui->stackedWidget->currentIndex() == 1) {
                                        m_gameActive = true;
                                        generateSequence();
                                        m_seqTimer->reset();
                                        m_timeLimit = 30 - (m_difficulty * 3);
                                        if (m_timeLimit < 10) m_timeLimit = 10;
                                        ui->labelSeqTimer->setText("Время: " + QString::number(m_timeLimit) + "с");
                                        startTimerMonitoring();
                                    }
                                });
                            }
                        }
                    });
                }

                QThread::msleep(100); // каждые 100 мс
            }
        });

        connect(monitorThread, &QThread::finished, monitorThread, &QObject::deleteLater);
        monitorThread->start();
    }

void MainWindow::on_randomPlayBtn_clicked()
{

    int randomPage = QRandomGenerator::global()->bounded(1, 6);
    ui->stackedWidget->setCurrentIndex(randomPage);

    switch(randomPage) {
    case 1: on_sequenceBtn_clicked(); break;
    case 2: on_excessBtn_clicked(); break;
    case 3: on_compareBtn_clicked(); break;
    case 4: on_statermentsBtn_clicked(); break;
    case 5: on_memoryBtn_clicked(); break;
    }
}

void MainWindow::on_BtnSeqBack_clicked()
{
    stopAllTimers();
    returnToMainMenu();
}

void MainWindow::on_btnExcessBack_clicked()
{
    stopAllTimers();
    returnToMainMenu();
}


void MainWindow::on_btnCompareBack_clicked()
{
    stopAllTimers();
    returnToMainMenu();
}


void MainWindow::on_btnStatermentBack_clicked()
{
    stopAllTimers();
    returnToMainMenu();
}


void MainWindow::on_btnMemoryBack_clicked()
{
    stopAllTimers();
    returnToMainMenu();
}

void MainWindow::returnToMainMenu()
{
    ui->stackedWidget->setCurrentIndex(0);
    updateAllStatsLabels();
}

void MainWindow::generateSequence()
{
    // Генерация арифметической прогрессии
    int start = QRandomGenerator::global()->bounded(1, 20);

    // Шаг зависит от сложности
    int stepMin = 1;
    int stepMax = 3 + m_difficulty;
    int step = QRandomGenerator::global()->bounded(stepMin, stepMax);

    // Случайно делаем шаг отрицательным (убывающая прогрессия)
    if (QRandomGenerator::global()->bounded(2) == 1) {
        step = -step;
    }

    // Формируем строку последовательности (4 элемента)
    QString sequence;
    for (int i = 0; i < 4; i++) {
        sequence += QString::number(start + i * step);
        if (i < 3) sequence += ", ";
    }

    // Правильный ответ - 5-й элемент
    m_correctAnswer = start + 4 * step;

    // Отображаем
    ui->labelSeqQuestion->setText("Продолжите последовательность:\n" + sequence + ", ?");
    ui->inputSeqAnswer->clear();
    ui->inputSeqAnswer->setFocus();
    ui->labelSeqFeedback->clear();
}

void MainWindow::on_btnSeqSubmit_clicked()
{
    if (!m_gameActive) return;

    QString inputText = ui->inputSeqAnswer->text();
    bool ok;
    int answer = inputText.toInt(&ok);

    if (!ok) {
        ui->labelSeqFeedback->setText("Введите число");
        return;
    }

    bool correct = (answer == m_correctAnswer);
    int timeSpent = m_seqTimer->get();

    if (correct) {
        int totalPoint = add_point_Seq+m_combo*5;

        ui->labelSeqFeedback->setText("Верно +" + QString::number(totalPoint));
        addScore(totalPoint);
        updateCombo(true);
        m_gameActive = false;

        // Следующее задание через 1.5 секунды
        QTimer::singleShot(1500, this, [this]() {
            if (ui->stackedWidget->currentIndex() == 1){
                m_gameActive = true;
                generateSequence();
                m_seqTimer->reset();
                m_timeLimit = 30 - (m_difficulty * 3);
                if (m_timeLimit < 10) m_timeLimit = 10;
                ui->labelSeqTimer->setText("Время: " + QString::number(m_timeLimit) + "с");
                startTimerMonitoring();
            }
        });
    } else {
        ui->labelSeqFeedback->setText("Неверно, правильно: " + QString::number(m_correctAnswer));
        deductScore(ded_point_Seq);
        updateCombo(false);

        QTimer::singleShot(1500, this, [this]() {
            if (ui->stackedWidget->currentIndex() == 1){
                m_gameActive = true;
                generateSequence();
                m_seqTimer->reset();
                m_timeLimit = 30 - (m_difficulty * 3);
                if (m_timeLimit < 10) m_timeLimit = 10;
                ui->labelSeqTimer->setText("Время: " + QString::number(m_timeLimit) + "с");
                startTimerMonitoring();
            }
        });
    }
}

void MainWindow::generateExcess()
{
    // База данных объектов по категориям
    vector<ExcessObject> allObjects = {
        {"🍎 Яблоко", "Фрукт"},
        {"🍌 Банан", "Фрукт"},
        {"🍊 Апельсин", "Фрукт"},
        {"🍇 Виноград", "Фрукт"},
        {"🍓 Клубника", "Фрукт"},

        {"🥕 Морковь", "Овощ"},
        {"🥔 Картофель", "Овощ"},
        {"🥦 Брокколи", "Овощ"},
        {"🍅 Помидор", "Овощ"},
        {"🥒 Огурец", "Овощ"},


        {"🐕 Собака", "Животное"},
        {"🐈 Кошка", "Животное"},
        {"🐄 Корова", "Животное"},
        {"🐑 Овца", "Животное"},
        {"🐎 Лошадь", "Животное"},

        {"🚗 Машина", "Транспорт"},
        {"✈️ Самолет", "Транспорт"},
        {"🚂 Поезд", "Транспорт"},
        {"🚲 Велосипед", "Транспорт"},
        {"🛴 Самокат", "Транспорт"},

        {"🪑 Стул", "Мебель"},
        {"🛏️ Кровать", "Мебель"},
        {"📦 Шкаф", "Мебель"},
        {"🛋️ Диван", "Мебель"},

        {"🔨 Молоток", "Инструмент"},
        {"🪛 Отвертка", "Инструмент"},
        {"🔧 Гаечный ключ", "Инструмент"},
        {"⛏️ Кирка", "Инструмент"},

        {"💧 Вода", "Напиток"},
        {"🥛 Молоко", "Напиток"},
        {"☕ Кофе", "Напиток"},
        {"🍵 Чай", "Напиток"},

        {"👕 Футболка", "Одежда"},
        {"👖 Джинсы", "Одежда"},
        {"🧥 Куртка", "Одежда"},
        {"🧦 Носки", "Одежда"}
    };

    m_currentObjects.clear();

    // Выбираем случайную категорию (для 3 объектов)
    int categoryCount = 8;  // количество категорий
    int mainCategory = QRandomGenerator::global()->bounded(categoryCount);

    QString targetCategory;
    switch(mainCategory) {
    case 0: targetCategory = "Фрукт"; break;
    case 1: targetCategory = "Овощ"; break;
    case 2: targetCategory = "Животное"; break;
    case 3: targetCategory = "Транспорт"; break;
    case 4: targetCategory = "Мебель"; break;
    case 5: targetCategory = "Инструмент"; break;
    case 6: targetCategory = "Напиток"; break;
    case 7: targetCategory = "Одежда"; break;
    }

    // Собираем объекты нужной категории
    vector<ExcessObject> sameCategory;
    for (const auto& obj : allObjects) {
        if (obj.category == targetCategory) {
            sameCategory.push_back(obj);
        }
    }

    // Выбираем 3 случайных объекта из одной категории
    shuffleVector(m_currentObjects);
    for (int i = 0; i < 3 && i < (int)sameCategory.size(); i++) {
        m_currentObjects.push_back(sameCategory[i]);
    }

    // Выбираем 1 объект из другой категории (лишний)
    int otherCategory;
    do {
        otherCategory = QRandomGenerator::global()->bounded(categoryCount);
    } while (otherCategory == mainCategory);

    QString otherCat;
    switch(otherCategory) {
    case 0: otherCat = "Фрукт"; break;
    case 1: otherCat = "Овощ"; break;
    case 2: otherCat = "Животное"; break;
    case 3: otherCat = "Транспорт"; break;
    case 4: otherCat = "Мебель"; break;
    case 5: otherCat = "Инструмент"; break;
    case 6: otherCat = "Напиток"; break;
    case 7: otherCat = "Одежда"; break;
    }

    vector<ExcessObject> otherCategoryObjects;
    for (const auto& obj : allObjects) {
        if (obj.category == otherCat) {
            otherCategoryObjects.push_back(obj);
        }
    }

    // Добавляем лишний объект
    int randomIndex = QRandomGenerator::global()->bounded((int)otherCategoryObjects.size());
    m_currentObjects.push_back(otherCategoryObjects[randomIndex]);

    // Перемешиваем порядок
    shuffleVector(m_currentObjects);

    // Находим индекс лишнего объекта (у него категория отличается)
    for (int i = 0; i < 4; i++) {
        if (m_currentObjects[i].category != targetCategory) {
            m_correctOption = i;
            break;
        }
    }

    // Отображаем на кнопках
    ui->btnOpt1->setText(m_currentObjects[0].name);
    ui->btnOpt2->setText(m_currentObjects[1].name);
    ui->btnOpt3->setText(m_currentObjects[2].name);
    ui->btnOpt4->setText(m_currentObjects[3].name);

    ui->labelExcessQuestion->setText("Какой объект лишний?");
    ui->labelExcessFeedback->clear();

    qDebug() << "Excess: категория =" << targetCategory << ", лишний индекс =" << m_correctOption;
}

void MainWindow::on_btnOpt1_clicked()
{
    if (!m_gameActive) return;

    bool correct = (m_correctOption == 0);

    if (correct) {
        QString category = m_currentObjects[0].category;
        ui->labelExcessFeedback->setText("Верно! Это " + category + " среди " + m_currentObjects[m_correctOption == 0 ? 1 : 0].category);
        addScore(add_point_Excess + m_combo * 5);
        updateCombo(true);
        m_gameActive = false;
        QTimer::singleShot(2000, this, [this]() {
            generateExcess();
            startGameTimer(2);
        });
    } else {
        ui->labelExcessFeedback->setText("Неверно! Попробуй еще");
        deductScore(ded_point_Excess);
        updateCombo(false);
    }
}
void MainWindow::on_btnOpt2_clicked()
{
    if (!m_gameActive) return;

    bool correct = (m_correctOption == 1);

    if (correct) {
        QString category = m_currentObjects[0].category;
        ui->labelExcessFeedback->setText("Верно! Это " + category + " среди " + m_currentObjects[m_correctOption == 0 ? 1 : 0].category);
        addScore(add_point_Excess + m_combo * 5);
        updateCombo(true);
        m_gameActive = false;
        QTimer::singleShot(2000, this, [this]() {
            generateExcess();
            startGameTimer(2);
        });
    } else {
        ui->labelExcessFeedback->setText("Неверно! Попробуй еще");
        deductScore(ded_point_Excess);
        updateCombo(false);
    }
}
void MainWindow::on_btnOpt3_clicked()
{
    if (!m_gameActive) return;

    bool correct = (m_correctOption == 2);

    if (correct) {
        QString category = m_currentObjects[0].category;
        ui->labelExcessFeedback->setText("Верно! Это " + category + " среди " + m_currentObjects[m_correctOption == 0 ? 1 : 0].category);
        addScore(add_point_Excess + m_combo * 5);
        updateCombo(true);
        m_gameActive = false;
        QTimer::singleShot(2000, this, [this]() {
            generateExcess();
            startGameTimer(2);
        });
    } else {
        ui->labelExcessFeedback->setText("Неверно! Попробуй еще");
        deductScore(ded_point_Excess);
        updateCombo(false);
    }
}
void MainWindow::on_btnOpt4_clicked()
{
    if (!m_gameActive) return;

    bool correct = (m_correctOption == 3);

    if (correct) {
        QString category = m_currentObjects[0].category;
        ui->labelExcessFeedback->setText("Верно! Это " + category + " среди " + m_currentObjects[m_correctOption == 0 ? 1 : 0].category);
        addScore(add_point_Excess + m_combo * 5);
        updateCombo(true);
        m_gameActive = false;
        QTimer::singleShot(2000, this, [this]() {
            generateExcess();
            startGameTimer(2);
        });
    } else {
        ui->labelExcessFeedback->setText("Неверно! Попробуй еще");
        deductScore(ded_point_Excess);
        updateCombo(false);
    }
}

void MainWindow::generateCompare()
{
    QString operators[] = {"+", "-", "×", "÷"};

    // левое выражение
    int opLeft = QRandomGenerator::global()->bounded(4);
    int a, b, leftResult;
    QString leftStr;

    switch(opLeft) {
    case 0: // Сложение
        a = QRandomGenerator::global()->bounded(5, 50);
        b = QRandomGenerator::global()->bounded(5, 50);
        leftResult = a + b;
        leftStr = QString::number(a) + " + " + QString::number(b);
        break;
    case 1: // Вычитание
        a = QRandomGenerator::global()->bounded(20, 100);
        b = QRandomGenerator::global()->bounded(5, a);
        leftResult = a - b;
        leftStr = QString::number(a) + " - " + QString::number(b);
        break;
    case 2: // Умножение
        a = QRandomGenerator::global()->bounded(2, 12);
        b = QRandomGenerator::global()->bounded(2, 12);
        leftResult = a * b;
        leftStr = QString::number(a) + " × " + QString::number(b);
        break;
    case 3: // Деление
        b = QRandomGenerator::global()->bounded(2, 10);
        leftResult = QRandomGenerator::global()->bounded(2, 10);
        a = b * leftResult;
        leftStr = QString::number(a) + " ÷ " + QString::number(b);
        break;
    }

    // правое выражение
    int opRight = QRandomGenerator::global()->bounded(4);
    int c, d, rightResult;
    QString rightStr;

    switch(opRight) {
    case 0: // Сложение
        c = QRandomGenerator::global()->bounded(5, 50);
        d = QRandomGenerator::global()->bounded(5, 50);
        rightResult = c + d;
        rightStr = QString::number(c) + " + " + QString::number(d);
        break;
    case 1: // Вычитание
        c = QRandomGenerator::global()->bounded(20, 100);
        d = QRandomGenerator::global()->bounded(5, c);
        rightResult = c - d;
        rightStr = QString::number(c) + " - " + QString::number(d);
        break;
    case 2: // Умножение
        c = QRandomGenerator::global()->bounded(2, 12);
        d = QRandomGenerator::global()->bounded(2, 12);
        rightResult = c * d;
        rightStr = QString::number(c) + " × " + QString::number(d);
        break;
    case 3: // Деление
        d = QRandomGenerator::global()->bounded(2, 10);
        rightResult = QRandomGenerator::global()->bounded(2, 10);
        c = d * rightResult;
        rightStr = QString::number(c) + " ÷ " + QString::number(d);
        break;
    }

    if (m_difficulty >= 3) {
        leftResult = leftResult;
        rightResult = rightResult;
    }

    ui->labelCompareLeft->setText(leftStr + " = ?");
    ui->labelCompareRight->setText(rightStr + " = ?");

    if (leftResult > rightResult) {
        m_correctCompare = '>';
    } else if (leftResult < rightResult) {
        m_correctCompare = '<';
    } else {
        m_correctCompare = '=';
    }

    // Сохраняем результаты для отображения при ошибке
    m_correctAnswer = leftResult;
    m_correctOption = rightResult;

    ui->labelCompareFeedback->clear();
}

void MainWindow::on_btnGreater_clicked()
{
    if (!m_gameActive) return;

    bool correct = (m_correctCompare == '>');

    if (correct) {
        ui->labelCompareFeedback->setText("Верно! +" + QString::number(10 + m_combo*5));
        addScore(add_point_Compare + m_combo*5);
        updateCombo(true);
        m_gameActive = false;
        QTimer::singleShot(1500, this, [this]() {
            if (ui->stackedWidget->currentIndex() == 3) {
                generateCompare();
                startGameTimer(3);
            }
        });
    } else {
        QString correctSign;
        if (m_correctCompare == '>') correctSign = ">";
        else if (m_correctCompare == '<') correctSign = "<";
        else correctSign = "=";

        ui->labelCompareFeedback->setText("Неверно! Правильно: " + correctSign);
        deductScore(ded_point_Compare);
        updateCombo(false);
        QTimer::singleShot(1500, this, [this]() {
            if (ui->stackedWidget->currentIndex() == 3) {
                generateCompare();
                startGameTimer(3);
            }
        });
    }
}

void MainWindow::on_btnLess_clicked()
{
    if (!m_gameActive) return;

    bool correct = (m_correctCompare == '<');

    if (correct) {
        ui->labelCompareFeedback->setText("Верно! +" + QString::number(10 + m_combo*5));
        addScore(add_point_Compare + m_combo*5);
        updateCombo(true);
        m_gameActive = false;
        QTimer::singleShot(1500, this, [this]() {
            if (ui->stackedWidget->currentIndex() == 3) {
                generateCompare();
                startGameTimer(3);
            }
        });
    } else {
        QString correctSign;
        if (m_correctCompare == '>') correctSign = ">";
        else if (m_correctCompare == '<') correctSign = "<";
        else correctSign = "=";

        ui->labelCompareFeedback->setText("Неверно! Правильно: " + correctSign);
        deductScore(ded_point_Compare);
        updateCombo(false);
        QTimer::singleShot(1500, this, [this]() {
            if (ui->stackedWidget->currentIndex() == 3) {
                generateCompare();
                startGameTimer(3);
            }
        });
    }
}

void MainWindow::on_btnEqual_clicked()
{
    if (!m_gameActive) return;

    bool correct = (m_correctCompare == '=');

    if (correct) {
        ui->labelCompareFeedback->setText("Верно! +" + QString::number(10 + m_combo*5));
        addScore(add_point_Compare + m_combo*5);
        updateCombo(true);
        m_gameActive = false;
        QTimer::singleShot(1500, this, [this]() {
            if (ui->stackedWidget->currentIndex() == 3) {
                generateCompare();
                startGameTimer(3);
            }
        });
    } else {
        QString correctSign;
        if (m_correctCompare == '>') correctSign = ">";
        else if (m_correctCompare == '<') correctSign = "<";
        else correctSign = "=";

        ui->labelCompareFeedback->setText("Неверно! Правильно: " + correctSign);
        deductScore(ded_point_Compare);
        updateCombo(false);
        QTimer::singleShot(1500, this, [this]() {
            if (ui->stackedWidget->currentIndex() == 3) {
                generateCompare();
                startGameTimer(3);
            }
        });
    }
}

void MainWindow::generateStatement()
{
    int num = QRandomGenerator::global()->bounded(2, 50);
    QString statements[4] = {
        QString::number(num) + " — простое число",
        QString::number(num) + " делится на 3",
        QString::number(num) + " > " + QString::number(num * 2),
        QString::number(num) + " — четное число"
    };

    int idx = QRandomGenerator::global()->bounded(4);
    ui->labelStatementText->setText(statements[idx]);

    // Проверка истинности
    bool isPrime = true;
    for (int i = 2; i * i <= num; i++) if (num % i == 0) { isPrime = false; break; }

    switch(idx) {
    case 0: m_correctStatement = isPrime; break;
    case 1: m_correctStatement = (num % 3 == 0); break;
    case 2: m_correctStatement = (num > num * 2); break;
    case 3: m_correctStatement = (num % 2 == 0); break;
    }

    ui->labelStatementFeedback->clear();
}

void MainWindow::on_btnTrue_clicked() {
    if (!m_gameActive) return;
    bool correct = (m_correctStatement == true);

    if (correct) {
        ui->labelStatementFeedback->setText("Верно! +" + QString::number(add_point_Statement + m_combo*5));
        addScore(add_point_Statement + m_combo*5);
        updateCombo(true);
        m_gameActive = false;
        QTimer::singleShot(1500, this, [this]() {
            generateStatement();
            startGameTimer(4);
        });
    } else {
        ui->labelStatementFeedback->setText("Неверно! -5");
        deductScore(5);
        updateCombo(false);

        // Новое задание после ошибки
        QTimer::singleShot(1000, this, [this]() {
            generateStatement();
            m_statementTimer->reset();
            ui->labelStatementTimer->setText("Время: " + QString::number(m_timeLimit) + "с");
        });
    }
}
void MainWindow::on_btnFalse_clicked() {
    if (!m_gameActive) return;
    bool correct = (m_correctStatement == false);

    if (correct) {
        ui->labelStatementFeedback->setText("Верно! +" + QString::number(add_point_Statement + m_combo*5));
        addScore(add_point_Statement + m_combo*5);
        updateCombo(true);
        m_gameActive = false;
        QTimer::singleShot(1500, this, [this]() {
            generateStatement();
            startGameTimer(4);
        });
    } else {
        ui->labelStatementFeedback->setText("Неверно! -5");
        deductScore(5);
        updateCombo(false);

        // Новое задание после ошибки
        QTimer::singleShot(1000, this, [this]() {
            generateStatement();
            m_statementTimer->reset();
            ui->labelStatementTimer->setText("Время: " + QString::number(m_timeLimit) + "с");
        });
    }
}

void MainWindow::generateMemory()
{
    QString symbols[] = {"А", "У", "В", "Ч", "Я", "Б", "Х", "Щ", "Й", "Р"};
    m_correctSymbols.clear();

    int len = 3 + m_difficulty;
    for (int i = 0; i < len; i++) {
        m_correctSymbols += symbols[QRandomGenerator::global()->bounded(10)];
    }

    ui->labelMemorySequence->setText("Запомни: " + m_correctSymbols);

    QTimer::singleShot(3000, this, [this]() {
        ui->labelMemorySequence->setText("Введи последовательность:");
        ui->inputMemoryAnswer->setFocus();
    });

    ui->inputMemoryAnswer->clear();
    ui->labelMemoryFeedback->clear();
}

void MainWindow::on_btnMemorySubmit_clicked()
{
    if (!m_gameActive) return;

    QString answer = ui->inputMemoryAnswer->text().trimmed();
    bool correct = (answer == m_correctSymbols);

    if (correct) {
        ui->labelMemoryFeedback->setText("Верно! +" + QString::number(15 + m_combo*5));
        addScore(add_point_Memory + m_combo*5);
        updateCombo(true);
        m_gameActive = false;
        QTimer::singleShot(1500, this, [this]() {
            if (ui->stackedWidget->currentIndex() == 5) {
                generateMemory();
                startGameTimer(5);
            }
        });
    } else {
        ui->labelMemoryFeedback->setText("Неверно! Было: " + m_correctSymbols);
        deductScore(ded_point_Memory);
        updateCombo(false);
        QTimer::singleShot(1500, this, [this]() {
            if (ui->stackedWidget->currentIndex() == 5) {
                generateMemory();
                startGameTimer(5);
            }
        });
    }
}

// Таймер

void MainWindow::startGameTimer(int gameType)
{
    m_gameActive = true;
    m_timeLimit = 30 - (m_difficulty * 2);
    if (m_timeLimit < 10) m_timeLimit = 10;

    Timer *timer = nullptr;
    QLabel *label = nullptr;

    switch(gameType) {
    case 1: timer = m_seqTimer; label = ui->labelSeqTimer; break;
    case 2: timer = m_excessTimer; label = ui->labelExcessTimer; break;
    case 3: timer = m_compareTimer; label = ui->labelCompareTimer; break;
    case 4: timer = m_statementTimer; label = ui->labelStatementTimer; break;
    case 5: timer = m_memoryTimer; label = ui->labelMemoryTimer; break;
    }

    if (timer) timer->reset();
    if (label) label->setText("Время: " + QString::number(m_timeLimit) + "с");

    QThread *monitor = QThread::create([this, gameType, timer, label]() {
        while (m_gameActive && m_currentGame == gameType) {
            int elapsed = timer->get();
            int left = m_timeLimit - elapsed;

            QMetaObject::invokeMethod(this, [this, label, left, gameType]() {
                if (m_gameActive && m_currentGame == gameType) {
                    label->setText("Время: " + QString::number(left) + "с");
                    if (left <= 0) {
                        m_gameActive = false;
                        label->setText("⏰ Время вышло!");
                        updateCombo(false);
                        QTimer::singleShot(2000, this, [this, gameType]() {
                            if (m_currentGame == gameType) {
                                switch(gameType) {
                                case 1: generateSequence(); break;
                                case 2: generateExcess(); break;
                                case 3: generateCompare(); break;
                                case 4: generateStatement(); break;
                                case 5: generateMemory(); break;
                                }
                                startGameTimer(gameType);
                            }
                        });
                    }
                }
            });
            QThread::msleep(100);
        }
    });
    connect(monitor, &QThread::finished, monitor, &QObject::deleteLater);
    monitor->start();
}

void MainWindow::stopAllTimers()
{
    m_gameActive = false;
    m_currentGame = 0;
}

// Система очков и статистика

void MainWindow::addScore(int points)
{
    m_score += points;
    updateAllStatsLabels();
    adjustDifficulty();
}

void MainWindow::deductScore(int points)
{
    m_score -= points;
    if (m_score < 0) m_score = 0;
    updateAllStatsLabels();
    adjustDifficulty();
}

void MainWindow::updateCombo(bool correct)
{
    if (correct) {
        m_combo++;
    } else {
        m_combo = 0;
    }
    updateAllStatsLabels();
}

void MainWindow::adjustDifficulty()
{
    // Сложность растет каждые 50 очков
    int newDifficulty = 1 + (m_score / 50);
    if (newDifficulty > 5) newDifficulty = 5;
    m_difficulty = newDifficulty;
    updateAllStatsLabels();
}

void MainWindow::updateAllStatsLabels()
{
    // Обновляем лейблы на главной странице
    ui->label_score->setText("🏆 Очки: " + QString::number(m_score));
    ui->label_combo->setText("🔥 Комбо: x" + QString::number(m_combo));

    // Отображение сложности
    QString diffText;
    switch(m_difficulty) {
    case 1: diffText = "🟢 Легко"; break;
    case 2: diffText = "🟡 Средне"; break;
    case 3: diffText = "🟠 Сложно"; break;
    case 4: diffText = "🔴 Очень сложно"; break;
    default: diffText = "💀 Эксперт"; break;
    }
    ui->label_difficulty->setText("📊 " + diffText);
    if (m_score > bestScore) {
        bestScore = m_score;
        ui->labelBestScore->setText("Рекорд: " + QString::number(bestScore));
    }
}