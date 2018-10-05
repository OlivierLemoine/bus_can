#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)//,
    //ui(new Ui::MainWindow)
{
    QWidget *zoneCentrale = new QWidget;
    setCentralWidget(zoneCentrale);         // Définition d'une Fenêtre
                                            // Nécessaire avec mainwindow

    m_bouton = new QPushButton("recopie"); 	// NB : on ne précise pas le parent
    emetteur = new QLineEdit();
    recepteur = new QLineEdit();

    m_layout = new QVBoxLayout();           // REM : existe aussi QHBoxLayout / QGridLayout / QFormLayout

    m_layout->addWidget(emetteur);
    m_layout->addWidget(m_bouton);
    m_layout->addWidget(recepteur);

    zoneCentrale->setLayout(m_layout);

    connect( m_bouton, SIGNAL(clicked()), this, SLOT(btnClick()));

//*******************************************************
//              AFFICHAGE BARRE DE MENUS
//*******************************************************
    QMenu *menuFichier = menuBar()->addMenu("&Fichier");
    QAction *actionQuitter = new QAction("&Quitter", this);
    menuFichier->addAction(actionQuitter);
    connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));

    //ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnClick()
{
    recepteur->setText(emetteur->text());   // Action de recopie
    emetteur->clear();
}
