#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:


  void openSerialPort();
  void onButSendClicked();
  void writeData(const QByteArray &);
  void readData();

private:

  QSerialPort *serial;
  Ui::MainWindow *ui;

  QVBoxLayout *m_layout;
  QPushButton *but_send;
  QLineEdit   *mes_to_send;
  QLineEdit   *mes_received;




};

#endif // MAINWINDOW_H
