/**
 * @file ajoutcontact.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#ifndef AJOUTCONTACT_H
#define AJOUTCONTACT_H

#include "gestionbdd.h"

#include <QDialog>

/**
 * @interface ajoutContact
 * @brief Interface concernant la fenêtre de l'ajout de contact
 */
namespace Ui {
class AjoutContact;
}

/**
 * @class ajoutContact
 * @brief Classe définissant les attributs et méthodes liées à la fenêtre d'ajout de contact
 */
class AjoutContact : public QDialog
{
    Q_OBJECT

private:
    /* Attributs */
    Ui::AjoutContact *ui;
    gestionBDD *gbdd;

public:
    /* Constructeur */
    explicit AjoutContact(QWidget *parent = nullptr);

    /* Destructeur */
    ~AjoutContact();

    /* Fonctions de classe */
    void initialisationFenetre();
    bool VerifsChampsContact();
    /* surcharge de la fonction close() de la fenêtre */
    bool close();

private slots:
    /* slots */
    void ajouterPhotoContact();
    void validerNouveauContact();
    void retour();

signals:
    /* signaux */
    void fermerFenetre();
};

#endif // AJOUTCONTACT_H
