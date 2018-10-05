#include "mainwindow.h"
#include "ui_mainwindow.h"

QString nameport = "/dev/ttyACM0";

//=================================================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    QWidget *zoneCentrale = new QWidget;
    setCentralWidget(zoneCentrale);         // Définition d'une Fenêtre
                                               // Nécessaire avec mainwindow

    QLabel *label_to_send = new QLabel;
    label_to_send->setText("Message à Envoyer");
    QLabel *label_received = new QLabel;
    label_received->setText("Message reçu");


    but_send = new QPushButton("Send Message");  // NB : on ne précise pas le parent
    mes_to_send = new QLineEdit();
    mes_received = new QLineEdit();

    m_layout = new QVBoxLayout();

    m_layout->addWidget(label_to_send);
    m_layout->addWidget(mes_to_send);
    m_layout->addWidget(but_send);
        m_layout->addWidget(label_received);
    m_layout->addWidget(mes_received);

    zoneCentrale->setLayout(m_layout);

    openSerialPort();
    connect(serial,SIGNAL(readyRead()),this,SLOT(readData()));
    connect(but_send,SIGNAL(clicked()),this,SLOT(onButSendClicked()));

   // ui->setupUi(this);
}
//=================================================================================
MainWindow::~MainWindow()
{
    delete ui;
}
//=================================================================================
void MainWindow::openSerialPort()
{
    serial = new QSerialPort(this);
    serial->setPortName(nameport);
    serial->open(QIODevice::ReadWrite);

     if( serial->isOpen()==false)
     {
          serial->clearError();
          QMessageBox::critical(this, "Port error", "Port: "+nameport);
          QMessageBox::information(this, "Port error", "Vérifier nom du port \n Fermer tout programme utilisant la lisaison RS232 "+nameport);
      }
   else
     {
         QMessageBox::information(this, "Port open", " "+nameport);
          serial->setBaudRate(QSerialPort::Baud115200);
          serial->setStopBits(QSerialPort::OneStop);
          serial->setParity(QSerialPort::NoParity);
          serial->setDataBits(QSerialPort::Data8);
          serial->setFlowControl(QSerialPort::NoFlowControl);
     }
}

//====================================================

void MainWindow::onButSendClicked()
{
    QString message=mes_to_send->text();
    writeData(message.toUtf8()); // QString --> QByteArray
}
//====================================================

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
//====================================================
void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    mes_received->setText(data);
}
//====================================================
