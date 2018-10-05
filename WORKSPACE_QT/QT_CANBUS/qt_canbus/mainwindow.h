#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCore>
#include <QDebug>

#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>

#include <libpcan.h>        // /usr/include/libpcan.h
#include <fcntl.h>    // O_RDWR
#include <signal.h>
#include <unistd.h>   // exit

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void openCANPort();
    void sendCANMessage();
    void receiveCANMessage();

public slots:

    void onButSendClicked();
         void onTimer_Tick();



private:

    HANDLE h;
    TPCANRdMsg pMsgBuff;

    Ui::MainWindow  *ui;
     QGridLayout     *m_layout;
     QPushButton     *but_send;
     QLineEdit       *receive_box_0,*receive_box_1,*receive_box_2,*receive_box_3 ;
     QLineEdit       *receive_box_4,*receive_box_5,*receive_box_6,*receive_box_7 ;
     QLineEdit       *send_box_0, *send_box_1,*send_box_2,*send_box_3;
     QLineEdit       *send_box_4, *send_box_5,*send_box_6,*send_box_7;

    QTimer *timer_tick;
};

#endif // MAINWINDOW_H
