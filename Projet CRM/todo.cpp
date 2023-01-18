/**
 * @file todo.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date septembre/octobre 2022
 */

#include "todo.h"

/**
 * using nécessaires pour utiliser les types d'attributs date définies dans le date.h
 */
using namespace date;
using namespace std::chrono;


/**
 * @brief un constructeur de la classe, utilisé dans le cas où la date de réalisation de la tâche n'est pas précisée dans l'interaction
 * @param c correspond au contenu de la tâche
 * @var dateDuJour permet de récuperer la date actuelle
 * @warning La date de réalisation de la tâche est la date du jour
 */
toDo::toDo(const std::string &c)
{
    this->setContenuTD(c);
    auto dateDuJour = date::year_month_day{floor<days>(system_clock::now())};
    this->setDateTD(dateDuJour);
}


/**
 * @brief un constructeur de la classe, utilisé dans le cas où la date de réalisation de la tâche est précisée dans l'interaction
 * @param c correspond au contenu de la tâche
 * @param d correspond à la date de réalisation de la tâche
 * @warning La date de réalisation de la tâche est la date définie dans l'interaction
 */
toDo::toDo(const std::string &c, const date::year_month_day &d){
    this->setContenuTD(c);
    this->setDateTD(d);
}


/**
 * @brief le destructeur de la classe
 */
toDo::~toDo(){
}


std::string toDo::getContenuTD() const {return contenuTD;}
date::year_month_day toDo::getDateTD() const {return dateTD;}


void toDo::setContenuTD(const std::string &c){
    contenuTD = c;
}
void toDo::setDateTD(const date::year_month_day &d){
    dateTD = d;
}


/**
 * @brief Fonction amie permettant de prédéfinir l'affichage en console d'une tâche
 * @see Utilisée essentiellement pour afficher les attributs des tâches lors des tests dans le main
 * @param os correspond à la sortie console contenant la chaine de caractères
 * @param inte correspond à la tâche que l'on souhaite afficher
 * @return la chaîne de caractères affichée dans la console
 */
std::ostream& operator <<(std::ostream &os, const toDo &td){
    os<<td.getContenuTD()<<" | "<<td.getDateTD();
    return os;
}


/**
 * @brief Fonction permettant de convertir la date de réalisation de la tâche en chaine de caractères
 * @var dateModJour est une chaine de caractères contenant le jour de la date de modification
 * @var dateModMois est une chaine de caractères contenant le mois de la date de modification
 * @var dateModAnnee est une chaine de caractères contenant l'année de la date de modification
 * @var dateModString est une chaine de caractères obtenue en concaténant le jour, le mois et l'année
 * @warning on fait en sorte que la date de réalisation soit toujours de la forme "dd/mm/yyyy" en rajoutant des 0 devant le jour ou le mois si nécéssaire
 * @return la chaine de caractères contenant la date de réalisation de la tâche
 */
std::string toDo::conversionDateReaToString(){
    std::string dateReaJour = std::to_string(this->getDateTD().day().operator unsigned int());
    if(dateReaJour.size()==1){
        dateReaJour = "0"+dateReaJour;
    }
    std::string dateReaMois = std::to_string(this->getDateTD().month().operator unsigned int());
    if(dateReaMois.size()==1){
        dateReaMois = "0"+dateReaMois;
    }
    std::string dateReaAnnee = std::to_string(this->getDateTD().year().operator int());
    std::string dateReaString = dateReaJour + "/" + dateReaMois + "/" + dateReaAnnee;

    return dateReaString;
}
