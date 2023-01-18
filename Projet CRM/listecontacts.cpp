/**
 * @file listecontacts.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date septembre/octobre 2022
 */

#include "listecontacts.h"
#include <iostream>

using namespace date;
using namespace std::chrono;


/**
 * @brief le constructeur de la classe
 * @brief initialise le nombre total de contacts de la liste à 0 lors de la création
 */
listeContacts::listeContacts()
{
    this->setNbTotal(0);

}

/**
 * @brief le destructeur de la classe
 * @brief Si la liste de contact est supprimée, son contenu est nettoyé de ses données pour gagner de la place en mémoire
 */
listeContacts::~listeContacts(){
    listeC.clear();
}


std::list<contact> listeContacts::getListeC() const {return listeC;}
unsigned int listeContacts::getNbTotal() const {return nbTotal;}
date::year_month_day listeContacts::getDateDerniereSuppression() const {return dateDerniereSuppression;}


void listeContacts::setNbTotal(const unsigned int &nb){
    nbTotal = nb;
}
void listeContacts::setDateDerniereSuppression(const date::year_month_day &d){
    dateDerniereSuppression = d;
}


/**
 * @brief Fonction amie permettant de comparer les noms et prénoms de 2 contacts, afin de déterminer lequel est le premier par ordre alphabétique
 * @see utilisée comme paramètre dans la fonction trierCParOrdreAlphabetique de cette classe
 * @param c1 correspond au premier contact dont on souhaite comparer le nom
 * @param c2 correspond au deuxième contact dont on souhaite comparer le nom
 * @return un booléen avec la valeur "True" si le premier contact est situé avant dans l'ordre alphabétique, "False" si c'est le deuxième
 * @warning si les noms sont identiques, on compare alors les prénoms
 * @warning si les noms et les prénoms sont identiques, le premier contact est considéré être le premier par ordre alphabétique
 */
bool compare_Nom(const contact &c1, const contact &c2){
    std::string nom1 = c1.getNomC();
    std::string nom2 = c2.getNomC();
    std::transform(nom1.begin(), nom1.end(), nom1.begin(), ::toupper);
    std::transform(nom2.begin(), nom2.end(), nom2.begin(), ::toupper);

    if(nom1 < nom2){
        return true;
    }
    else if (nom1 > nom2){
        return false;
    }
    else {
        if(nom1 < nom2){
             return true;
        }
        else if (nom1 > nom2){
             return false;
        }
        else {
             return true;
        }
    }
}


/**
 * @brief Fonction amie permettant de comparer les dates de création de 2 contacts, afin de déterminer laquelle est la plus récente et laquelle est la plus ancienne
 * @see utilisée comme paramètre dans la fonction trierCParDateCreation() de cette classe
 * @param c1 correspond au premier contact dont on souhaite comparer la date de création
 * @param c2 correspond au deuxième contact dont on souhaite comparer la date de création
 * @return un booléen avec la valeur "True" si le premier contact possède la date la plus ancienne, "False" si c'est le deuxième
 * @warning si les dates sont identiques, la date du premier contact est considérée la plus ancienne (partie "else" de la condition)
 */
bool compare_Date(const contact &c1, const contact &c2){
    if(c1.getDateCreationC() < c2.getDateCreationC()){
        return true;
    }
    else if (c1.getNomC() > c2.getNomC()){
        return false;
    }
    else {
        return true;
    }
}


/**
 * @brief Fonction permettant d'ajouter un nouveau contact dans la liste
 * @param c correspond au nouveau contact à ajouter
 * @warning si le contact est ajouté, on ajoute 1 au nombre total de contacts de la liste
 */
void listeContacts::ajouterContact(const contact &c){
    listeC.push_back(c);
    this->nbTotal = nbTotal+1;
}


/**
 * @brief Fonction permettant de supprimer un contact de la liste
 * @param co correspond au contact à supprimer
 * @var dateDuJour permet de récuperer la date actuelle
 * @warning la date de dernière suppression d'un contact est mise à jour avec la date du jour
 * @warning si le contact est supprimé, on enlève 1 au nombre total de contacts de la liste
 */
void listeContacts::supprimerContact(const contact &co){
    listeC.remove(co);
    auto dateDuJour = date::year_month_day{floor<days>(system_clock::now())};
    this->setDateDerniereSuppression(dateDuJour);
    this->nbTotal = nbTotal-1;
}

/**
 * @brief Fonction permettant de modifier un contact dans la liste
 * @param i correspond à l'interaction à supprimer
 * @param ti correspond au nouveau titre de l'interaction
 * @param ci correspond au nouveau contenu de l'interaction
 * @warning on réalise une copie de l'interaction à modifier avant de la supprimer de la liste, puis on effectue les modifications sur la copie avant de la réajouter à la liste
 */

