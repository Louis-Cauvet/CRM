/**
 * @file listetotalitetaches.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#ifndef LISTETOTALITETACHES_H
#define LISTETOTALITETACHES_H

#include "gestionbdd.h"

#include <QWidget>

/**
 * @interface ListeTotaliteTaches
 * @brief Interface concernant la fenêtre listant les tâches de tous les contacts
 */
namespace Ui {
class ListeTotaliteTaches;
}

/**
 * @class ListeTotaliteTaches
 * @brief Classe définissant les attributs et méthodes liées à la fenêtre listant les tâches de tous les contacts
 */
class ListeTotaliteTaches : public QWidget
{
    Q_OBJECT

private:
    /* Attributs */
    Ui::ListeTotaliteTaches *ui;
    gestionBDD *gbdd;

public:
    /* Constructeur */
    explicit ListeTotaliteTaches(QWidget *parent = nullptr);

    /* Destructeur */
    ~ListeTotaliteTaches();

    /* Fonctions de classe */
    void afficherListeTaches(QSqlQuery &);
    /* surcharge de la fonction close() de la fenêtre */
    bool close();

private slots:
    /* Fonctions slots */
    void actualiserFenetre();
    void changerDateDebut(const QDate &);
    void changerDateFin(const QDate &);
    void supprimerTache();
    void exporterJson();
    void retour();

signals:
    /* Signaux */
    void fermerFenetre();
    void actualiserApresSuppTache();

};

#endif // LISTETOTALITETACHES_H
