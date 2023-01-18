/**
 * @file ajoutinteraction.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#ifndef AJOUTINTERACTION_H
#define AJOUTINTERACTION_H

#include "gestionbdd.h"

#include <QDialog>

/**
 * @interface ajoutInteraction
 * @brief Interface concernant la fenêtre de l'ajout d'interaction
 */
namespace Ui {
class ajoutInteraction;
}

/**
 * @class ajoutInteraction
 * @brief Classe définissant les attributs et méthodes liées à la fenêtre d'ajout d'interaction
 */
class ajoutInteraction : public QDialog
{
    Q_OBJECT

private:
    /* attributs */
    Ui::ajoutInteraction *ui;
    gestionBDD *gbdd;
    QString identifiantContact;

public:
    /* Constructeur */
    explicit ajoutInteraction(QWidget *parent = nullptr, QString idContact="");

    /* Destructeur */
    ~ajoutInteraction();

    /* surcharge de la fonction close() de la fenêtre */
    bool close();

private slots:
    /* slots */
    void validerAjout();
    void recupererTaches(int &);
    void retour();

signals:
    /* signaux */
    void fermerFenetre();
    void traiterInteraction(int &);
};

#endif // AJOUTINTERACTION_H
