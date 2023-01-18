/**
 * @file gestionListeContacts.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date septembre/octobre 2022
 **/

#ifndef GESTIONLISTEINTERACTIONS_H
#define GESTIONLISTEINTERACTIONS_H

#include "interraction.h"

#include <list>

/**
 * @class gestionListeInteractions
 * @brief Classe définissant  les attributs et méthodes permettant de gérer la liste d'interactions d'un contact
 */
class gestionListeInteractions
{
private:
    /* Attributs */
    std::list<interraction> listeInterractions;

public:
    /* Constructeur */
    gestionListeInteractions();


    /* Destructeur */
    ~gestionListeInteractions();


    /* Accesseurs */
    std::list<interraction> getListeInterractions() const;


    /* Fonctions amies */
    friend bool compare_DateInteractions(const interraction &, const interraction &);


    /* Fonctions de classe */
    void ajouterInterraction(interraction &);
    void supprimerInterraction(const interraction &);
    void modifierInterraction(const interraction &, const std::string &, const std::string &);
    void trierInteParDate();
    std::list<interraction> chercherInteParIntervalleDate(const date::year_month_day &, const date::year_month_day &);

};

#endif // GESTIONLISTEINTERACTIONS_H
