/**
 * @file fichecontact.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#ifndef FICHECONTACT_H
#define FICHECONTACT_H

#include "listetaches.h"
#include "detailinteraction.h"
#include "ajoutinteraction.h"
#include "gestionbdd.h"

#include <QMainWindow>
#include <QSqlQuery>

/**
 * @interface ficheContact
 * @brief Interface concernant la fenêtre de consultation de contact
 */
QT_BEGIN_NAMESPACE
namespace Ui { class FicheContact; }
QT_END_NAMESPACE

/**
 * @class ficheContact
 * @brief Classe définissant les attributs et méthodes liées à la fenêtre de consultation de contact
 */
class FicheContact : public QMainWindow
{
    Q_OBJECT

private:
    /* Attributs */
    Ui::FicheContact *ui;
    ListeTaches *lt;
    DetailInteraction *di;
    ajoutInteraction *ai;
    gestionBDD *gbdd;
    QString identifiantContact;

public:
    /* Constructeur */
    FicheContact(QWidget *parent = nullptr, QString idContact="");

    /* Destructeur */
    ~FicheContact();

    /*Fonctions de classe */
    void afficherDonneesContact(QSqlQuery &);
    void afficherListeInteractions(QSqlQuery &);
    bool verifsChampsContact();
    /* surcharge de la fonction close() de la fenêtre */
    bool close();

private slots:
    /* Fonctions slots */
    void actualiserFenetre();
    void changerPhotoContact();
    void validerModifications();
    void supprimerContact();
    void modifierDateDebut(const QDate &);
    void modifierDateFin(const QDate &);
    void ouvrirListeTaches();
    void ouvrirAjoutInteraction();
    void ouvrirDetailInteraction();
    void supprimerInteraction();
    void exporterJson();
    void boutonRetour();

signals:
    /* Signaux */
    void fermerFenetre();
    void actualiserApresSuppInte();

};
#endif // FICHECONTACT_H
