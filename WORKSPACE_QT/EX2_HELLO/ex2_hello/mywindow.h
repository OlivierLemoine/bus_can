#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QMessageBox>


class MyWindow : public QWidget // On hérite de QWidget (IMPORTANT)
{
    Q_OBJECT                    // Nécessaire pour créer un slot maison

    public:
    MyWindow();                // Constructeur (forcément pubic)

    public slots:               // Slots maison
    void Dialogue();

    private:
    QPushButton *m_bouton;      // Attribut (forcément privé)
                                // pointeur --> il faudra le construire dynamiquement (new)

};

#endif // MYWINDOW_H

