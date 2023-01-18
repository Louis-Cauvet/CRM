/**
 * @file gestionbdd.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#include "gestionbdd.h"
#include "qdebug.h"
#include "date.h"

#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QFileDialog>

/**
 * using nécessaires pour utiliser les types d'attributs date définies dans le date.h
 */
using namespace date;
using namespace std::chrono;

/**
 * @brief le constructeur de la classe
 * @param chemin correspond au chemin absolu du fichier de configuration de base de données choisi par l'utilisateur
 * @see on appelle la fonction connexionBDD() pour établir une connexion avec la base de données à l'aide du fichier de configuration passé en paramètres
 */
gestionBDD::gestionBDD(QString &chemin)
{
    connexionBDD(chemin);
}


/**
 * @brief le destructeur de la classe
 */
gestionBDD::~gestionBDD()
{
}


/**
 * @brief fonction permettant d'établir une connexion avec la base de données
 * @param ch corrspond au chemin absolu permettant de récupérer la base de données sélectionnée par l'utilisateur
 * @warning si la connexion n'arrive pas à s'établir correctement, le programme s'arrête de lui même pour éviter les problèmes de connexion ensuite
 */
void gestionBDD::connexionBDD(QString &ch){

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(ch);

    if(db.open()){
        qDebug() << "Réussite de la connexion à la base de données !";
    } else {
        qDebug() << "La connexion à la base de données à échouée !";
        exit(EXIT_FAILURE);
    }
}


/**
 * @brief fonction permettant de fermer la connexion à la base de données
 */
void gestionBDD::fermerConnexion(){
    db.close();
    qDebug() << "la base de données a bien été fermée";
}


/**
 * @brief fonction permettant d'exécuter une requête qui retourne le nombre total de contacts stockés dans la base de données
 * @return le nombre total de contacts stockés dans la base de données
 */
QString gestionBDD::retournerNbTotalContacts(){

    QSqlQuery query("SELECT COUNT (IDc) FROM Contact");
    QString resultat = "";

    if(query.exec()){

        if(query.next() == true){
            resultat = query.value(0).toString();
        }

    } else {
        qDebug() << "souci dans l'exécution de la requête du nombre de contacts";
    }

    return resultat;

}


/**
 * @brief fonction permettant d'exécuter une requête qui retourne la dernière date de suppression de contacts stockée dans la base de données
 * @return la dernière date de suppression de contacts dans la base de données
 */
QString gestionBDD::retournerDerniereSuppressionContact(){

    QSqlQuery query("SELECT Dateds FROM DernSupp");
    QString resultat = "";

    if(query.exec()){

        if(query.next() == true){
            resultat = query.value(0).toString();
        }

    } else {
        qDebug() << "souci dans l'exécution de la requête de la date de dernière suppression";
    }

    return resultat;

}


/**
 * @brief fonction permettant d'exécuter une requête qui ajoute un nouveau contact dans la base de données
 * @param n correspond au nom du nouveau contact
 * @param p correspond au prénom du nouveau contact
 * @param e correspond à l'entreprise du nouveau contact
 * @param m correspond à l'adresse-mail du nouveau contact
 * @param t correspond au numéro de téléphone du nouveau contact
 * @param u correspond à l'url de la photo du nouveau contact
 * @param dc correspond à la date de création du nouveau contact
 * @param dm correspond à la date de modification du nouveau contact
 */
void gestionBDD::ajouterNouveauContact(const QString &n,const QString &p,const QString &e,const QString &m,const QString &t,const QString &u,const QString &dc,const QString &dm){

    QSqlQuery query;
    query.prepare("INSERT INTO Contact (Nom,Prenom,Entreprise,AdresseMail,Téléphone,UrlPhoto,DateCreationC,DateModifC) VALUES ("
                    ":n,:p,:e,:m,:t,:u,:dc,:dm);");
    query.bindValue(":n",n);
    query.bindValue(":p",p);
    query.bindValue(":e",e);
    query.bindValue(":m",m);
    query.bindValue(":t",t);
    query.bindValue(":u",u);
    query.bindValue(":dc",dc);
    query.bindValue(":dm",dm);

    if(!query.exec()){
        qDebug() << "souci dans l'exécution de la requête";
    }

}


/**
 * @brief fonction permettant d'exécuter une requête qui retourne la liste des contacts stockés dans la base de données, ordonnés par date de création
 * @return le résultat d'une requête qui contient la liste des contacts ordonnés par date de création
 */
