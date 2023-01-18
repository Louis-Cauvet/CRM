/**
 * @file premierefenetre.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#ifndef PREMIEREFENETRE_H
#define PREMIEREFENETRE_H

#include "ajoutcontact.h"
#include "fichecontact.h"
#include "listetotalitetaches.h"
#include "gestionbdd.h"

namespace Ui {
class PremiereFenetre;
}

class PremiereFenetre : public QDialog
{
    Q_OBJECT

private:
    /* Instances d'autres classes */
    Ui::PremiereFenetre *ui;
    AjoutContact *aj;
    FicheContact *fc;
    ListeTotaliteTaches *ltt;
    gestionBDD *gbdd;

public:
    /* Constructeur */
    explicit PremiereFenetre(QWidget *parent = nullptr);

    /* Destructeur */
    ~PremiereFenetre();

    /* Fonctions de classe */
    void recupererCheminBDD();
    void afficherLignesTableau(QSqlQuery &);

private slots:
    /* Fonctions slots */
    void actualiserFenetre();
    void modifierTypeDeRecherche(const QString &);
    void afficherChampRecherche();
    void afficherIntervDates();
    void modifierChampRecherche(const QString &);
    void modifierDateDebut(const QDate &);
    void modifierDateFin(const QDate &);
    void changerModeTri(const QString &);
    void exporterJson();
    void ouvrirAjoutContact();
    void ouvrirFicheContact();
    void ouvrirListeTotaliteTaches();

signals:
    /* Signaux */
    void signalAfficherChampRecherche();
    void signalAfficherIntervDates();
};

#endif // PREMIEREFENETRE_H
