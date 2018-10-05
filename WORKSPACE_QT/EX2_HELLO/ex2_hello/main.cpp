#include "mywindow.h"
#include <QApplication>

int main(int argc, char *argv[])    // c'est ici que ça commence
{
    QApplication a(argc, argv);     // constructeur de l'application
    MyWindow w;                     // Constructeur de la fenetre
    w.show();                       // Affichage de la fenetre

    return a.exec();                // Exécution de l'application
}
