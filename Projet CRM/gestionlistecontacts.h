/**
 * @file listecontacts.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date septembre/octobre 2022
 **/

#ifndef GESTIONLISTECONTACTS_H
#define GESTIONLISTECONTACTS_H

#include "date.h"
#include "contact.h"

#include <list>

/**
 * @class gestionListeContacts
 * @brief Classe définissant les attributs et méthodes permettant de gérer la liste de contacts de l'application
 */
class gestionListeContacts
{
private :
    /* Attributs */
    std::list<contact> listeC;
    unsigned int nbTotal;
    date::year_month_day dateDerniereSuppression;


public:
    /* Constructeur */
    gestionListeContacts();


    /* Destructeur */
    ~gestionListeContacts();


    /* Accesseurs */
    std::list<contact> getListeC() const;
    unsigned int getNbTotal() const;
    date::year_month_day getDateDerniereSuppression() const;


    /* Mutateurs */
    void setNbTotal(const unsigned int &);
    void setDateDerniereSuppression(const date::year_month_day &);


    /* Fonctions amies */
    friend bool compare_Nom(const contact &, const contact &);
    friend bool compare_Date(const contact &, const contact &);


    /* Fonctions de classe */
    void ajouterContact(const contact &);
    void supprimerContact(const contact &);
    void modifierContact(const contact & , const std::string & , const std::string & , const std::string & , const std::string & , const std::string & , const std::string &);
    void trierCParOrdreAlphabetique();
    void trierCParDateCreation();
    std::list<contact> chercherCParNom(std::string &);
    std::list<contact> chercherCParEntreprise(std::string &);
    std::list<contact> chercherCParIntervalleDate(const date::year_month_day &, const date::year_month_day &);
};

#endif // GESTIONLISTECONTACTS_H
