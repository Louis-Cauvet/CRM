/**
 * @file detailinteraction.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#ifndef DETAILINTERACTION_H
#define DETAILINTERACTION_H

#include "gestionbdd.h"

#include <QDialog>
#include <QSqlQuery>

/**
 * @interface detailInteraction
 * @brief Interface concernant la fenêtre de consultation d'interaction
 */
namespace Ui {
class DetailInteraction;
}

/**
 * @class detailInteraction
 * @brief Classe définissant les attributs et méthodes liées à la fenêtre de consultation d'interaction
 */
class DetailInteraction : public QDialog
{
    Q_OBJECT

private:
    /* attributs */
    Ui::DetailInteraction *ui;
    gestionBDD *gbdd;
    QString identifiantInteraction;

public:
    /* constructeur */
    explicit DetailInteraction(QWidget *parent = nullptr, QString idInteraction="");

    /* destructeur */
    ~DetailInteraction();

    /*fonction de classe */
    void afficherDonneesInteraction(QSqlQuery &);
    /* surcharge de la fonction close() de la fenêtre */
    bool close();

private slots:
    /* Fonctions slots */
    void validerModifications();
    void recupererTaches(int &);
    void retour();

signals :
    /* Signaux */
    void fermerFenetre();
    void traiterInteraction(int &);
};

#endif // DETAILINTERACTION_H
