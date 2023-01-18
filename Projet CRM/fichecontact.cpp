/**
 * @file fichecontact.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#include "fichecontact.h"
#include "ui_fichecontact.h"
#include "date.h"

#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

/**
 * using nécessaires pour utiliser les types d'attributs date définies dans le date.h
 */
using namespace date;
using namespace std::chrono;


/**
 * @brief le constructeur de la classe
 * @param idContact correspond à l'identifiant du contact que l'on souhaite consulter
 * @brief on créé une instance d'interface
 * @see on appelle la fonction actualiserFenetre() de cette classe pour afficher par défaut les données du contact que l'on consulte
 * @warning toutes les connexions signaux/slots liées a des éléments de l'interface sont effectuées dans le constructeur
 */
FicheContact::FicheContact(QWidget *parent, QString idContact) : QMainWindow(parent), ui(new Ui::FicheContact)
{
    this->show();
    ui->setupUi(this);

    this->identifiantContact = idContact;

    actualiserFenetre();

    QObject::connect(ui->ChangerPhoto,SIGNAL(clicked()),this,SLOT(changerPhotoContact()));
    QObject::connect(ui->ValiderModifs,SIGNAL(clicked()),this,SLOT(validerModifications()));
    QObject::connect(ui->SupprimerContact,SIGNAL(clicked()),this,SLOT(supprimerContact()));
    QObject::connect(ui->dateDebut,SIGNAL(userDateChanged(QDate)),this,SLOT(modifierDateDebut(QDate)));
    QObject::connect(ui->dateFin,SIGNAL(userDateChanged(QDate)),this,SLOT(modifierDateFin(QDate)));
    QObject::connect(ui->listeTachesContact,SIGNAL(clicked()),this,SLOT(ouvrirListeTaches()));
    QObject::connect(ui->SupprimerInteraction,SIGNAL(clicked()),this,SLOT(supprimerInteraction()));
    QObject::connect(ui->AjouterInteraction,SIGNAL(clicked()),this,SLOT(ouvrirAjoutInteraction()));
    QObject::connect(ui->ConsulterInteraction,SIGNAL(clicked()),this,SLOT(ouvrirDetailInteraction()));
    QObject::connect(ui->ExportJson,SIGNAL(clicked()),this,SLOT(exporterJson()));
    QObject::connect(ui->BoutonRetour,SIGNAL(clicked()),this,SLOT(boutonRetour()));
    QObject::connect(this,SIGNAL(actualiserApresSuppInte()),this,SLOT(actualiserFenetre()));
}


/**
 * @brief le destructeur de la classe
 */
FicheContact::~FicheContact()
{
    delete ui;
}


/**
 * @brief fonction permettant d'actualiser l'aspect et les données de la fenêtre une fois que celle-ci a subi des modifications
 * @see on appelle la fonction retournerDonneesContact() de la classe gestionBDD pour récupérer les données du contact que l'on veut consulter
 * @see on appelle la fonction afficherDonneesContact de cette classe pour afficher par défaut les données du contact que l'on consulte
 * @see on appelle la fonction remplirTableauInteParDate() de la classe gestionBDD pour récupérer dans la base de données la liste des interactions liées au contact consulté
 * @see on appelle la fonction afficherListeInteractions de cette classe pour afficher dans le tableau la liste des interactions liées au contact consulté
 */
void FicheContact::actualiserFenetre(){
    ui->tableListeInteractions->setColumnHidden(0,true);
    ui->tableListeInteractions->setColumnHidden(3,true);
    ui->tableListeInteractions->setRowCount(0);

    QSqlQuery requete = gbdd->retournerDonneesContact(identifiantContact);
    afficherDonneesContact(requete);

    QSqlQuery requete2 = gbdd->remplirTableauInteParDate(identifiantContact);
    afficherListeInteractions(requete2);

    ui->tableListeInteractions->setCurrentItem(nullptr);

    this->show();
}


/**
 * @brief fonction permettant d'afficher les données du contact que l'on consulte dans les champs associés de l'interface
 * @param qu correspond au résultat de la requête retournant les données du contact à consulter
 * @brief on attribue à chaque champ de l'interface une donnée récupérée grâce à la requete passée en paramètres
 * @warning si l'application ne parvient pas a retrouver la photo du contact à partir de son url, elle considère que le contact n'en a pas (son url de photo est indiqué comme vide)
 */
