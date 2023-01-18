/**
 * @file listetaches.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#ifndef LISTETACHES_H
#define LISTETACHES_H

#include "qsqlquery.h"
#include "gestionbdd.h"

#include <QDialog>

/**
 * @interface ListeTaches
 * @brief Interface concernant la fenêtre listant les tâches d'un contact
 */
namespace Ui {
class ListeTaches;
}

/**
 * @class ListeTaches
 * @brief Classe définissant les attributs et méthodes liées à la fenêtre listant les tâches d'un contact
 */
class ListeTaches : public QDialog
{
    Q_OBJECT

private:
    /* attributs */
    Ui::ListeTaches *ui;
    gestionBDD *gbdd;
    QString identifiantContact;

public:
    /* constructeur */
    explicit ListeTaches(QWidget *parent = nullptr, QString idContact="");

    /* destructeur */
    ~ListeTaches();

    /* Fonctions de classe */
    void afficherIdentiteContact(QSqlQuery &);
    void afficherListeTaches(QSqlQuery &);
    /* surcharge de la fonction close() de la fenêtre */
    bool close();

private slots:
    /* Fonctions slots */
    void actualiserFenetre();
    void changerDateDebut(const QDate &date);
    void changerDateFin(const QDate &date);
    void supprimerTache();
    void retour();

signals:
    /* Signaux */
    void fermerFenetre();
    void actualiserApresSuppTache();
};

#endif // LISTETACHES_H
