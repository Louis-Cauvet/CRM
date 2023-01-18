/**
 * @file contact.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date septembre/octobre 2022
 */

#include "contact.h"

#include <string>
#include <vector>
#include <iostream>

/**
 * using nécessaires pour utiliser les types d'attributs date définies dans le date.h
 */
using namespace date;
using namespace std::chrono;


/**
 * @brief le constructeur de la classe
 * @brief les attributs passés en paramètres seront saisis par l'utilisateur lors de la création d'un contact
 * @param n correspond au nom du contact
 * @param p correspond au prénom du contact
 * @param e correspond à l'entreprise du contact
 * @param m correspond à l'adresse mail du contact
 * @param t correspond au numéro de téléphone du contact
 * @param ph correspond à l'URL de la photo du contact
 * @var dateDuJour permet de recupérer la date actuelle
 * @warning La date de création et la date de modification du contact sont instanciées avec la date du jour
 */
contact::contact(const std::string &n, const std::string &p, const std::string &e, const std::string &m, const std::string &t, const std::string &ph)
{
    this->setNomC(n);
    this->setPrenomC(p);
    this->setEntrepriseC(e);
    this->setMailC(m);
    this->setTelC(t);
    this->setUrlPhotoC(ph);
    auto dateDuJour = date::year_month_day{floor<days>(system_clock::now())};
    this->setDateCreationC(dateDuJour);
    this->setDateModifC(dateDuJour);
}


/**
 * @brief le destructeur de la classe
 */
contact::~contact(){
};


std::string contact::getNomC() const {return nomC;}
std::string contact::getPrenomC() const {return prenomC;}
std::string contact::getEntrepriseC() const {return entrepriseC;}
std::string contact::getMailC() const {return mailC;}
std::string contact::getTelC() const {return telC;}
std::string contact::getUrlPhotoC() const {return urlphotoC;}
date::year_month_day contact::getDateCreationC() const { return dateCreationC;}
date::year_month_day contact::getDateModifC() const {return dateModifC;}
gestionListeInteractions contact::getListeInte() const { return listeInte;}


void contact::setNomC(const std::string &n){
    nomC = n;
}
void contact::setPrenomC(const std::string &p){
    prenomC = p;
}
void contact::setEntrepriseC(const std::string &e){
    entrepriseC = e;
}
void contact::setMailC(const std::string &m){
    mailC = m;
}
void contact::setTelC(const std::string &t){
    telC = t;
}
void contact::setUrlPhotoC(const std::string &ph){
    urlphotoC = ph;
}
void contact::setDateCreationC(const date::year_month_day &dc){
    dateCreationC = dc;
}
void contact::setDateModifC(const date::year_month_day &dm){
    dateModifC = dm;
}
void contact::setListeInte(const gestionListeInteractions &newListeInte)
{
    listeInte = newListeInte;
}


/**
 * @brief Fonction amie permettant de prédéfinir l'affichage en console d'un contact
 * @see Utilisée essentiellement pour afficher les attributs des contacts lors des tests dans le main
 * @param os correspond à la sortie console contenant la chaine de caractères
 * @param c correspond au contact que l'on souhaite afficher
 * @return la chaîne de caractères affichée dans la console
 */
std::ostream& operator <<(std::ostream &os, const contact &c){
    os << c.getNomC() <<" | "<< c.getPrenomC() <<" | "<< c.getEntrepriseC() <<" | "<< c.getMailC() <<" | "<<  c.getTelC() <<" | "<< c.getUrlPhotoC() <<" | "<< c.getDateCreationC() <<" | "<< c.getDateModifC();
    return os;
}


/**
 * @brief Fonction amie permettant de comparer les dates de modification de 2 interactions, afin de déterminer laquelle est la plus récente et laquelle est la plus ancienne
 * @see utilisée comme paramètre dans la fonction trierInteParDate() de cette classe
 * @param i1 correspond à la première interaction dont on souhaite comparer la date de modification
 * @param i2 correspond à la deuxième interaction dont on souhaite comparer la date de modification
 * @return un booléen avec la valeur "True" si la première interraction possède la date la plus ancienne, "False" si c'est la deuxième
 * @warning si les dates sont identiques, la date de la première interraction est considérée la plus ancienne (partie "else" de la condition)
 */