void FicheContact::afficherDonneesContact(QSqlQuery &qu){
    if (qu.first()){
        ui->champNom->setText(qu.value(1).toString());
        ui->champPrenom->setText(qu.value(2).toString());
        ui->champEntreprise->setText(qu.value(3).toString());
        ui->champMail->setText(qu.value(4).toString());
        ui->champTel->setText(qu.value(5).toString());
        ui->champUrl->setText(qu.value(6).toString());

        QPixmap pix(qu.value(6).toString());
        int w = ui->imgPhoto->width();
        int h = ui->imgPhoto->height();
        ui->imgPhoto->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

        if(pix.isNull()){
            ui->champUrl->setText("");
        }

        ui->champUrl->hide();
        ui->ChampDateCreation->setText(qu.value(7).toString());
        ui->ChampDateModif->setText(qu.value(8).toString());
    }
}


/**
 * @brief fonction permettant d'afficher la liste des interactions liées au contact dans le tableau de l'interface
 * @param qu correspond au résultat de la requête qui récupère les données des interactions liées au contact
 * @brief pour chaque ligne renvoyée par la requête, on créé des nouveaux QTableWidgetItem à afficher dans le QtableWidget
 * @var numLigne est un entier qui s'incrémente à chaque fois que l'on ajoute une ligne supplémentaire au tableau
 */
void FicheContact::afficherListeInteractions(QSqlQuery &qu){
    int numLigne = 0;

    while(qu.next()){
        ui->tableListeInteractions->insertRow(numLigne);

        QTableWidgetItem *id = new QTableWidgetItem;
        QTableWidgetItem *dm = new QTableWidgetItem;
        QTableWidgetItem *ti = new QTableWidgetItem;
        QTableWidgetItem *ci = new QTableWidgetItem;

        id->setText(qu.value(0).toString());
        dm->setText(qu.value(1).toString());
        ti->setText(qu.value(2).toString());
        ci->setText(qu.value(3).toString());

        ui->tableListeInteractions->setItem(numLigne,0,id);
        ui->tableListeInteractions->setItem(numLigne,1,dm);
        ui->tableListeInteractions->setItem(numLigne,2,ti);
        ui->tableListeInteractions->setItem(numLigne,3,ci);

        numLigne++;
    }
}


/**
 * @brief fonction permettant de modifier la photo du contact
 * @brief on ouvre une fenetre permettant de choisir une autre photo, et de récupérer son url
 * @brief on affiche la photo sélectionnée afin de donner un apreçu à l'utilisateur
 * @var urlPhoto correspond à l'url absolu de la photo, qui sera ensuite stockée dans la base de données
 */
void FicheContact::changerPhotoContact()
{
    QString recherchePhoto = QFileDialog::getOpenFileName(this, "Choisissez une  nouvelle photo pour le contact", "C://", tr("Images (*.png *.PNG *.jpg *.xpm"));
    QFileInfo cheminPhoto(recherchePhoto);

    QPixmap pix(recherchePhoto);
    int w = ui->imgPhoto->width();
    int h = ui->imgPhoto->height();
    ui->imgPhoto->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    QString urlPhoto = cheminPhoto.absoluteFilePath();
    ui->champUrl->setText(urlPhoto);
}


/**
 * @brief fonction permettant de valider les modifications des données du contact consulté
 * @see on appelle la fonction VerifsChampsContact() de cette classe pour vérifier que toutes les conditions ont été remplies avant la validation
 * @see on appelle la fonction modifierContact() de la classe gestionBDD pour mettre à jour dans la base de donnée les données du contact (identifié grâce à son identifiant passé en paramètres)
 */
void FicheContact::validerModifications()
{
    QMessageBox::StandardButton reponse = QMessageBox::question(this,"Valider les modifications ?", "Voulez-vous vraiment valider les changements dans les données du contact ?",QMessageBox::Yes | QMessageBox::No);

    if(reponse == QMessageBox::Yes){
        bool verifOk = verifsChampsContact();
        if(verifOk == true){
            gbdd->modifierContact(identifiantContact, ui->champNom->text(),ui->champPrenom->text(),ui->champEntreprise->text(),ui->champTel->text(),ui->champMail->text(),ui->champUrl->text());
            QMessageBox::information(this,"Modifications effectuées", "Les modifications ont été effectuées avec succès !");
            this->close();
        }
    }
}


/**
 * @brief fonction permettant de vérifier si toutes les conditions nécéssaires à la mise à jour d'un contact ont bien été remplies
 * @warning on vérifie que tous les champs du contact sont remplis (pas de valeur nulle)
 * @warning on vérifie que le contact possède bien une photo
 * @warning on vérifie que l'adresse mail possède un "@"
 * @warning on vérifie que le numéro de téléphone se compose de 10 chiffres uniquement
 * @return un booléen avec la valeur "True" si toutes les conditions sont vérifiées, "False" sinon
 */