QSqlQuery gestionBDD::remplirTableauContactsParDate(){

    QSqlQuery query;
    query.prepare("SELECT IDc,DateCreationC,Nom,Prenom,Entreprise,Téléphone,AdresseMail FROM Contact;");

    if(!query.exec()){
        qDebug() << "souci dans l'exécution de la requête, pas d'insertion des contacts dans le tableau";
    }

    return query;

}


/**
 * @brief fonction permettant d'exécuter une requête qui retourne la liste des contacts stockés dans la base de données, ordonnés par ordre alphabétique
 * @return le résultat d'une requête qui contient la liste des contacts ordonnés par ordre alphabétique
 */
QSqlQuery gestionBDD::remplirTableauContactsParAlphabetique(){

    QSqlQuery query;
    query.prepare("SELECT IDc,DateCreationC,Nom,Prenom,Entreprise,Téléphone,AdresseMail FROM Contact ORDER BY Nom, Prenom ASC;");

    if(!query.exec()){
        qDebug() << "souci dans l'exécution de la requête, pas d'insertion des contacts dans le tableau";
    }

    return query;

}


/**
 * @brief fonction permettant d'exécuter une requête qui retourne la liste des contacts stockés dans la base de données dont le nom correspond à une certaine recherche passée en paramètres
 * @param rech correspond à la recherche textuelle effectuée par l'utilisateur
 * @param alphab correpond au choix du type de tri : true si c'est un tri par ordre alphabétique et false si c'est pas date de création
 * @warning la requête renvoyant les contacts est adaptée selon le mode de tri choisi par l'utilisateur et passé en paramètres
 * @return le résultat d'une requête qui contient la liste des contacts dont le nom correspond  à une certaine recherche passée en paramètres
 */
QSqlQuery gestionBDD::rechercherContactSelonNom(const QString &rech, bool &alphab){

    QSqlQuery query;

    if(alphab == true){
        query.prepare("SELECT IDc,DateCreationC,Nom,Prenom,Entreprise,Téléphone,AdresseMail FROM Contact WHERE Nom LIKE '%" + rech +"%' ORDER BY Nom, Prenom ASC;");
    } else {
        query.prepare("SELECT IDc,DateCreationC,Nom,Prenom,Entreprise,Téléphone,AdresseMail FROM Contact WHERE Nom LIKE '%" + rech +"%' ORDER BY DateCreationC ASC;");
    }

    if(!query.exec()){
        qDebug() << "souci dans l'exécution de la requête, pas de recherche par nom des contacts";
    }

    return query;

}


/**
 * @brief fonction permettant d'exécuter une requête qui retourne la liste des contacts stockés dans la base de données dont l'entreprise correspond à une certaine recherche passée en paramètres
 * @param rech correspond à la recherche textuelle effectuée par l'utilisateur
 * @param alphab correpond au choix du type de tri : true si c'est un tri par ordre alphabétique et false si c'est pas date de création
 * @warning la requête renvoyant les contacts est adaptée selon le mode de tri choisi par l'utilisateur et passé en paramètres
 * @return le résultat d'une requête qui contient la liste des contacts dont l'entreprise correspond  à une certaine recherche passée en paramètres
 */
QSqlQuery gestionBDD::rechercherContactSelonEntreprise(const QString &rech, bool &alphab){

    QSqlQuery query;
    if(alphab == true){
        query.prepare("SELECT IDc,DateCreationC,Nom,Prenom,Entreprise,Téléphone,AdresseMail FROM Contact WHERE Entreprise LIKE '%" + rech +"%' ORDER BY Nom, Prenom ASC;");
    } else {
        query.prepare("SELECT IDc,DateCreationC,Nom,Prenom,Entreprise,Téléphone,AdresseMail FROM Contact WHERE Entreprise LIKE '%" + rech +"%' ORDER BY DateCreationC ASC;");
    }


    if(!query.exec()){
        qDebug() << "souci dans l'exécution de la requête, pas de recherche par nom des contacts";
    }

    return query;

}


/**
 * @brief fonction d'exécuter une requête qui retourne les données d'un seul contact identifié par son identifiant passé en paramètres
 * @param id correspond à l'identifiant du contact dont on souhaite récupérer les données
 * @return le résultat d'une requête qui contient les données du contact qui possède l'identifiant passé en paramètres
 */