/**
 * @brief Fonction permettant de modifier un contact dans la liste
 * @param c correspond au contact dont on souhaite modifier les données
 * @param n correspond au nouveau nom du contact
 * @param p correspond au nouveau prénom du contact
 * @param e correspond à la nouvelle entreprise du contact
 * @param m correspond à la nouvelle adresse-mail du contact
 * @param t correspond au nouveau numéro de téléphone du contact
 * @param ph correspond à l'URL de la nouvelle photo
 * @warning on réalise une copie du contact à modifier avant de le supprimer de la liste, puis on effectue les modifications sur la copie avant de la réajouter à la liste
 */
void listeContacts::modifierContact(const contact &c , const std::string &n , const std::string &p , const std::string &e , const std::string &m , const std::string &t , const std::string &ph){
    for (contact cont :listeC){
        if(cont == c){
            contact copie = cont;
            this->supprimerContact(cont);
            copie.modifierDonneesContact(n, p, e, m, t, ph);
            this->ajouterContact(copie);
        }
    }
}


/**
 * @brief Fonction permettant de trier la liste de contacts par ordre alphabétique
 * @see utilise la fonction amie compare_Nom comme paramètre de la fonction sort() pour établir la comparaison entre 2 contacts
 */
void listeContacts::trierCParOrdreAlphabetique(){
    listeC.sort(compare_Nom);
}


/**
 * @brief Fonction permettant de trier la liste de contacts par date de création
 * @see utilise la fonction amie compare_Date comme paramètre de la fonction sort() pour établir la comparaison entre 2 contacts
 */
void listeContacts::trierCParDateCreation(){
    listeC.sort(compare_Date);
}


/**
 * @brief Fonction permettant de chercher uniquement les contacts de la liste dont le nom commence de la même facon que la recherche passée en paramètres
 * @param recherche correspond à la chaine que l'on recherche dans un nom de contact
 * @return une liste de contacts contenant uniquement les contacts qui remplissent la condition
 * @warning si la recherche de l'utilisateur est vide, la fonction retourne tous les contacts de la liste
 * @warning on convertit la recherche et le nom de chaque contact afin de ne pas devoir respecter la casse lors de la comparaison
 */
std::list<contact> listeContacts::chercherCParNom(std::string &recherche){
    if(recherche == ""){
        return listeC;
    } else {
        std::list<contact> listeRechNom;
        std::transform(recherche.begin(), recherche.end(), recherche.begin(), ::toupper);

        for (contact c : listeC){
            std::string nomC = c.getNomC();

            std::transform(nomC.begin(), nomC.end(), nomC.begin(), ::toupper);

            if(!nomC.find(recherche)){
                 listeRechNom.push_back(c);
            }
        }

        return listeRechNom;
    }
}


/**
 * @brief Fonction permettant de chercher uniquement les contacts de la liste dont l'entreprise commence de la même facon que la recherche passée en paramètres
 * @param recherche correspond à la chaine que l'on recherche dans l'entreprise d'un contact
 * @return une liste de contacts contenant uniquement les contacts qui remplissent la condition
 * @warning si la recherche de l'utilisateur est vide, la fonction retourne tous les contacts de la liste
 * @warning on convertit la recherche et l'entreprise de chaque contact afin de ne pas devoir respecter la casse lors de la comparaison
 */
std::list<contact> listeContacts::chercherCParEntreprise(std::string &recherche){
    if(recherche == ""){
        return listeC;
    } else {
        std::list<contact> listeRechEntreprise;

        std::transform(recherche.begin(), recherche.end(), recherche.begin(), ::toupper);

        for (contact c : listeC){
            std::string entrepriseC = c.getEntrepriseC();

            std::transform(entrepriseC.begin(), entrepriseC.end(), entrepriseC.begin(), ::toupper);
            if(!entrepriseC.find(recherche)){
                 listeRechEntreprise.push_back(c);
            }
        }

        return listeRechEntreprise;
    }
}


/**
 * @brief Fonction permettant de chercher uniquement les contacts de la liste dont la date de création est comprise entre les 2 dates passées en paramètres
 * @param d1 correspond à la date la plus ancienne de l'intervalle
 * @param d2 correspond à la date la plus récente de l'intervalle
 * @return une liste de contacts contenant uniquement les contacts qui remplissent la condition
 */
std::list<contact> listeContacts::chercherCParIntervalleDate(const date::year_month_day &d1, const date::year_month_day &d2){
    std::list<contact> listeRechIntervDates;

    for (contact c : listeC){
        if(c.getDateCreationC() >= d1){
            if(c.getDateCreationC() <= d2){
                listeRechIntervDates.push_back(c);
            }
        }
    }

    return listeRechIntervDates;
}
