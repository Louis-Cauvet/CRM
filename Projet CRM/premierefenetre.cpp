/**
 * @file premierefenetre.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#include "premierefenetre.h"
#include "qsqlquery.h"
#include "ui_premierefenetre.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>


/**
 * @brief le constructeur de la classe
 * @brief on créé une instance d'interface
 * @see on appelle la fonction recupererCheminBDD() de cette classe pour afficher une pop-up permettant à l'utilisateur de choisir la connexion bdd à utiliser
 * @see on appelle la fonction actualiserFenetre() de cette classe pour afficher par défaut les données que l'on consulte
 * @see on appelle la fonction afficherChampRecherche() de cette classe pour afficher par défaut un champ de recherche textuel
 * @warning toutes les connexions signaux/slots liées a des éléments de l'interface sont effectuées dans le constructeur
 */
PremiereFenetre::PremiereFenetre(QWidget *parent) : QDialog(parent), ui(new Ui::PremiereFenetre)
{
    ui->setupUi(this);

    recupererCheminBDD();

    actualiserFenetre();
    afficherChampRecherche();

    QObject::connect(ui->choixRecherche,SIGNAL(currentTextChanged(QString)),this,SLOT(modifierTypeDeRecherche(QString)));
    QObject::connect(this,SIGNAL(signalAfficherChampRecherche()),this,SLOT(afficherChampRecherche()));
    QObject::connect(this,SIGNAL(signalAfficherIntervDates()),this,SLOT(afficherIntervDates()));
    QObject::connect(ui->champRecherche,SIGNAL(textChanged(QString)),this,SLOT(modifierChampRecherche(QString)));
    QObject::connect(ui->dateDebut,SIGNAL(userDateChanged(QDate)),this,SLOT(modifierDateDebut(QDate)));
    QObject::connect(ui->dateFin,SIGNAL(userDateChanged(QDate)),this,SLOT(modifierDateFin(QDate)));
    QObject::connect(ui->choixModeTri,SIGNAL(currentTextChanged(QString)),this,SLOT(changerModeTri(QString)));
    QObject::connect(ui->ExportJson,SIGNAL(clicked()),this,SLOT(exporterJson()));
    QObject::connect(ui->NouveauContact,SIGNAL(clicked()),this,SLOT(ouvrirAjoutContact()));
    QObject::connect(ui->consulterContact,SIGNAL(clicked()),this,SLOT(ouvrirFicheContact()));
    QObject::connect(ui->ListeTotalTaches,SIGNAL(clicked()),this,SLOT(ouvrirListeTotaliteTaches()));
}


/**
 * @brief le destructeur de la classe
 */
PremiereFenetre::~PremiereFenetre()
{
    delete ui;
}


/**
 * @brief fonction permettant d'ouvrir une pop-up dasn laquelle l'utilisateur peut choisir la connexion bdd à utiliser
 * @var cheminBDD correspond au chemin absolu du fichier de configuration de la base de données qu'on veut utiliser
 * @brief une fois le fichier de configuration choisi, on crée une instance de gestionBDD afin d'établir la connexion avec la base de données
 * @warning si l'utilisateur ne choisit pas de fichier de configuration, le programme affiche un message d'erreur puis s'arrête automatiquement
 */
void PremiereFenetre::recupererCheminBDD(){
    QString cheminBDD = QFileDialog::getOpenFileName(this, "Choisissez le fichier de configuration de base de données à utiliser", "C://", tr("Fichier de configuration BDD (*.db)"));

    if (cheminBDD == ""){
        QMessageBox::critical(this, "Aucune donnée à charger", "Vous devez obligatoirement sélectionner un fichier.db");
        exit(EXIT_FAILURE);
    } else {
        gestionBDD g(cheminBDD);
    }
}


