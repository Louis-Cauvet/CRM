/**
 * @file listecontacts.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date septembre/octobre 2022
 **/

#ifndef LISTECONTACTS_H
#define LISTECONTACTS_H

#include "contact.h"

#include <list>

/**
 * @class ListeContacts
 * @brief Classe définissant les attributs et méthodes de la liste de contacts gérée par l'application
 */
class listeContacts
{
private:
    /* Attributs */
    std::list<contact> listeC;
    unsigned int nbTotal;
    date::year_month_day dateDerniereSuppression;

public:
    /* Constructeur */
    listeContacts();


    /* Destructeur */
    ~listeContacts();


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

#endif // LISTECONTACTS_H
