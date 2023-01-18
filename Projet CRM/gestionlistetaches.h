/**
 * @file contact.h
 * @authors Louis CAUVET - Félix DACLIN
 * @date septembre/octobre 2022
 **/

#ifndef GESTIONLISTETACHES_H
#define GESTIONLISTETACHES_H

#include "todo.h"

#include <list>

/**
 * @class gestionListeTaches
 * @brief Classe définissant  les attributs et méthodes permettant de gérer la liste de tâches d'un contact
 */
class gestionListeTaches
{
private:
    /* Attributs */
    std::list<toDo> listeToDo;

public:
    /* Constructeur */
    gestionListeTaches();


    /* Destructeur */
    ~gestionListeTaches();


    /* Accesseurs */
    std::list<toDo> getListeToDo() const;


    /* Fonctions amies */
    friend bool compare_DateToDo(const toDo &, const toDo &);


    /* Fonctions de classes */
    void ajouterToDo(const toDo &);
    void supprimerToDo(const toDo &);
    void trierTDParDate();
    std::list<toDo> chercherTDParIntervalleDate(const date::year_month_day &, const date::year_month_day &);
};

#endif // GESTIONLISTETACHES_H
