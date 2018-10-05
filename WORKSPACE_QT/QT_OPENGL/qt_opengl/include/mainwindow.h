#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QtWidgets/QMainWindow>
#include <qgridlayout.h>
#include <objectgl.h>
#include <QMenuBar>
#include <QMessageBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor and destructor
    MainWindow(QWidget *parent = 0, int w=600, int h=400);
    ~MainWindow();

    bool                    connect();

protected slots:
    // Redraw the scene
    void                    onTimer_UpdateDisplay();

protected:

    // Overload of the resize event
    void                    resizeEvent(QResizeEvent *);

private:

    // Layout of the window
    QGridLayout             *gridLayout;
    QWidget                 *gridLayoutWidget;

    // Central widget (where the openGL window is drawn)
    QWidget                 *centralWidget;

    // OpenGL object
    ObjectOpenGL            *Object_GL;
};

#endif // MAINWINDOW_H
