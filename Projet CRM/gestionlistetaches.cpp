/**
 * @file contact.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date septembre/octobre 2022
 */

#include "gestionlistetaches.h"

/**
 * @brief le constructeur de la classe
 */
gestionListeTaches::gestionListeTaches()
{
}


/**
 * @brief le destructeur de la classe
 * @brief Si la liste de tâches est supprimée, son contenu est nettoyé de ses données pour gagner de la place en mémoire
 */
gestionListeTaches::~gestionListeTaches(){
    listeToDo.clear();
}


std::list<toDo> gestionListeTaches::getListeToDo() const {return listeToDo;}


/**
 * @brief Fonction amie permettant de comparer les dates de réalisation de 2 tâches, afin de déterminer laquelle est la plus récente et laquelle est la plus ancienne
 * @see utilisée comme paramètre dans la fonction trierTDParDate() de cette classe
 * @param td1 correspond à la première tâche dont on souhaite comparer la date de réalisation
 * @param i2 correspond à la deuxième tâche dont on souhaite comparer la date de réalisation
 * @return un booléen avec la valeur "True" si la première tâche possède la date la plus ancienne, "False" si c'est la deuxième
 * @warning si les dates sont identiques, la date de la première tâche est considérée la plus ancienne (partie "else" de la condition)
 */
bool compare_DateToDo(const toDo &td1, const toDo &td2){
    if(td1.getDateTD() < td2.getDateTD()){
        return true;
    }
    else if (td1.getDateTD() > td2.getDateTD()){
        return false;
    }
    else {
        return true;
    }
}


/**
 * @brief Fonction permettant d'ajouter une nouvelle tâche dans la liste
 * @param inte correspond à la nouvelle tâche à ajouter
 */
void gestionListeTaches::ajouterToDo(const toDo &td){
    listeToDo.push_back(td);
}


/**
 * @brief Fonction permettant de supprimer une tâche dans la liste
 * @param inte correspond à la tâche à supprimer de la liste
 */
void gestionListeTaches::supprimerToDo(const toDo &td){
    listeToDo.remove(td);
}


/**
 * @brief Fonction permettant de trier la liste des tâches par date de réalisation
 * @see utilise la fonction amie compare_DateTD comme paramètre de la fonction sort() pour établir la comparaison entre 2 tâches
 */
void gestionListeTaches::trierTDParDate(){
    listeToDo.sort(compare_DateToDo);
}


/**
 * @brief Fonction permettant de chercher uniquement les tâches de la liste dont la date de réalisation est comprise entre les 2 dates passées en paramètres
 * @param d1 correspond à la date la plus ancienne de l'intervalle
 * @param d2 correspond à la date la plus récente de l'intervalle
 * @return une liste de tâches contenant uniquement les tâches qui remplissent la condition
 */
std::list<toDo> gestionListeTaches::chercherTDParIntervalleDate(const date::year_month_day &d1, const date::year_month_day &d2){
    std::list<toDo> listeRechIntervDates;

    for (toDo td : listeToDo){
        if(td.getDateTD() >= d1){
            if(td.getDateTD() <= d2){
                listeRechIntervDates.push_back(td);
            }
        }
    }

    return listeRechIntervDates;
}
