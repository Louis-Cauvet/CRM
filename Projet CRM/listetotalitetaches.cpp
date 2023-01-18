/**
 * @file listetotalitetaches.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#include "listetotalitetaches.h"
#include "qsqlquery.h"
#include "ui_listetotalitetaches.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

/**
 * @brief le constructeur de la classe
 * @brief on créé une instance d'interface
 * @see on appelle la fonction actualiserFenetre() de cette classe pour afficher par défaut les données que l'on consulte
 * @warning toutes les connexions signaux/slots liées a des éléments de l'interface sont effectuées dans le constructeur
 */
ListeTotaliteTaches::ListeTotaliteTaches(QWidget *parent) : QWidget(parent), ui(new Ui::ListeTotaliteTaches)
{
    ui->setupUi(this);

    actualiserFenetre();

    QObject::connect(ui->dateDebut,SIGNAL(userDateChanged(QDate)),this,SLOT(changerDateDebut(QDate)));
    QObject::connect(ui->dateFin,SIGNAL(userDateChanged(QDate)),this,SLOT(changerDateFin(QDate)));
    QObject::connect(ui->SupprimerTache,SIGNAL(clicked()),this,SLOT(supprimerTache()));
    QObject::connect(ui->ExportJson,SIGNAL(clicked()),this,SLOT(exporterJson()));
    QObject::connect(ui->BoutonRetour,SIGNAL(clicked()),this,SLOT(retour()));
    QObject::connect(this,SIGNAL(actualiserApresSuppTache()),this,SLOT(actualiserFenetre()));
}


/**
 * @brief le destructeur de la classe
 */
ListeTotaliteTaches::~ListeTotaliteTaches()
{
    delete ui;
}


/**
 * @brief fonction permettant d'actualiser l'aspect et les données de la fenêtre une fois que celle-ci a subi des modifications
 * @brief on nettoie le tableau de son contenu avant de réafficher toutes les données
 * @see on appelle la fonction remplirTableauTotaliteTaches() de la classe gestionBDD pour récupérer dans la base de données la liste de toutes les tâches
 * @see on appelle la fonction afficherListeTaches de cette classe pour afficher dans le tableau la liste de toutes les tâches
 */
void ListeTotaliteTaches::actualiserFenetre(){
    ui->tableListeTotaliteTaches->setColumnHidden(0,true);
    ui->tableListeTotaliteTaches->setRowCount(0);

    QSqlQuery requete = gbdd->remplirTableauTotaliteTaches();
    afficherListeTaches(requete);

    ui->tableListeTotaliteTaches->setCurrentItem(nullptr);
}


/**
 * @brief fonction permettant d'afficher la liste de toutes les tâches dans le tableau de l'interface
 * @param qu correspond au résultat de la requête qui récupère les données de toutes les tâches
 * @brief pour chaque ligne renvoyée par la requête, on créé des nouveaux QTableWidgetItem à afficher dans le QtableWidget
 * @var numLigne est un entier qui s'incrémente à chaque fois que l'on ajoute une ligne supplémentaire au tableau
 */
void ListeTotaliteTaches::afficherListeTaches(QSqlQuery &qu){
    int numLigne = 0;

    while(qu.next()){
        ui->tableListeTotaliteTaches->insertRow(numLigne);

        QTableWidgetItem *idt = new QTableWidgetItem;
        QTableWidgetItem *dr = new QTableWidgetItem;
        QTableWidgetItem *con = new QTableWidgetItem;
        QTableWidgetItem *identite = new QTableWidgetItem;

        idt->setText(qu.value(0).toString());
        dr->setText(qu.value(1).toString());
        con->setText(qu.value(2).toString());
        identite->setText(qu.value(4).toString()+" "+qu.value(3).toString());

        ui->tableListeTotaliteTaches->setItem(numLigne,0,idt);
        ui->tableListeTotaliteTaches->setItem(numLigne,1,dr);
        ui->tableListeTotaliteTaches->setItem(numLigne,2,con);
        ui->tableListeTotaliteTaches->setItem(numLigne,3,identite);

        numLigne++;
    }
}


/**
 * @brief fonction permettant de vérifier que la date de début reste antérieure à la date de fin lorsqu'on la change
 * @param date correspond à la nouvelle date de début
 */
void ListeTotaliteTaches::changerDateDebut(const QDate &date)
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
void ListeTotaliteTaches::changerDateFin(const QDate &date)
{
    QDate dateD = ui->dateDebut->date();
    if(date<dateD){
        ui->dateDebut->setDate(date);
    }
}