bool FicheContact::verifsChampsContact(){
    bool champsVerifies = false;

    std::string mailContact = ui->champMail->text().toStdString();
    std::string telContact = ui->champTel->text().toStdString();
    int tailleTel = telContact.size();

    bool telOk = true;
    auto charTel = telContact.begin();
    while (charTel != telContact.end()){
        if(std::isdigit(*charTel)){
            charTel++;
        } else {
            telOk = false;
            break;
        }
    }

    if(ui->champNom->text() == "" || ui->champPrenom->text() == "" || ui->champEntreprise->text() == "" || ui->champMail->text() == "" || ui->champTel->text() == ""){
        QMessageBox::critical(this, "Informations manquantes", "Vous devez remplir tous les champs pour créer le contact");
    } else if (ui->champUrl->text() == ""){
        QMessageBox::critical(this, "Photo manquante", "Vous devez ajouter une photo pour créer le contact");
    } else if (mailContact.find("@") == std::string::npos){
        QMessageBox::critical(this, "Adresse-mail incorrecte", "L'adresse-mail n'est pas correcte (il faut un @)");
    } else if (tailleTel != 10 || telOk == false){
        QMessageBox::critical(this, "Numéro de téléphone incorrect", "Le numéro de téléphone doit posséder obligatoirement uniquement 10 chiffres");
    }
    else {
        champsVerifies = true;
    }

    return champsVerifies;
}


/**
 * @brief fonction permettant de supprimer le contact consulté de la liste des contacts
 * @see on appelle la fonction supprimerContact() de la classe gestionBDD pour supprimer le contact dans la base de données
 * @brief une fois le contact supprimé, on récupère la date du jour qui deviendra la nouvelle dernière date de suppression de contact stockéee dans la base de données
 * @warning on fait en sorte que la date de suppression soit toujours de la forme "dd/mm/yyyy" en rajoutant des 0 devant le jour ou le mois si nécéssaire
 * @var ds correspond à la nouvelle dernière date de suppression de contact
 * @see on appelle la fonction supprimerContact() de la classe gestionBDD pour mettre à jour la date de dernière supppresion de contact dans la base de données
 * @brief on emet le signal fermerFenetre pour actualiser la fenêtre qui va être ouverte à la place
 */
void FicheContact::supprimerContact()
{
    QMessageBox::StandardButton reponse = QMessageBox::question(this,"Supprimer le contact ?", "Voulez-vous vraiment supprimer définitivement ce contact ? (Ses interractions et tâches associées le seront aussi)",QMessageBox::Yes | QMessageBox::No);
    if(reponse == QMessageBox::Yes){
        gbdd->supprimerContact(identifiantContact);
        QMessageBox::information(this,"Contact supprimé", "Le contact a été supprimé de la liste");

        auto dateDuJour = date::year_month_day{floor<days>(system_clock::now())};
        std::string dateSupJour = std::to_string(dateDuJour.day().operator unsigned int());
        if(dateSupJour.size()==1){
            dateSupJour = "0"+dateSupJour;
        }
        std::string dateSupMois = std::to_string(dateDuJour.month().operator unsigned int());
        if(dateSupMois.size()==1){
            dateSupMois = "0"+dateSupMois;
        }
        std::string dateSupAnnee = std::to_string(dateDuJour.year().operator int());
        std::string dateSupString = dateSupJour + "/" + dateSupMois + "/" + dateSupAnnee;
        QString ds = QString::fromStdString(dateSupString);

        gbdd->changerDerniereSuppressionContact(ds);

        emit fermerFenetre();
        this->close();
    }
}


/**
 * @brief fonction permettant de vérifier que la date de début reste antérieure à la date de fin lorsqu'on la change
 * @param date correspond à la nouvelle date de début
 */
void FicheContact::modifierDateDebut(const QDate &date)
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
void FicheContact::modifierDateFin(const QDate &date)
{
     QDate dateD = ui->dateDebut->date();
     if(date<dateD){
         ui->dateDebut->setDate(date);
     }
}


/**
 * @brief fonction permettant d'ouvrir la fenetre contenant la liste des tâches du contact
 * @brief on initialise une instance de la fenêtre à ouvrir
 * @warning on connecte le signal fermerFenetre() de la fenetre que l'on va ouvrir avec la fonction actualiserFenetre() de cette classe
 */
void FicheContact::ouvrirListeTaches(){
    lt = new ListeTaches(nullptr,identifiantContact);

    QObject::connect(lt,SIGNAL(fermerFenetre()),this,SLOT(actualiserFenetre()));

    lt->show();
    this->hide();
}


/**
 * @brief fonction permettant d'ouvrir la fenetre permettant d'ajouter une interaction
 * @brief on initialise une instance de la fenêtre à ouvrir
 * @warning on connecte le signal fermerFenetre() de la fenetre que l'on va ouvrir avec la fonction actualiserFenetre() de cette classe
 */
void FicheContact::ouvrirAjoutInteraction()
{
    ai = new ajoutInteraction(nullptr,identifiantContact);

    QObject::connect(ai,SIGNAL(fermerFenetre()),this,SLOT(actualiserFenetre()));

    ai->show();
    this->hide();
}