/**
* @brief fonction permettant d'actualiser l'aspect et les données de la fenêtre une fois que celle-ci a subi des modifications
* @brief on nettoie le tableau de son contenu avant de réafficher toutes les données
* @see on appelle la fonction remplirTableauContactsParDate() de la classe gestionBDD pour récupérer les données des contacts qui seront affichés dans le tableau de l'interface
* @see on appelle la fonction afficherLignesTableau de cette classe pour afficher dans le tableau la liste des contacts
* @see on appelle la fonction retournerDerniereSuppressionContact() de la classe gestionBDD pour récupérer dans la base de données la dernière date de suppression d'un contact
* @warning on émet signalAfficherChampRecherche pour afficher par défaut un champ de recherche textuel lors de l'actualisation de la fenêtre
*/
void PremiereFenetre::actualiserFenetre(){
    ui->tableListeContacts->setColumnHidden(0,true);
    ui->tableListeContacts->setRowCount(0);

    QSqlQuery requete = gbdd->remplirTableauContactsParDate();
    afficherLignesTableau(requete);

    QString requete2 = gbdd->retournerDerniereSuppressionContact();
    ui->dateDerniereSuppression->setText(requete2);

    emit signalAfficherChampRecherche();

    ui->tableListeContacts->setCurrentItem(nullptr);
    ui->nombreContacts->setText(gbdd->retournerNbTotalContacts());
    this->show();
}


/**
 * @brief fonction permettant d'afficher la liste des contacts dans le tableau de l'interface
 * @param qu correspond au résultat de la requête qui récupère les données des contacts dans la base de données
 * @brief pour chaque ligne renvoyée par la requête, on créé des nouveaux QTableWidgetItem à afficher dans le QtableWidget
 * @var numLigne est un entier qui s'incrémente à chaque fois que l'on ajoute une ligne supplémentaire au tableau
 */
void PremiereFenetre::afficherLignesTableau(QSqlQuery &qu){
    int numLigne = 0;

    while(qu.next()){
        ui->tableListeContacts->insertRow(numLigne);

        QTableWidgetItem *id = new QTableWidgetItem;
        QTableWidgetItem *dc = new QTableWidgetItem;
        QTableWidgetItem *n = new QTableWidgetItem;
        QTableWidgetItem *p = new QTableWidgetItem;
        QTableWidgetItem *e = new QTableWidgetItem;
        QTableWidgetItem *t = new QTableWidgetItem;
        QTableWidgetItem *m = new QTableWidgetItem;

        id->setText(qu.value(0).toString());
        dc->setText(qu.value(1).toString());
        n->setText(qu.value(2).toString());
        p->setText(qu.value(3).toString());
        e->setText(qu.value(4).toString());
        t->setText(qu.value(5).toString());
        m->setText(qu.value(6).toString());

        ui->tableListeContacts->setItem(numLigne,0,id);
        ui->tableListeContacts->setItem(numLigne,1,dc);
        ui->tableListeContacts->setItem(numLigne,2,n);
        ui->tableListeContacts->setItem(numLigne,3,p);
        ui->tableListeContacts->setItem(numLigne,4,e);
        ui->tableListeContacts->setItem(numLigne,5,t);
        ui->tableListeContacts->setItem(numLigne,6,m);

        numLigne++;
    }
}


/**
 * @brief fonction permettant de modifier le résultat de la recherche effectuée par l'utilisateur lorsqu'il change le type de recherche
 * @param choixRech est une chaine de caractères qui correspond au type de recherche choisi par l'utilisateur
 * @brief on vérifie si le type de recherche choisi nécéssite un champ textuel, ou alors une interface de sélection par intervalle de dates
 * @warning si l'utilisateur choisit une recherche par intervalle de dates, on émet signalAfficherIntervDates() qui permet d'afficher une interface de sélection par intervalle de dates
 * @warning si l'utilisateur choisit une recherche par champ textuel, on émet signalAfficherChampRecherche() qui permet d'afficher une interface de recherche par champ textuel
 */
void PremiereFenetre::modifierTypeDeRecherche(const QString &choixRech)
{
    if (choixRech == "Intervalle de dates"){
        ui->tableListeContacts->setRowCount(0);
        QSqlQuery requete = gbdd->remplirTableauContactsParDate();
        afficherLignesTableau(requete);
        emit signalAfficherIntervDates();
    } else {
        ui->champRecherche->clear();
        emit signalAfficherChampRecherche();
    }
}


/**
 * @brief fonction permettant d'afficher une interface de recherche par champ textuel
 */