QSqlQuery gestionBDD::retournerDonneesContact(const QString &id){

    QSqlQuery query;
    query.prepare("SELECT * FROM Contact WHERE IDc = "+ id +";");

    if(!query.exec()){
        qDebug() << "souci dans l'exécution de la requête, on ne peut pas récupérer les données du contact";
    }

    return query;

}


/**
 * @brief fonction permettant d'exécuter une requête qui met à jour les données d'un contact identifié par son identifiant passé en paramètres
 * @param id correspond à l'identifiant du contact qu'on veut mettre à jour
 * @param n correspond au nouveau nom du contact
 * @param p correspond au nouveau prénom du contact
 * @param e correspond à la nouvelle entreprise du contact
 * @param m correspond à la nouvelle adresse-mail du contact
 * @param t correspond au nouveau numéro de téléphone du contact
 * @param u correspond à la nouvelle url de la photo du contact
 * @brief on met à jour automatiquement la date de modification du contact avec la date du jour
 * @warning on fait en sorte que la date de modification du contact soit toujours de la forme "dd/mm/yyyy" en rajoutant des 0 devant le jour ou le mois si nécéssaire
 */
void gestionBDD::modifierContact(const QString &id,const QString &n,const QString &p,const QString &e,const QString &t,const QString &m,const QString &u){

     auto dateDuJour = date::year_month_day{floor<days>(system_clock::now())};
     std::string dateModJour = std::to_string(dateDuJour.day().operator unsigned int());
     if(dateModJour.size()==1){
         dateModJour = "0"+dateModJour;
     }
     std::string dateModMois = std::to_string(dateDuJour.month().operator unsigned int());
     if(dateModMois.size()==1){
         dateModMois = "0"+dateModMois;
     }
     std::string dateModAnnee = std::to_string(dateDuJour.year().operator int());
     std::string dateModString = dateModJour + "/" + dateModMois + "/" + dateModAnnee;
     QString dm = QString::fromStdString(dateModString);

     QSqlQuery query;
     query.prepare("UPDATE Contact SET Nom = :n, Prenom = :p, Entreprise = :e, Téléphone = :t, AdresseMail = :m, UrlPhoto = :u, DateModifC = :dm WHERE IDc = "+ id +";");
     query.bindValue(":n",n);
     query.bindValue(":p",p);
     query.bindValue(":e",e);
     query.bindValue(":t",t);
     query.bindValue(":m",m);
     query.bindValue(":u",u);
     query.bindValue(":dm",dm);

     if(!query.exec()){
         qDebug() << "souci dans l'exécution de la requête, pas de mise à jour du contact";
     }

}


/**
 * @brief fonction permettant d'exécuter une requête qui supprime un contact dans la base de données
 * @param id correspond à l'identifiant du contact que l'on souhaite supprimer
 */
void gestionBDD::supprimerContact(const QString &id){

     QSqlQuery query;
     query.prepare("DELETE FROM Contact WHERE IDc = '"+ id +"';");

     if(!query.exec()){
         qDebug() << "souci dans l'exécution de la requête, pas de suppression du contact";
     }

}


/**
 * @brief fonction permettant d'exécuter une requête qui met à jour la date de dernière suppression de contact dans la base de données
 * @param nvdate correspond à la nouvelle date de dernière suppression
 */
void gestionBDD::changerDerniereSuppressionContact(const QString &nvdate){

     QSqlQuery query;
     query.prepare("UPDATE DernSupp SET Dateds = '" + nvdate + "';");

     if(!query.exec()){
         qDebug() << "souci dans l'exécution de la requête, pas de mise à jour de la date de dernière suppression de contacts";
     }

}


/**
 * @brief fonction permettant d'exécuter une requête qui retourne la liste des interraction d'un contact stockées dans la base de données, ordonnées par identifiant descendant
 * @param id correspond à l'identifiant du contact dont on souhaite récupérer la liste des interactions
 * @return le résultat d'une requête qui contient la liste des interraction d'un contact ordonnées par identifiant descendant
 */
QSqlQuery gestionBDD::remplirTableauInteParDate(const QString &id){

     QSqlQuery query;
     query.prepare("SELECT IDi,DateModifI,Titre,ContenuI FROM Interaction WHERE IDc = '" + id +"'ORDER BY IDi DESC;");

     if(!query.exec()){
         qDebug() << "souci dans l'exécution de la requête, pas d'insertion des interactions dans le tableau";
     }

     return query;

}


