/**
 * @file contact.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date septembre/octobre 2022
 */

#include "gestionlisteinteractions.h"


#include <iostream>

/**
 * @brief le constructeur de la classe
 */
gestionListeInteractions::gestionListeInteractions()
{ 
}


/**
 * @brief le destructeur de la classe
 * @brief Si la liste d'interactions est supprimée, son contenu est nettoyé de ses données pour gagner de la place en mémoire
 */
gestionListeInteractions::~gestionListeInteractions(){
    listeInterractions.clear();
};


std::list<interraction> gestionListeInteractions::getListeInterractions() const {return listeInterractions;}


/**
 * @brief Fonction amie permettant de comparer les dates de modification de 2 interactions, afin de déterminer laquelle est la plus récente et laquelle est la plus ancienne
 * @see utilisée comme paramètre dans la fonction trierInteParDate() de cette classe
 * @param i1 correspond à la première interaction dont on souhaite comparer la date de modification
 * @param i2 correspond à la deuxième interaction dont on souhaite comparer la date de modification
 * @return un booléen avec la valeur "True" si la première interraction possède la date la plus ancienne, "False" si c'est la deuxième
 * @warning si les dates sont identiques, la date de la première interraction est considérée la plus ancienne (partie "else" de la condition)
 */
bool compare_DateInteractions(const interraction &i1, const interraction &i2){
    if(i1.getDateModifInte() < i2.getDateModifInte()){
        return true;
    }
    else if (i1.getDateModifInte() > i2.getDateModifInte()){
        return false;
    }
    else {
        return true;
    }
}



/**
 * @brief Fonction permettant d'ajouter une nouvelle interaction dans la liste
 * @param inte correspond à la nouvelle interaction à ajouter
 * @see la fonction traiterInterraction() est appelée dans cette fonction, afin de parcourir la nouvelle interaction et d'y extraire possiblement des tâches à ajouter dans la liste de tâches du contact
 */
void gestionListeInteractions::ajouterInterraction(interraction &inte){
    listeInterractions.push_back(inte);
    gestionListeTaches gt = inte.traiterInterraction();
    inte.setListeTaches(gt);
}


/**
 * @brief Fonction permettant de supprimer une interaction dans la liste
 * @param inte correspond à l'interaction à supprimer de la liste
 */
void gestionListeInteractions::supprimerInterraction(const interraction &inte){
    listeInterractions.remove(inte);
}


/**
 * @brief Fonction permettant de modifier une interaction dans la liste
 * @param i correspond à l'interaction dont on souhaite modifier les données
 * @param ti correspond au nouveau titre de l'interaction
 * @param ci correspond au nouveau contenu de l'interaction
 * @warning on réalise une copie de l'interaction à modifier avant de la supprimer de la liste, puis on effectue les modifications sur la copie avant de la réajouter à la liste
 */
void gestionListeInteractions::modifierInterraction(const interraction &i, const std::string &ti, const std::string &ci){
    for (interraction inte : listeInterractions){
        if(inte == i){
            interraction copieI = inte;
            this->supprimerInterraction(inte);

            copieI.modifierDonneesInterraction(ti, ci);
            this->ajouterInterraction(copieI);
        }
    }
}


/**
 * @brief Fonction permettant de trier la liste des interactions par date de modification
 * @see utilise la fonction amie compare_DateInte comme paramètre de la fonction sort() pour établir la comparaison entre 2 interactions
 */
void gestionListeInteractions::trierInteParDate(){
     listeInterractions.sort(compare_DateInteractions);
}


/**
 * @brief Fonction permettant de chercher uniquement les interactions de la liste dont la date de modification est comprise entre les 2 dates passées en paramètres
 * @param d1 correspond à la date la plus ancienne de l'intervalle
 * @param d2 correspond à la date la plus récente de l'intervalle
 * @return une liste d'interaction contenant uniquement les interactions qui remplissent la condition
 */
std::list<interraction> gestionListeInteractions::chercherInteParIntervalleDate(const date::year_month_day &d1, const date::year_month_day &d2){
    std::list<interraction> listeRechIntervDates;

    for (interraction i : listeInterractions){
        if(i.getDateModifInte() >= d1){
            if(i.getDateModifInte() <= d2){
                listeRechIntervDates.push_back(i);
            }
        }
    }

    return listeRechIntervDates;
}
