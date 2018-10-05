/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *Start;
    QRadioButton *OpenPort;
    QLCDNumber *lcdanalog1;
    QSlider *valueanalogwrite1;
    QLCDNumber *lcdanalog2;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label;
    QLabel *label_4;
    QPushButton *Stop;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->setEnabled(true);
        MainWindow->resize(399, 195);
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        Start = new QPushButton(centralWidget);
        Start->setObjectName(QStringLiteral("Start"));
        Start->setGeometry(QRect(20, 50, 100, 30));
        QFont font;
        font.setPointSize(12);
        Start->setFont(font);
        OpenPort = new QRadioButton(centralWidget);
        OpenPort->setObjectName(QStringLiteral("OpenPort"));
        OpenPort->setEnabled(true);
        OpenPort->setGeometry(QRect(20, 20, 97, 25));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush1);
        QBrush brush2(QColor(127, 127, 127, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush2);
        QBrush brush3(QColor(170, 170, 170, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush3);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush1);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
        QBrush brush4(QColor(255, 255, 220, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush4);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        OpenPort->setPalette(palette);
        OpenPort->setFont(font);
        OpenPort->setStyleSheet(QStringLiteral(""));
        lcdanalog1 = new QLCDNumber(centralWidget);
        lcdanalog1->setObjectName(QStringLiteral("lcdanalog1"));
        lcdanalog1->setGeometry(QRect(20, 140, 100, 40));
        lcdanalog1->setFrameShape(QFrame::WinPanel);
        lcdanalog1->setSmallDecimalPoint(false);
        lcdanalog1->setDigitCount(3);
        lcdanalog1->setSegmentStyle(QLCDNumber::Flat);
        valueanalogwrite1 = new QSlider(centralWidget);
        valueanalogwrite1->setObjectName(QStringLiteral("valueanalogwrite1"));
        valueanalogwrite1->setGeometry(QRect(240, 10, 30, 181));
        valueanalogwrite1->setMouseTracking(true);
        valueanalogwrite1->setLayoutDirection(Qt::LeftToRight);
        valueanalogwrite1->setAutoFillBackground(false);
        valueanalogwrite1->setMaximum(255);
        valueanalogwrite1->setTracking(true);
        valueanalogwrite1->setOrientation(Qt::Vertical);
        valueanalogwrite1->setInvertedAppearance(false);
        valueanalogwrite1->setInvertedControls(false);
        valueanalogwrite1->setTickPosition(QSlider::TicksBelow);
        valueanalogwrite1->setTickInterval(16);
        lcdanalog2 = new QLCDNumber(centralWidget);
        lcdanalog2->setObjectName(QStringLiteral("lcdanalog2"));
        lcdanalog2->setGeometry(QRect(20, 90, 100, 40));
        QPalette palette1;
        QBrush brush5(QColor(0, 170, 0, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush5);
        lcdanalog2->setPalette(palette1);
        lcdanalog2->setFrameShape(QFrame::NoFrame);
        lcdanalog2->setFrameShadow(QFrame::Raised);
        lcdanalog2->setSmallDecimalPoint(false);
        lcdanalog2->setDigitCount(3);
        lcdanalog2->setSegmentStyle(QLCDNumber::Flat);
        lcdanalog2->setProperty("value", QVariant(0));
        lcdanalog2->setProperty("intValue", QVariant(0));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(280, 170, 51, 20));
        label_2->setFont(font);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(280, 0, 51, 20));
        label_3->setFont(font);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(120, 90, 30, 40));
        QFont font1;
        font1.setPointSize(28);
        label->setFont(font1);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(120, 140, 30, 40));
        label_4->setFont(font1);
        Stop = new QPushButton(centralWidget);
        Stop->setObjectName(QStringLiteral("Stop"));
        Stop->setGeometry(QRect(130, 50, 91, 31));
        Stop->setFont(font);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Demo 001", 0));
        Start->setText(QApplication::translate("MainWindow", "START", 0));
        OpenPort->setText(QApplication::translate("MainWindow", "ON PORT", 0));
        label_2->setText(QApplication::translate("MainWindow", "0.00 V", 0));
        label_3->setText(QApplication::translate("MainWindow", "5.00 V", 0));
        label->setText(QApplication::translate("MainWindow", "V", 0));
        label_4->setText(QApplication::translate("MainWindow", "V", 0));
        Stop->setText(QApplication::translate("MainWindow", "STOP", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
