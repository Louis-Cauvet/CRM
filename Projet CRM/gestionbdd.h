/**
 * @file gestionbdd.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#ifndef GESTIONBDD_H
#define GESTIONBDD_H

#include <QtSql/QSqlDatabase>
#include <QSqlQueryModel>

/**
 * @class gestionBDD
 * @brief Classe définissant les attributs et méthodes liées à la fenêtre de gestion de la base de données
 */
class gestionBDD
{

private:
    /* Attributs */
    QSqlDatabase db;

public:
    /* Constructeur */
    gestionBDD(QString &);

    /* Destructeur */
    ~gestionBDD();

    /* Fonctions de classe */
    void connexionBDD(QString &);
    void fermerConnexion();

    /* Fonctions liées à la fenêtre PremiereFenetre */
    QString retournerNbTotalContacts();
    QString retournerDerniereSuppressionContact();
    QSqlQuery remplirTableauContactsParDate();
    QSqlQuery remplirTableauContactsParAlphabetique();
    QSqlQuery rechercherContactSelonNom(const QString &, bool &);
    QSqlQuery rechercherContactSelonEntreprise(const QString &, bool &);

    /* Fonction liée à la fenêtre AjoutContact */
    void ajouterNouveauContact(const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &);

    /* Fonctions liées à la fenêtre FicheContact */
    QSqlQuery retournerDonneesContact(const QString &);
    void modifierContact(const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &);
    void supprimerContact(const QString &);
    void changerDerniereSuppressionContact(const QString &);
    QSqlQuery remplirTableauInteParDate(const QString &);
    void supprimerInteraction(const QString &);

    /* Fonctions liées à la fenêtre AjoutInteraction */
    void ajouterNouvelleInteraction(const QString &,const QString &,const QString &,const int &);
    int recupererIdentifiantInteraction();
    void ajouterNouvelleTache(const QString &,const QString &,const int &);

    /* Fonctions liées à la fenêtre DetailInteraction */
    QSqlQuery retournerDonneesInteraction(const QString &);
    void modifierInteraction(const QString &,const QString &,const QString &);

    /* Fonctions liées aux fenêtres ListeTaches et ListeTotaliteTaches */
    QSqlQuery retournerIdentiteContact(const QString &);
    QSqlQuery remplirTableauTachesContact(const QString &);
    QSqlQuery remplirTableauTotaliteTaches();
    void supprimerTache(const QString &);
};

#endif // GESTIONBDD_H
