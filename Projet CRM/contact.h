/**
 * @file contact.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date septembre/octobre 2022
 **/

#ifndef CONTACT_H
#define CONTACT_H

#include "date.h"
#include "gestionlisteinteractions.h"

#include <list>
#include <iostream>

/**
 * @class Contact
 * @brief Classe définissant les attributs et méthodes des contacts gérés par l'application
 */
class contact
{

private:
    /* Attributs */
    std::string nomC;
    std::string prenomC;
    std::string entrepriseC;
    std::string mailC;
    std::string telC;
    std::string urlphotoC;
    date::year_month_day dateCreationC;
    date::year_month_day dateModifC;
    gestionListeInteractions listeInte;

public:
    /* Constructeur */
    contact(const std::string & , const std::string & , const std::string & , const std::string & , const std::string & , const std::string &);


    /* Destructeur */
    ~contact();


    /* Accesseurs */
    std::string getNomC() const;
    std::string getPrenomC() const;
    std::string getEntrepriseC() const;
    std::string getMailC() const;
    std::string getTelC() const;
    std::string getUrlPhotoC() const;
    date::year_month_day getDateCreationC() const;
    date::year_month_day getDateModifC() const;
    gestionListeInteractions getListeInte() const;


    /* Mutateurs */
    void setNomC(const std::string &);
    void setPrenomC(const std::string &);
    void setEntrepriseC(const std::string &);
    void setMailC(const std::string &);
    void setTelC(const std::string &);
    void setUrlPhotoC(const std::string &);
    void setDateCreationC(const date::year_month_day &);
    void setDateModifC(const date::year_month_day &);
    void setListeInte(const gestionListeInteractions &);


    /* Opérateur */
    /**
     * @brief operateur ==
     * @see Opérateur permettant de vérifer si un contact est bien celui que l'on cherche (utilisé notamment dans la fonction supprimerContact() de la classe listeContacts)
     * @param c correspond au contact à vérifier
     * @return un booléen qui vaut "True" si le contact est bien celui que l'on cherche, "False" sinon
     * @warning corps de l'operateur défini directement içi pour réduire le nombre de fonctions implémentées dans le .cpp
     */
    bool operator ==(const contact &c){return (nomC == c.getNomC() && prenomC == c.getPrenomC() && entrepriseC == c.getEntrepriseC() && mailC == c.getMailC() && telC == c.getTelC() && urlphotoC == c.getUrlPhotoC());}


    /* Fonctions amies */
    friend std::ostream& operator <<(std::ostream &, const contact &);


    /* Fonctions de classes */
    void modifierDonneesContact(const std::string & , const std::string & , const std::string & , const std::string & , const std::string & , const std::string &);
    std::string conversionDateCreaToString();
    std::string conversionDateModifToString();
};

#endif // CONTACT_H
