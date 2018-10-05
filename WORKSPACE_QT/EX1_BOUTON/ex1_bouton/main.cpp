#include <QApplication>     // Classes
#include <QPushButton>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);   // Création de l'objet app

    /* Création de l'objet fenetre (appartient à classe Qwidget) */
    QWidget myWindow;
    myWindow.setFixedSize(700,150);      // Personnalisation de la fenetre

    /*Création et personnalisation de l'objet bouton */
    QPushButton bouton("bouton", &myWindow); // Parent = fenetre
    bouton.setText("texte bouton modifié"); // accesseur *set* de l'attribut texte du bouton
    bouton.setFont(QFont("Courier",12));
    bouton.setToolTip("un bouton");         // Info bulle
    //bouton.move(50,50);                   // Repositionner le bouton ( Positionnement absolu)
    bouton.setGeometry(50,50,500,50);       // Abs / Ord / largeur / hauteur

    myWindow.show(); // Méthode pour afficher la fenetre
    return app.exec();
}