/**
 * @brief fonction permettant de supprimer la tâche sélectionnée de la liste de toutes les tâches
 * @warning on vérifie que l'utilisateur à bien sélectionnée dans la liste une tâche à supprimer
 * @var idSelec correspond à l'identifiant de la tâche sélectionnée par l'utilisateur dans le tableau
 * @see on appelle la fonction supprimerTache() de la classe gestionBDD pour supprimer la tâche dans la base de données
 * @brief on emet le signal actualiserApresSuppTache() pour actualiser directement la fenêtre après la suppression
 */
void ListeTotaliteTaches::supprimerTache()
{
    QList selec = ui->tableListeTotaliteTaches->selectedRanges();
    if(selec.count() == 0){
        QMessageBox::critical(this, "Pas de tâche sélectionnée", "Vous devez sélectionner une tâche à supprimer");
    } else {
        QMessageBox::StandardButton reponse = QMessageBox::question(this,"Supprimer la tâche ?", "Voulez-vous vraiment supprimer définitivement cette tâche ? ",QMessageBox::Yes | QMessageBox::No);
        if(reponse == QMessageBox::Yes){
            int ligneSelec = ui->tableListeTotaliteTaches->currentItem()->row();
            QString idSelec = ui->tableListeTotaliteTaches->item(ligneSelec,0)->text();
            gbdd->supprimerTache(idSelec);

            QMessageBox::information(this,"Tâche supprimée", "La tâche a été supprimée de la liste");
            emit actualiserApresSuppTache();
        }
    }
}


/**
 * @brief fonction permettant d'exporter la liste de toutes les tâches dans un fichier JSON
 * @var docJson est un QJsonDocument qui correspond au document qui sera exporté au format JSON
 * @var donneesJson est un QJsonArray qui va récupérer les données de toutes les tâches exportées
 * @var tailleTab correspond au nombre de tâches que l'on retrouve dans le tableau qui liste l'ensemble des tâches
 * @brief pour chaque tâche, on définit un QJsonObjet qui correspond aux données de la tâche, puis on ajoute cet objet dans le QJsonArray
 * @brief on ouvre une fenetre afin que l'utilisateur choisisse l'emplacement où il souhaite enregistrer le document JSON
 * @var cheminFichJson correspond au chemin absolu de l'emplacement où sera enregistré le document JSON
 */
void ListeTotaliteTaches::exporterJson(){
    QJsonDocument docJson;
    QJsonArray donneesJson;
    int tailleTab = ui->tableListeTotaliteTaches->rowCount();

    for(int i=0; i<tailleTab; i++){
        QString dateReaT = ui->tableListeTotaliteTaches->item(i,1)->text();
        QString contenuT = ui->tableListeTotaliteTaches->item(i,2)->text();
        QString identiteC = ui->tableListeTotaliteTaches->item(i,3)->text();

        QJsonObject tacheJson;
        tacheJson.insert("date de réalisation", dateReaT);
        tacheJson.insert("contenu", contenuT);
        tacheJson.insert("identité du contact", identiteC);

        donneesJson.push_back(tacheJson);
    }

    docJson.setArray(donneesJson);
    QByteArray data = docJson.toJson();

    QString cheminFichJson = QFileDialog::getSaveFileName(this,"Où voulez-vous enregistrer votre export JSON ?", "C://", tr("JSON Files (*.json)"));

    QFile fichierJson(cheminFichJson);
    if (!fichierJson.open(QIODevice::WriteOnly)){
        qDebug() << "Vous n'avez pas ouvert de fichier JSON";
    } else {
        QTextStream stream(&fichierJson);
        stream << data;
        stream.flush();
        fichierJson.close();

        QStringList decoupageNomFichier = fichierJson.fileName().split(QChar('/'));
        QString nomFichier = decoupageNomFichier.last();
        QMessageBox::information(this,"Export JSON effectué !", "Le fichier "+ nomFichier + " à bien été créé et enregistré à l'endroit spécifié !");
    }
}


/**
 * @brief fonction permettant de fermer la fenêtre
 */
void ListeTotaliteTaches::retour()
{
    this->close();
}


/**
 * @brief surcharge de la fonction close() de la fenêtre
 * @brief on emet le signal fermerFenetre pour actualiser la fenêtre qui va être ouverte à la place
 * @return l'état close() de la fenetre
 */
bool ListeTotaliteTaches::close()
{
    emit fermerFenetre();
    return QWidget::close();
}
