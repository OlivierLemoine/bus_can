#include "mainwindow.h"
#include "ui_mainwindow.h"

#define DEFAULT_NODE "/dev/pcanusb32"

//===============================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QWidget *zoneCentrale = new QWidget;
    setCentralWidget(zoneCentrale);

    but_send = new QPushButton("Send Message");
    m_layout = new QGridLayout();

    QLabel *label_to_send = new QLabel;
    label_to_send->setText("Message to Send");
    QLabel *label_received = new QLabel;
    label_received->setText("Message Received");

    send_box_0 = new QLineEdit();
    send_box_1 = new QLineEdit();
    send_box_2 = new QLineEdit();
    send_box_3 = new QLineEdit();
    send_box_4 = new QLineEdit();
    send_box_5 = new QLineEdit();
    send_box_6 = new QLineEdit();
    send_box_7 = new QLineEdit();

    receive_box_0 = new QLineEdit();
    receive_box_1 = new QLineEdit();
    receive_box_2 = new QLineEdit();
    receive_box_3 = new QLineEdit();
    receive_box_4 = new QLineEdit();
    receive_box_5 = new QLineEdit();
    receive_box_6 = new QLineEdit();
    receive_box_7 = new QLineEdit();

    m_layout->addWidget(label_to_send,0,1);

    m_layout->addWidget(but_send,1,0);

    m_layout->addWidget(send_box_7,1,1);
    m_layout->addWidget(send_box_6,1,2);
    m_layout->addWidget(send_box_5,1,3);
    m_layout->addWidget(send_box_4,1,4);
    m_layout->addWidget(send_box_3,1,5);
    m_layout->addWidget(send_box_2,1,6);
    m_layout->addWidget(send_box_1,1,7);
    m_layout->addWidget(send_box_0,1,8);

    m_layout->addWidget(label_received,2,1);

    m_layout->addWidget(receive_box_7,3,1);
    m_layout->addWidget(receive_box_6,3,2);
    m_layout->addWidget(receive_box_5,3,3);
    m_layout->addWidget(receive_box_4,3,4);
    m_layout->addWidget(receive_box_3,3,5);
    m_layout->addWidget(receive_box_2,3,6);
    m_layout->addWidget(receive_box_1,3,7);
    m_layout->addWidget(receive_box_0,3,8);

    zoneCentrale->setLayout(m_layout);

    openCANPort();

    timer_tick = new QTimer();
    connect( timer_tick, SIGNAL(timeout()), this, SLOT(onTimer_Tick()));


    connect(but_send,SIGNAL(clicked()),this,SLOT(onButSendClicked()));

     timer_tick -> start(100); // in ms
}
//===============================================================

MainWindow::~MainWindow()
{
    delete ui;
}

//===============================================================
void MainWindow::openCANPort()
{
    const char *szDevNode = DEFAULT_NODE;

    h=LINUX_CAN_Open(szDevNode, O_RDWR);
    if (!h)
        {
            printf("can't open %s\n", szDevNode);
        }
    CAN_Init(h, CAN_BAUD_500K,  CAN_INIT_TYPE_ST);         // BTR0BTR1	bitrate code in hex (default=0x1c (500 kbps))
    CAN_Status(h); // Clear Status

}
//===============================================================
void MainWindow::onButSendClicked()
{
    sendCANMessage();
}
//===============================================================
void MainWindow::sendCANMessage()
{
    TPCANMsg msgBuff;

    msgBuff.ID      =   0x1;
    msgBuff.MSGTYPE =   MSGTYPE_STANDARD;
    msgBuff.LEN     =   8;

    msgBuff.DATA[0] =   send_box_0->text().toInt();
    msgBuff.DATA[1] =   send_box_1->text().toInt();
    msgBuff.DATA[2] =   send_box_2->text().toInt();
    msgBuff.DATA[3] =   send_box_3->text().toInt();
    msgBuff.DATA[4] =   send_box_4->text().toInt();
    msgBuff.DATA[5] =   send_box_5->text().toInt();
    msgBuff.DATA[6] =   send_box_6->text().toInt();
    msgBuff.DATA[7] =   send_box_7->text().toInt();


 LINUX_CAN_Write_Timeout(h, &msgBuff,0);

}
//===============================================================
void MainWindow::receiveCANMessage()
{


    LINUX_CAN_Read_Timeout(h, &pMsgBuff,1); // Timeout=1ms --> Non Bloquant

    receive_box_0->setText(QString::number((uint)pMsgBuff.Msg.DATA[0]));
    receive_box_1->setText(QString::number((uint)pMsgBuff.Msg.DATA[1]));
    receive_box_2->setText(QString::number((uint)pMsgBuff.Msg.DATA[2]));
    receive_box_3->setText(QString::number((uint)pMsgBuff.Msg.DATA[3]));
    receive_box_4->setText(QString::number((uint)pMsgBuff.Msg.DATA[4]));
    receive_box_5->setText(QString::number((uint)pMsgBuff.Msg.DATA[5]));
    receive_box_6->setText(QString::number((uint)pMsgBuff.Msg.DATA[6]));
    receive_box_7->setText(QString::number((uint)pMsgBuff.Msg.DATA[7]));

}
//===============================================================

void MainWindow::onTimer_Tick()
{
receiveCANMessage();
}