/**
 * @brief fonction permettant d'exécuter une requête qui supprime une interaction dans la base de données
 * @param id correspond à l'identifiant de l'interaction que l'on souhaite supprimer
 */
void gestionBDD::supprimerInteraction(const QString &id){

     QSqlQuery query;
     query.prepare("DELETE FROM Interaction WHERE IDi = '"+ id +"';");

     if(!query.exec()){
         qDebug() << "souci dans l'exécution de la requête, pas de suppression de l'interaction";
     }

}


/**
 * @brief fonction permettant d'exécuter une requête qui ajoute une nouvelle interaction dans la base de données
 * @param t correspond au titre de la nouvelle interaction
 * @param c correspond au contenu de la nouvelle interaction
 * @param dm correspond à la date de modification de la nouvelle interaction
 * @param idc correspond à l'identifiant du contact associé à la nouvelle interraction
 */
void gestionBDD::ajouterNouvelleInteraction(const QString &t,const QString &c,const QString &dm,const int &idc){

     QSqlQuery query;
     query.prepare("INSERT INTO Interaction (Titre,ContenuI,DateModifI,IDc) VALUES ("
                     ":t,:c,:dm,:idc);");
     query.bindValue(":t",t);
     query.bindValue(":c",c);
     query.bindValue(":dm",dm);
     query.bindValue(":idc",idc);

     if(!query.exec()){
         qDebug() << "souci dans l'exécution de la requête, pas d'insertion d'interraction possible";
     }

}


/**
 * @brief fonction permettant d'exécuter une requete qui retourne l'identifiant de la dernière interaction ajoutée dans la base de données
 * @return l'identifiant de la dernière interaction ajoutée dans la base de données
 */
int gestionBDD::recupererIdentifiantInteraction(){

     QSqlQuery query;
     query.prepare("SELECT MAX(IDi) FROM Interaction;");

     if(!query.exec()){
        qDebug() << "souci dans l'exécution de la requête, on ne peut pas récupérer l'identifiant de l'interaction";
     }

     int idInte = 0;
     if(query.next()){
         idInte = query.value(0).toInt();
     }
     return idInte;

}


/**
 * @brief fonction permettant d'exécuter une requête qui ajoute une nouvelle tâche dans la base de données
 * @param c correspond au contenu de la nouvelle tâche
 * @param dr correspond à la date de réalisation de la nouvelle tâche
 * @param idi correspond à l'identifiant de l'interaction associée à la nouvelle tâche
 */
void gestionBDD::ajouterNouvelleTache(const QString &c,const QString &dr,const int &idi){

     QSqlQuery query;
     query.prepare("INSERT INTO Tache (ContenuT,DateReaT,IDi) VALUES (:c,:dr,:idi);");
     query.bindValue(":c",c);
     query.bindValue(":dr",dr);
     query.bindValue(":idi",idi);

     if(!query.exec()){
         qDebug() << "souci dans l'exécution de la requête, pas d'insertion de tâche possible";
     }

}


/**
 * @brief fonction permettant d'exécuter une requête qui retourne les données d'une seule interaction identifié par son identifiant passé en paramètres
 * @param id correspond à l'identifiant de l'interraction dont on souhaite récupérer les données
 * @return le résultat d'une requête qui contient les données de l'interaction qui possède l'identifiant passé en paramètres
 */
QSqlQuery gestionBDD::retournerDonneesInteraction(const QString &id){

    QSqlQuery query;
    query.prepare("SELECT * FROM Interaction WHERE IDi = "+ id +";");

    if(!query.exec()){
        qDebug() << "souci dans l'exécution de la requête, pas de recherche des données de l'interaction";
    }

    return query;

}


/**
 * @brief fonction permettant d'exécuter une requête qui met à jour les données d'une interaction identifiée par son identifiant passé en paramètres
 * @param id correspond à l'identifiant de l'interaction qu'on veut mettre à jour
 * @param t correspond au nouveau titre de l'interaction
 * @param c correspond au nouveau contenu de l'interaction
 */