void PremiereFenetre::afficherChampRecherche(){
    ui->champRecherche->show();
    ui->labelDateDebut->hide();
    ui->dateDebut->hide();
    ui->labelDateFin->hide();
    ui->dateFin->hide();
}


/**
 * @brief fonction permettant d'afficher une interface de sélection par intervalle de dates
 */
void PremiereFenetre::afficherIntervDates(){
    ui->champRecherche->hide();
    ui->labelDateDebut->show();
    ui->dateDebut->show();
    ui->labelDateFin->show();
    ui->dateFin->show();
}


/**
 * @brief fonction permettant d'afficher les résultats de la recherche par champ textuel lorsque l'utilisateur change son contenu
 * @param choixRech correspond à la nouvelle recherche saisie par l'utilisateur dans le champ textuel
 * @var triAlphabétique est un booléen renvoyant "true" si le tri des contact s'effectue par ordre alphabétique, "false" si c'est par date de création
 * @brief en focntion du type de recherche et du mode de tri choisi par l'utilisateur, on appelle différentes fonction de la classe gestion BDD qui renvoient toutes une liste de contacts ordonnée différemment
 * @see si l'utilisateur a choisi une recherche par nom, on appelle la fonction rechercherContactSelonNom() de la classe gestionBDD pour récupérer dans la base de données la liste des contacts dont le nom correspond à la recherche
 * @see si l'utilisateur a choisi une recherche par entreprise, on appelle la fonction rechercherContactSelonEntreprise() de la classe gestionBDD pour récupérer dans la base de données la liste des contacts dont l'entreprise correspond à la recherche
 * @see quel que soit le type de recherche par champ textuel choisi, on appelle la fonction afficherLignesTableau de cette classe pour afficher dans le tableau la liste des contacts récupérée juste avant
 */
void PremiereFenetre::modifierChampRecherche(const QString &choixRech)
{
    ui->tableListeContacts->setRowCount(0);

    bool triAlphabetique;
    if (ui->choixModeTri->currentText() == "Ordre alphabétique"){
        triAlphabetique = true;
    } else {
        triAlphabetique = false;
    }

    if(ui->choixRecherche->currentText() == "Recherche par nom"){
        QSqlQuery requete = gbdd->rechercherContactSelonNom(choixRech, triAlphabetique);
        afficherLignesTableau(requete);
    } else if (ui->choixRecherche->currentText() == "Recherche par entreprise"){
        QSqlQuery requete = gbdd->rechercherContactSelonEntreprise(choixRech,triAlphabetique);
        afficherLignesTableau(requete);
    }
}


/**
 * @brief fonction permettant de vérifier que la date de début reste antérieure à la date de fin lorsqu'on la change
 * @param date correspond à la nouvelle date de début
 */
void PremiereFenetre::modifierDateDebut(const QDate &date)
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
void PremiereFenetre::modifierDateFin(const QDate &date)
{
    QDate dateD = ui->dateDebut->date();
    if(date<dateD){
        ui->dateDebut->setDate(date);
    }
}


/** @brief fonction permettant de changer l'ordre des données du tableau lorsque l'on change le mode de tri
 *  @param choixTri correspond au mode de tri choisi par l'utilisateur
 *  @brief on vérifie le mode de tri choisi avant de renvoyer les données des contacts ordonnées en conséquence
 *  @see on appelle la fonction remplirTableauContactsParDate() de la classe gestionBDD si l'on veut afficher les contacts par date de création (choix par défaut)
 *  @see on appelle la fonction remplirTableauContactsParAlphabetique() de la classe gestionBDD si l'on veut afficher les contacts par ordre alphabétique
 */
void PremiereFenetre::changerModeTri(const QString &choixTri)
{
    ui->champRecherche->setText("");
    if(choixTri == "Date de création"){
        ui->tableListeContacts->setRowCount(0);
        QSqlQuery requete = gbdd->remplirTableauContactsParDate();
        afficherLignesTableau(requete);
    } else if (choixTri == "Ordre alphabétique"){
        ui->tableListeContacts->setRowCount(0);
        QSqlQuery requete = gbdd->remplirTableauContactsParAlphabetique();
        afficherLignesTableau(requete);
    }
}