/**
 * @brief fonction permettant d'ouvrir la fenetre permettant de consulter une interaction sélectionnée
 * @warning on vérifie d'abord qu'une interaction à été sélectionnée dans le tableau afin de pouvoir la consulter
 * @var idSelec correspond à l'identifiant de l'interaction sélectionnée par l'utilisateur dans le tableau
 * @brief on initialise une instance de la fenêtre à ouvrir
 * @warning on connecte le signal fermerFenetre() de la fenetre que l'on va ouvrir avec la fonction actualiserFenetre() de cette classe
 */
void FicheContact::ouvrirDetailInteraction()
{
   QList selec = ui->tableListeInteractions->selectedRanges();
   if(selec.count() == 0){
       QMessageBox::critical(this, "Pas d'interaction sélectionnée", "Vous devez sélectionner une interraction à consulter");
   } else {
       int ligneSelec = ui->tableListeInteractions->currentItem()->row();
       QString idSelec = ui->tableListeInteractions->item(ligneSelec,0)->text();
       di = new DetailInteraction(nullptr,idSelec);

       QObject::connect(di,SIGNAL(fermerFenetre()),this,SLOT(actualiserFenetre()));

       di->show();
       this->hide();
   }
}


/**
 * @brief fonction permettant de supprimer une interaction sélectionnée parmi celle du contact que l'on consulte
 * @warning on vérifie d'abord qu'une interaction à été sélectionnée dans le tableau afin de pouvoir la supprimer
 * @var idSelec correspond à l'identifiant de l'interaction sélectionnée par l'utilisateur dans le tableau
 * @see on appelle la fonction supprimerInteraction() de la classe gestionBDD pour supprimer dans la base de données l'interaction sélectionnée
 * @brief on emet le signal actualiserApresSuppInte() pour actualiser directement la fenêtre après la suppression
 */
void FicheContact::supprimerInteraction()
{
    QList selec = ui->tableListeInteractions->selectedRanges();
    if(selec.count() == 0){
        QMessageBox::critical(this, "Pas d'interaction sélectionnée", "Vous devez sélectionner une interraction à supprimer");
    } else {
        QMessageBox::StandardButton reponse = QMessageBox::question(this,"Supprimer l'interaction ?", "Voulez-vous vraiment supprimer définitivement cette interaction (les tâches correspondantes seront supprimées aussi) ?",QMessageBox::Yes | QMessageBox::No);
        if(reponse == QMessageBox::Yes){
            int ligneSelec = ui->tableListeInteractions->currentItem()->row();
            QString idSelec = ui->tableListeInteractions->item(ligneSelec,0)->text();
            gbdd->supprimerInteraction(idSelec);

            QMessageBox::information(this,"Interaction supprimée", "L'interaction a été supprimée de la liste");
            emit actualiserApresSuppInte();
        }
    }
}


/**
 * @brief fonction permettant d'exporter la liste des interactions liées au contact que l'on consulte dans un fichier JSON
 * @var docJson est un QJsonDocument qui correspond au document qui sera exporté au format JSON
 * @var donneesJson est un QJsonArray qui va récupérer les données de toutes les interactions exportées
 * @var tailleTab correspond au nombre d'interaction que l'on retrouve dans le tableau qui liste les interactions du contact
 * @brief pour chaque interaction, on définit un QJsonObjet qui correspond aux données de l'interaction, puis on ajoute cet objet dans le QJsonArray
 * @brief on ouvre une fenetre afin que l'utilisateur choisisse l'emplacement où il souhaite enregistrer le document JSON
 * @var cheminFichJson correspond au chemin absolu de l'emplacement où sera enregistré le document JSON
 */
void FicheContact::exporterJson(){
    QJsonDocument docJson;
    QJsonArray donneesJson;
    int tailleTab = ui->tableListeInteractions->rowCount();

    for(int i=0; i<tailleTab; i++){
        QString dateModifI = ui->tableListeInteractions->item(i,1)->text();
        QString titreI = ui->tableListeInteractions->item(i,2)->text();
        QString contenuI = ui->tableListeInteractions->item(i,3)->text();

        QJsonObject interactionJson;
        interactionJson.insert("date de modification", dateModifI);
        interactionJson.insert("titre", titreI);
        interactionJson.insert("contenu", contenuI);

        donneesJson.push_back(interactionJson);
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
void FicheContact::boutonRetour()
{
    this->close();
}


/**
 * @brief surcharge de la fonction close() de la fenêtre
 * @brief on emet le signal fermerFenetre pour actualiser la fenêtre qui va être ouverte à la place
 * @return l'état close() de la fenetre
 */
bool FicheContact::close(){
    emit fermerFenetre();
    return QWidget::close();
}
