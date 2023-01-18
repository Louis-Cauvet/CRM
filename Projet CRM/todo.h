/**
 * @file todo.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date septembre/octobre 2022
 **/

#ifndef TODO_H
#define TODO_H

#include "date.h"

#include<string>

/**
 * @class ToDo
 * @brief Classe définissant les attributs et méthodes des tâches pour chaque contact géré par l'application
 */
class toDo
{
private:
    /* Attributs */
    std::string contenuTD;
    date::year_month_day dateTD;

public:
    /* Constructeurs */
    toDo(const std::string &);
    toDo(const std::string &, const date::year_month_day &);


    /* Destructeur */
    ~toDo();


    /* Accesseurs */
    std::string getContenuTD() const;
    date::year_month_day getDateTD() const;


    /* Mutateurs */
    void setContenuTD(const std::string &);
    void setDateTD(const date::year_month_day &);


    /* Opérateur */
    /**
     * @brief operateur ==
     * @see Opérateur permettant de vérifer si une tâche est bien celle que l'on cherche (utilisé notamment dans la fonction supprimerToDo() de la classe Contact)
     * @param td correspond à la tâche à vérifier
     * @return un booléen qui vaut "True" si la tâche est bien celle que l'on cherche, "False" sinon
     * @warning corps de l'operateur défini directement içi pour réduire le nombre de fonctions implémentées dans le .cpp
     */
    bool operator ==(const toDo &td){return (contenuTD == td.getContenuTD() && dateTD == td.getDateTD());}


    /* Fonction amie */
    friend std::ostream& operator <<(std::ostream&, const toDo &);

    /* Fonction de classe */
    std::string conversionDateReaToString();
};

#endif // TODO_H