void gestionBDD::modifierInteraction(const QString &id, const QString &t,const QString &c){

     auto dateDuJour = date::year_month_day{floor<days>(system_clock::now())};
     std::string dateModJour = std::to_string(dateDuJour.day().operator unsigned int());
     if(dateModJour.size()==1){
         dateModJour = "0"+dateModJour;
     }
     std::string dateModMois = std::to_string(dateDuJour.month().operator unsigned int());
     if(dateModMois.size()==1){
         dateModMois = "0"+dateModMois;
     }
     std::string dateModAnnee = std::to_string(dateDuJour.year().operator int());
     std::string dateModString = dateModJour + "/" + dateModMois + "/" + dateModAnnee;
     QString dm = QString::fromStdString(dateModString);


     QSqlQuery query;
     query.prepare("SELECT Idc FROM Interaction WHERE IDi = "+ id +";");
     QString idContact;
     if(query.exec()){
         if(query.next()){
            idContact = query.value(0).toString();
         }
     } else {
         idContact = nullptr;
         qDebug() << "souci dans l'exécution de la requête, on ne peut pas récupérer les données de l'interaction à modifier";
     }


     QSqlQuery query2;
     query2.prepare("DELETE FROM Interaction WHERE IDi = "+ id +";");
     if(!query2.exec()){
         qDebug() << "souci dans l'exécution de la requête, impossible de supprimer l'interaction";
     }


     QSqlQuery query3;
     query3.prepare("INSERT INTO Interaction (Titre,ContenuI,DateModifI,IDc) VALUES ("
                     ":t,:c,:dm,:idc);");
     query3.bindValue(":t",t);
     query3.bindValue(":c",c);
     query3.bindValue(":dm",dm);
     query3.bindValue(":idc",idContact);
     if(!query3.exec()){
         qDebug() << "souci dans l'exécution de la requête, pas d'insertion d'interraction possible";
     }

}


/**
 * @brief fonction permettant d'exécuter une requête qui retourne l'identité (prénom + nom) d'un contact identifié par l'identifiant passé en paramètres
 * @param idc correspond à l'identifiant du contact dont on souhaite connaître l'identité
 * @return l'identité du contact identifié par l'identifiant passé en paramètres
 */
QSqlQuery gestionBDD::retournerIdentiteContact(const QString &idc){

    QSqlQuery query;
    query.prepare("SELECT Nom,Prenom FROM Contact WHERE IDc = "+ idc +";");

    if(!query.exec()){
        qDebug() << "souci dans l'exécution de la requête, pas de retour sur l'identité du contact";
    }

    return query;

}


/**
 * @brief fonction permettant d'exécuter une requête qui retourne la liste des tâches d'un contact stockées dans la base de données
 * @param id correspond à l'identifiant du contact dont on souhaite obtenir la liste des tâches
 * @return le résultat d'une requête qui contient la liste des tâches d'un contact
 */
QSqlQuery gestionBDD::remplirTableauTachesContact(const QString &idc){

     QSqlQuery query;
     query.prepare("SELECT IDt,DateReaT,ContenuT FROM Tache INNER JOIN Interaction ON Tache.IDi = Interaction.IDi"
                   " INNER JOIN Contact ON Interaction.IDc = Contact.IDc WHERE Contact.IDc = '" + idc +"';");

     if(!query.exec()){
         qDebug() << "souci dans l'exécution de la requête, pas d'insertion des tâches du contact dans le tableau";
     }

     return query;

}


/**
 * @brief fonction permettant d'exécuter une requête qui retourne la liste de l'ensemble des tâches stockées dans la base de données
 * @return le résultat d'une requête qui contient la liste de l'ensemble des tâches
 */
QSqlQuery gestionBDD::remplirTableauTotaliteTaches(){

     QSqlQuery query;
     query.prepare("SELECT IDt,DateReaT,ContenuT,Nom,Prenom FROM Tache INNER JOIN Interaction ON Tache.IDi = Interaction.IDi INNER JOIN Contact ON Interaction.IDc  = Contact.IDc;");

     if(!query.exec()){
         qDebug() << "souci dans l'exécution de la requête, pas d'insertion de la totalité des tâches dans le tableau";
     }

     return query;

}


/**
 * @brief fonction permettant de supprimer une tâche dans la base de données
 * @param idt correspond à l'identifiant de la tâche que l'on souhaite supprimer
 */
void gestionBDD::supprimerTache(const QString &idt){

     QSqlQuery query;
     query.prepare("DELETE FROM Tache WHERE IDt = '"+ idt +"';");

     if(!query.exec()){
         qDebug() << "souci dans l'exécution de la requête, pas de suppression de la tâche";
     }

}