bool compare_DateInte(const interraction &i1, const interraction &i2){
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
 * @brief Fonction amie permettant de comparer les dates de réalisation de 2 tâches, afin de déterminer laquelle est la plus récente et laquelle est la plus ancienne
 * @see utilisée comme paramètre dans la fonction trierTDParDate() de cette classe
 * @param td1 correspond à la première tâche dont on souhaite comparer la date de réalisation
 * @param i2 correspond à la deuxième tâche dont on souhaite comparer la date de réalisation
 * @return un booléen avec la valeur "True" si la première tâche possède la date la plus ancienne, "False" si c'est la deuxième
 * @warning si les dates sont identiques, la date de la première tâche est considérée la plus ancienne (partie "else" de la condition)
 */
bool compare_DateTD(const toDo &td1, const toDo &td2){
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
 * @brief Fonction permettant de remplacer les données d'un contact avec les nouvelles données passées en paramètres
 * @see appelée dans le cadre de la fonction modifierContact() de la classe ListeContacts
 * @param n correspond au nouveau nom du contact
 * @param p correspond au nouveau prénom du contact
 * @param e correspond à la nouvelle entreprise du contact
 * @param m correspond à la nouvelle adresse mail du contact
 * @param t correspond au nouveau numéro de téléphone du contact
 * @param ph correspond à l'URL de la nouvelle photo du contact
 * @var dateDuJour permet de récuperer la date actuelle
 * @warning la date de modification du contact est mise à jour avec la date du jour
 */
void contact::modifierDonneesContact(const std::string &n , const std::string &p , const std::string &e , const std::string &m , const std::string &t , const std::string &ph)
{
    this->setNomC(n);
    this->setPrenomC(p);
    this->setEntrepriseC(e);
    this->setMailC(m);
    this->setTelC(t);
    this->setUrlPhotoC(ph);
    auto dateDuJour = date::year_month_day{floor<days>(system_clock::now())};
    this->setDateModifC(dateDuJour);
}


/**
 * @brief Fonction permettant de convertir la date de création du contact en chaine de caractères
 * @var dateCrJour est une chaine de caractères contenant le jour de la date de création
 * @var dateCrMois est une chaine de caractères contenant le mois de la date de création
 * @var dateCrAnnee est une chaine de caractères contenant l'année de la date de création
 * @var dateCrString est une chaine de caractères obtenue en concaténant le jour, le mois et l'année
 * @warning on fait en sorte que la date de création soit toujours de la forme "dd/mm/yyyy" en rajoutant des 0 devant le jour ou le mois si nécéssaire
 * @return la chaine de caractères contenant la date de création du contact
 */
std::string contact::conversionDateCreaToString(){
    std::string dateCrJour = std::to_string(this->getDateCreationC().day().operator unsigned int());
    if(dateCrJour.size()==1){
        dateCrJour = "0"+dateCrJour;
    }
    std::string dateCrMois = std::to_string(this->getDateCreationC().month().operator unsigned int());
    if(dateCrMois.size()==1){
        dateCrMois = "0"+dateCrMois;
    }
    std::string dateCrAnnee = std::to_string(this->getDateCreationC().year().operator int());
    std::string dateCrString = dateCrJour + "/" + dateCrMois + "/" + dateCrAnnee;

    return dateCrString;
}


/**
 * @brief Fonction permettant de convertir la date de modification du contact en chaine de caractères
 * @var dateModJour est une chaine de caractères contenant le jour de la date de modification
 * @var dateModMois est une chaine de caractères contenant le mois de la date de modification
 * @var dateModAnnee est une chaine de caractères contenant l'année de la date de modification
 * @var dateModString est une chaine de caractères obtenue en concaténant le jour, le mois et l'année
 * @warning on fait en sorte que la date de modification soit toujours de la forme "dd/mm/yyyy" en rajoutant des 0 devant le jour ou le mois si nécéssaire
 * @return la chaine de caractères contenant la date de modification du contact
 */
std::string contact::conversionDateModifToString(){
    std::string dateModJour = std::to_string(this->getDateModifC().day().operator unsigned int());
    if(dateModJour.size()==1){
        dateModJour = "0"+dateModJour;
    }
    std::string dateModMois = std::to_string(this->getDateModifC().month().operator unsigned int());
    if(dateModMois.size()==1){
        dateModMois = "0"+dateModMois;
    }
    std::string dateModAnnee = std::to_string(this->getDateModifC().year().operator int());
    std::string dateModString = dateModJour + "/" + dateModMois + "/" + dateModAnnee;

    return dateModString;
}
