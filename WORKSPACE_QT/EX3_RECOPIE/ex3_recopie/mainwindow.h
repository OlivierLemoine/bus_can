#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>

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
    void btnClick();

private:
    Ui::MainWindow *ui;

    QPushButton *m_bouton;
    QVBoxLayout *m_layout;
    QLineEdit   *emetteur;
    QLineEdit   *recepteur;
};

#endif // MAINWINDOW_H
