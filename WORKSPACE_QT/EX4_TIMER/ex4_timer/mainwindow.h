#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QLCDNumber>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resetCount();
    void updateCount();

public slots:
     void onTimer_Tick();
     void onButton_Start();
     void onButton_Reset();

private:
    Ui::MainWindow *ui;

    QPushButton *m_bout_start;
    QPushButton *m_bout_stop;
    QPushButton *m_bout_reset;

    QGridLayout *m_layout;

    QLCDNumber *m_lcd;

    QTimer *timer_chrono;

    int     countTimer;
};

#endif // MAINWINDOW_H
