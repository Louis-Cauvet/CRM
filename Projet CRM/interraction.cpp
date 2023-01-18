/**
 * @file interraction.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date septembre/octobre 2022
 */

#include "interraction.h"
#include "todo.h"
#include <vector>

/**
 * using nécessaires pour utiliser les types d'attributs date définies dans le date.h
 */
using namespace date;
using namespace std::chrono;


/**
 * @brief le constructeur de la classe
 * @param t correspond au titre de l'interaction
 * @param c correspond au contenu de l'interaction
 * @var dateDuJour permet de récupérer la date actuelle
 * @warning La date de modification de l'interaction est instanciée avec la date du jour
 */
interraction::interraction(const std::string &t,const std::string &c){
    this->setTitreInte(t);
    this->setContenuInte(c);
    auto dateDuJour = date::year_month_day{floor<days>(system_clock::now())};
    this->setDateModifInte(dateDuJour);
}


/**
 * @brief le destructeur de la classe
 */
interraction:: ~interraction(){
};


std::string interraction::getTitreInte() const {return titreInte;}
std::string interraction::getContenuInte() const {return contenuInte;}
date::year_month_day interraction::getDateModifInte()const {return dateModifInte;}
gestionListeTaches interraction::getListeTaches() const { return listeTaches;}


void interraction::setTitreInte(const std::string &t){
    titreInte = t;
}
void interraction::setContenuInte(const std::string &c){
    contenuInte = c;
}
void interraction::setDateModifInte(const date::year_month_day &d){
    dateModifInte = d;
}
void interraction::setListeTaches(const gestionListeTaches &newListeTaches)
{
    listeTaches = newListeTaches;
}


/**
 * @brief Fonction amie permettant de prédéfinir l'affichage en console d'une interaction
 * @see Utilisée essentiellement pour afficher les attributs des interactions lors des tests dans le main
 * @param os correspond à la sortie console contenant la chaine de caractères
 * @param inte correspond à l'interaction que l'on souhaite afficher
 * @return la chaîne de caractères affichée dans la console
 */
std::ostream& operator <<(std::ostream &os, const interraction &inte){
    os<<inte.getTitreInte()<<" | "<<inte.getContenuInte()<<" | "<<inte.getDateModifInte();
    return os;
}


/**
 * @brief Fonction permettant de remplacer les données d'une interaction avec les nouvelles données passées en paramètres
 * @see appelée dans le cadre de la fonction modifierInterraction() de la classe Contact
 * @param ti correspond au nouveau titre de l'interaction
 * @param ci correspond au nouveau contenu de l'interaction
 * @var dateDuJour permet de récuperer la date actuelle
 * @warning la date de modification de l'interaction est mise à jour avec la date du jour
 */
void interraction::modifierDonneesInterraction(const std::string &ti,const std::string &ci){

    this->setTitreInte(ti);
    this->setContenuInte(ci);
    auto dateDuJour = date::year_month_day{floor<days>(system_clock::now())};
    this->setDateModifInte(dateDuJour);
}


/**
 * @brief Fonction permettant de convertir la date de modification de l'interaction en chaine de caractères
 * @var dateModJour est une chaine de caractères contenant le jour de la date de modification
 * @var dateModMois est une chaine de caractères contenant le mois de la date de modification
 * @var dateModAnnee est une chaine de caractères contenant l'année de la date de modification
 * @var dateModString est une chaine de caractères obtenue en concaténant le jour, le mois et l'année
 * @warning on fait en sorte que la date de modification soit toujours de la forme "dd/mm/yyyy" en rajoutant des 0 devant le jour ou le mois si nécéssaire
 * @return la chaine de caractères contenant la date de modification de l'interaction
 */
std::string interraction::conversionDateModifToString(){
    std::string dateModJour = std::to_string(this->getDateModifInte().day().operator unsigned int());
    if(dateModJour.size()==1){
        dateModJour = "0"+dateModJour;
    }
    std::string dateModMois = std::to_string(this->getDateModifInte().month().operator unsigned int());
    if(dateModMois.size()==1){
        dateModMois = "0"+dateModMois;
    }
    std::string dateModAnnee = std::to_string(this->getDateModifInte().year().operator int());
    std::string dateModString = dateModJour + "/" + dateModMois + "/" + dateModAnnee;

    return dateModString;
}


/**
 * @brief Fonction permettant de parcourir une interaction qui vient d'être créée pour un contact afin d'y extraire des tâches à ajouter dans la liste de tâches du contact
 * @param inte correpond à l'intéraction qui vient d'être créée
 * @see cette fonction est appelée lors de la fonction ajouterInteraction de cette classe
 * @warning pour que cette fonction fonctionne, l'utilisateur doit séparer les différentes lignes de la nouvelle interaction avec des ";"
 * @warning le premier "while" sert à decomposer l'interaction en plusieurs lignes (séparées par ";")
 * @warning pour chaque ligne de l'interaction contenant une mention todo et une mention date, le second "while" sépare chaque mot de l'interaction
 * @var estLaDate est un booléen retournant "True" si le mot de l'interaction est la date mentionnée dans l'interaction qui deviendra la date de réalisation de la tâche, "False" sinon
 * @warning si le mot en cours de traitement est la date mentionnée dans l'interaction, on la décompose en jour, mois et année puis on l'appelle dans le constructeur à 2 paramètres de la tâche avant de l'ajouter à la liste
 * @warning si l'interaction en cours de traitement possède une mention todo mais pas de mention date, on appelle le constructeur à  paramètre de la tâche avant de l'ajouter à la liste
 */
gestionListeTaches interraction::traiterInterraction(){
    gestionListeTaches listeT;

    std::vector<std::string> morceauxInte;
    std::stringstream ss(this->getContenuInte());
    std::string extraitInte;
    char delimiteur = ';';
    while(std::getline(ss,extraitInte,delimiteur)){
        morceauxInte.push_back(extraitInte);
    }

    for (std::string s : morceauxInte){

        if(s.find("@todo") != std::string::npos && (s.find("@date") != std::string::npos)){

            std::vector<std::string> morceauxToDo;
            std::stringstream ss2(s);
            std::string extraitToDo;
            char delimiteur2 = ' ';
            while(std::getline(ss2,extraitToDo,delimiteur2)){
                morceauxToDo.push_back(extraitToDo);
            }

            bool estLaDate = false;
            for (std::string s2 : morceauxToDo){
                if (estLaDate == true){
                    int jour = stoi(s2.substr(0,2));
                    int mois = stoi(s2.substr(3,2));
                    int annee = stoi(s2.substr(6,4));

                    auto dtd = date::year{annee}/mois/jour;

                    toDo tdAjoutAvecDate(s,dtd);
                    listeT.ajouterToDo(tdAjoutAvecDate);
                    break;
                }

                if (s2.find("@date") != std::string::npos){
                    estLaDate = true;
                }
            }


        } else if (s.find("@todo") != std::string::npos){
            toDo tdAjoutSansDate(s);
            listeT.ajouterToDo(tdAjoutSansDate);
        }
    }

    return listeT;
}
