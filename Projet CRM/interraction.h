/**
 * @file interraction.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date septembre/octobre 2022
 **/

#ifndef INTERRACTION_H
#define INTERRACTION_H

#include "date.h"
#include "gestionlistetaches.h"

#include <list>

/**
 * @class Interraction
 * @brief Classe définissant les attributs et méthodes des interractions pour chaque contact géré par l'application
 */
class interraction
{
private:
    /* Attributs */
    std::string titreInte;
    std::string contenuInte;
    date::year_month_day dateModifInte;
    gestionListeTaches listeTaches;


public:
    /* Constructeur */
    interraction(const std::string &, const std::string &);


    /* Destructeur */
    ~interraction();


    /* Accesseurs */
    std::string getTitreInte() const;
    std::string getContenuInte() const;
    date::year_month_day getDateModifInte() const;
    gestionListeTaches getListeTaches() const;


    /* Mutateurs */
    void setTitreInte(const std::string &);
    void setContenuInte(const std::string &);
    void setDateModifInte(const date::year_month_day &);
    void setListeTaches(const gestionListeTaches &);


    /* Opérateur */
    /**
     * @brief operateur ==
     * @see Opérateur permettant de vérifer si une interaction est bien celle que l'on cherche (utilisé notamment dans la fonction supprimerInterraction() de la classe Contact)
     * @param i correspond à l'interaction à vérifier
     * @return un booléen qui vaut "True" si l'interaction est bien celle que l'on cherche, "False" sinon
     * @warning corps de l'operateur défini directement içi pour réduire le nombre de fonctions implémentées dans le .cpp
     */
    bool operator ==(const interraction &i){return (titreInte == i.getTitreInte() && contenuInte == i.getContenuInte() && dateModifInte == i.getDateModifInte());}


    /* Fonctions amies */
    friend std::ostream& operator <<(std::ostream&, const interraction &);


    /* Fonction de classe */
    void modifierDonneesInterraction(const std::string &, const std::string &);
    std::string conversionDateModifToString();
    gestionListeTaches traiterInterraction();

};

#endif // INTERRACTION_H