/**
 * @brief fonction permettant d'exporter la liste de tous les contacts dans un fichier JSON
 * @var docJson est un QJsonDocument qui correspond au document qui sera exporté au format JSON
 * @var donneesJson est un QJsonArray qui va récupérer les données de tous les contacts exportés
 * @var tailleTab correspond au nombre de contacts que l'on retrouve dans le tableau
 * @brief pour chaque contact, on définit un QJsonObjet qui correspond aux données du contact, puis on ajoute cet objet dans le QJsonArray
 * @brief on ouvre une fenetre afin que l'utilisateur choisisse l'emplacement où il souhaite enregistrer le document JSON
 * @var cheminFichJson correspond au chemin absolu de l'emplacement où sera enregistré le document JSON
 */
void PremiereFenetre::exporterJson(){
    QJsonArray donneesJson;
    QJsonDocument docJson;
    int tailleTab = ui->tableListeContacts->rowCount();

    for(int i=0; i<tailleTab; i++){
        QString dateCreaC = ui->tableListeContacts->item(i,1)->text();
        QString nomC = ui->tableListeContacts->item(i,2)->text();
        QString prenomC = ui->tableListeContacts->item(i,3)->text();
        QString entrepriseC = ui->tableListeContacts->item(i,4)->text();
        QString telephoneC = ui->tableListeContacts->item(i,5)->text();
        QString mailC = ui->tableListeContacts->item(i,6)->text();

        QJsonObject contactJson;
        contactJson.insert("date de création", dateCreaC);
        contactJson.insert("nom", nomC);
        contactJson.insert("prénom", prenomC);
        contactJson.insert("entreprise", entrepriseC);
        contactJson.insert("numéro de téléphone", telephoneC);
        contactJson.insert("adresse-mail", mailC);

        donneesJson.push_back(contactJson);
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
 * @brief fonction permettant d'ouvrir la fenetre permettant d'ajouter un contact
 * @brief on initialise une instance de la fenêtre à ouvrir
 * @warning on connecte le signal fermerFenetre() de la fenetre que l'on va ouvrir avec la fonction actualiserFenetre() de cette classe
 */
void PremiereFenetre::ouvrirAjoutContact()
{
    aj = new AjoutContact;

    QObject::connect(aj,SIGNAL(fermerFenetre()),this,SLOT(actualiserFenetre()));

    aj->show();
    this->close();
}


/**
 * @brief fonction permettant d'ouvrir la fenetre permettant de consulter un contact sélectionné
 * @warning on vérifie d'abord qu'un contact à été sélectionné dans le tableau afin de pouvoir le consulter
 * @var idSelec correspond à l'identifiant du contact sélectionné par l'utilisateur dans le tableau
 * @brief on initialise une instance de la fenêtre à ouvrir
 * @warning on connecte le signal fermerFenetre() de la fenetre que l'on va ouvrir avec la fonction actualiserFenetre() de cette classe
 */
void PremiereFenetre::ouvrirFicheContact()
{
    QList selec = ui->tableListeContacts->selectedRanges();

    if(selec.count() == 0){
        QMessageBox::critical(this, "Pas de contact sélectionné", "Vous devez sélectionner un contact à consulter");
    } else {
        int ligneSelec = ui->tableListeContacts->currentItem()->row();
        QString idSelec = ui->tableListeContacts->item(ligneSelec,0)->text();
        fc = new FicheContact(nullptr,idSelec);

        QObject::connect(fc,SIGNAL(fermerFenetre()),this,SLOT(actualiserFenetre()));

        fc->show();
        this->close();
    }
}


/**
 * @brief fonction permettant d'ouvrir la fenetre permettant de consulter la liste de l'ensemble des tâches
 * @brief on initialise une instance de la fenêtre à ouvrir
 * @warning on connecte le signal fermerFenetre() de la fenetre que l'on va ouvrir avec la fonction actualiserFenetre() de cette classe
 */
void PremiereFenetre::ouvrirListeTotaliteTaches(){
    ltt = new ListeTotaliteTaches;

    QObject::connect(ltt,SIGNAL(fermerFenetre()),this,SLOT(actualiserFenetre()));

    ltt->show();
    this->close();
}
