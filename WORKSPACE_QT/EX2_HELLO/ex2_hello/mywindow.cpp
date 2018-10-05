#include "mywindow.h"

MyWindow::MyWindow() : QWidget()  // spécification du constructeur
{
    setFixedSize(300,400); // setFixedSize appartient à classe MaFenetre (héritage de QWidget
                           // Largeur / Hauteur

    /* Construction du bouton */
    m_bouton = new QPushButton("open", this); // this = pointeur vers le widget parent, pointeur vers 'moi'
    m_bouton->move(100, 100);   // RePositionnement Absolu

    /* Connexions Signal - Slot */
    QObject::connect(m_bouton, SIGNAL(clicked()), this, SLOT(Dialogue())); // this = SLOT de MyWindow (SLOT MAISON)
}
//==========================================================================================
void MyWindow::Dialogue()
{
    QMessageBox::information(this, "Titre de la fenêtre", "Hello");
    // REM : information / warning / critical / question
    // int reponse =  QMessageBox::question(this, "Titre de la fenêtre", "vraiment ?", QMessageBox::Yes | QMessageBox::No);
    // (OU LOGIQUE)

}
//==========================================================================================
