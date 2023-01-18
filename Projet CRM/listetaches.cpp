/**
 * @file listetaches.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#include "listetaches.h"
#include "ui_listetaches.h"

#include <QMessageBox>

/**
 * @brief le constructeur de la classe
 * @param idContact correspond à l'identifiant du contact auquel est associé la liste de tâches que l'on consulte
 * @brief on créé une instance d'interface
 * @see on appelle la fonction actualiserFenetre() de cette classe pour afficher par défaut les données que l'on consulte
 * @warning toutes les connexions signaux/slots liées a des éléments de l'interface sont effectuées dans le constructeur
 */
ListeTaches::ListeTaches(QWidget *parent,QString idContact) : QDialog(parent), ui(new Ui::ListeTaches)
{
    ui->setupUi(this);

    this->identifiantContact = idContact;

    actualiserFenetre();

    QObject::connect(ui->dateDebut,SIGNAL(userDateChanged(QDate)),this,SLOT(changerDateDebut(QDate)));
    QObject::connect(ui->dateFin,SIGNAL(userDateChanged(QDate)),this,SLOT(changerDateFin(QDate)));
    QObject::connect(ui->SupprimerTache,SIGNAL(clicked()),this,SLOT(supprimerTache()));
    QObject::connect(ui->boutonRetour,SIGNAL(clicked()),this,SLOT(retour()));
    QObject::connect(this,SIGNAL(actualiserApresSuppTache()),this,SLOT(actualiserFenetre()));
}


/**
 * @brief le destructeur de la classe
 */
ListeTaches::~ListeTaches()
{
    delete ui;
}


/**
 * @brief fonction permettant d'actualiser l'aspect et les données de la fenêtre une fois que celle-ci a subi des modifications
 * @brief on nettoie le tableau de son contenu avant de réafficher toutes les données
 * @see on appelle la fonction retournerIdentiteContact() de la classe gestionBDD pour récupérer l'identité du contact associé à la liste de tâches consultée
 * @see on appelle la fonction afficherIdentiteContact de cette classe pour afficher le nom et le prénom du contact associé à la liste de tâches consultée
 * @see on appelle la fonction remplirTableauTachesContact() de la classe gestionBDD pour récupérer dans la base de données la liste des tâches liées au contact consulté
 * @see on appelle la fonction afficherListeTaches de cette classe pour afficher dans le tableau la liste des tâches liées au contact consulté
 */
void ListeTaches::actualiserFenetre(){
    ui->tableListeTaches->setColumnHidden(0,true);
    ui->tableListeTaches->setRowCount(0);

    QSqlQuery requete = gbdd->retournerIdentiteContact(identifiantContact);
    afficherIdentiteContact(requete);

    QSqlQuery requete2 = gbdd->remplirTableauTachesContact(identifiantContact);
    afficherListeTaches(requete2);

    ui->tableListeTaches->setCurrentItem(nullptr);
}


/**
 * @brief fonction permettant d'afficher le nom et le prénom du contact associé à la liste de tâche consultée
 * @param qu correpond au résultat de la requête qui retourne l'identité du contact
 */
void ListeTaches::afficherIdentiteContact(QSqlQuery &qu){
    if (qu.first()){
        QString nomC = qu.value(0).toString();
        QString prenomC = qu.value(1).toString();
        QString identiteC = nomC + " " + prenomC;

        ui->nomContact->setText(identiteC);
    }
}


/**
 * @brief fonction permettant d'afficher la liste des tâches liées au contact dans le tableau de l'interface
 * @param qu correspond au résultat de la requête qui récupère les données des tâches liées au contact
 * @brief pour chaque ligne renvoyée par la requête, on créé des nouveaux QTableWidgetItem à afficher dans le QtableWidget
 * @var numLigne est un entier qui s'incrémente à chaque fois que l'on ajoute une ligne supplémentaire au tableau
 */
void ListeTaches::afficherListeTaches(QSqlQuery &qu){
    int numLigne = 0;

    while(qu.next()){
        ui->tableListeTaches->insertRow(numLigne);

        QTableWidgetItem *idt = new QTableWidgetItem;
        QTableWidgetItem *dr = new QTableWidgetItem;
        QTableWidgetItem *con = new QTableWidgetItem;

        idt->setText(qu.value(0).toString());
        dr->setText(qu.value(1).toString());
        con->setText(qu.value(2).toString());

        ui->tableListeTaches->setItem(numLigne,0,idt);
        ui->tableListeTaches->setItem(numLigne,1,dr);
        ui->tableListeTaches->setItem(numLigne,2,con);

        numLigne++;
    }
}


/**
 * @brief fonction permettant de vérifier que la date de début reste antérieure à la date de fin lorsqu'on la change
 * @param date correspond à la nouvelle date de début
 */
void ListeTaches::changerDateDebut(const QDate &date)
{
    QDate dateF = ui->dateFin->date();
    if(date>dateF){
        ui->dateDebut->setDate(dateF);
    }
}


/**
 * @brief fonction permettant de vérifier que la date de fin reste postérieure à la date de début lorsqu'on la change
 * @param date correspond à la nouvelle date de fin
 */
void ListeTaches::changerDateFin(const QDate &date)
{
    QDate dateD = ui->dateDebut->date();
    if(date<dateD){
        ui->dateDebut->setDate(date);
    }
}


/**
 * @brief fonction permettant de supprimer la tâche sélectionnée de la liste des tâches du contact
 * @warning on vérifie que l'utilisateur à bien sélectionnée dans la liste une tâche à supprimer
 * @var idSelec correspond à l'identifiant de la tâche sélectionnée par l'utilisateur dans le tableau
 * @see on appelle la fonction supprimerTache() de la classe gestionBDD pour supprimer la tâche dans la base de données
 * @brief on emet le signal actualiserApresSuppTache() pour actualiser directement la fenêtre après la suppression
 */
void ListeTaches::supprimerTache()
{
    QList selec = ui->tableListeTaches->selectedRanges();
    if(selec.count() == 0){
        QMessageBox::critical(this, "Pas de tâche sélectionnée", "Vous devez sélectionner une tâche à supprimer");
    } else {
        QMessageBox::StandardButton reponse = QMessageBox::question(this,"Supprimer la tâche ?", "Voulez-vous vraiment supprimer définitivement cette tâche ? ",QMessageBox::Yes | QMessageBox::No);
        if(reponse == QMessageBox::Yes){
            int ligneSelec = ui->tableListeTaches->currentItem()->row();
            QString idSelec = ui->tableListeTaches->item(ligneSelec,0)->text();
            gbdd->supprimerTache(idSelec);

            QMessageBox::information(this,"Tâche supprimée", "La tâche a été supprimée de la liste");
            emit actualiserApresSuppTache();
        }
    }
}


/**
 * @brief fonction permettant de fermer la fenêtre
 */
void ListeTaches::retour()
{
    this->close();
}


/**
 * @brief surcharge de la fonction close() de la fenêtre
 * @brief on emet le signal fermerFenetre pour actualiser la fenêtre qui va être ouverte à la place
 * @return l'état close() de la fenetre
 */
bool ListeTaches::close()
{
    emit fermerFenetre();
    return QWidget::close();
}



/**
 * @brief fonction permettant d'exécuter une requête qui retourne la dernière date de suppression de contacts dans la base de données
 * @return la dernière date de suppression de contacts dans la base de données
 * @return le résultat d'une requête qui contient la liste des contacts dont l'entreprise correspond  à une certaine recherche passée en paramètres
 */
