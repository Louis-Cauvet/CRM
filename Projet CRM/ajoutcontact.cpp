/**
 * @file ajoutcontact.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#include "ajoutcontact.h"
#include "ui_ajoutcontact.h"
#include "contact.h"

#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>

/**
 * @brief le constructeur de la classe
 * @brief on créé une instance d'interface
 * @warning toutes les connexions signaux/slots liées a des éléments de l'interface sont effectuées dans le constructeur
 */
AjoutContact::AjoutContact(QWidget *parent) : QDialog(parent), ui(new Ui::AjoutContact)
{
    ui->setupUi(this);

    initialisationFenetre();

    QObject::connect(ui->ImporterPhoto,SIGNAL(clicked()),this,SLOT(ajouterPhotoContact()));
    QObject::connect(ui->boutonValider,SIGNAL(clicked()),this,SLOT(validerNouveauContact()));
    QObject::connect(ui->boutonRetour,SIGNAL(clicked()),this,SLOT(retour()));
}


/**
 * @brief le destructeur de la classe
 */
AjoutContact::~AjoutContact()
{
    delete ui;
}


/**
 * @brief fonction permettant d'initialiser la fenetre avec l'aspect et les données affichées lors de son ouverture
 * @brief on initialise les dimensions du champ permettant d'afficher la photo du contact
 * @warning on cache l'url de la photo, afin qu'il ne soit pas visible pour les utilisateurs mais récupérable facilement
 */
void AjoutContact::initialisationFenetre(){
    QPixmap pix("C:/Users/louis/Desktop/ProjetFinalCDAA_gp_AH_CAUVET_DACLIN/Photos/IconeContactVide.png");
    int w = ui->champPhoto->width();
    int h = ui->champPhoto->height();
    ui->champPhoto->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    ui->champUrlPhoto->hide();

}


/**
 * @brief fonction permettant à l'utilisateur de sélectionner une photo pour le contact en cours de création
 * @brief on ouvre une fenetre permettant de choisir une photo, et de récupérer son url
 * @brief on affiche la photo sélectionnée afin de donner un apreçu à l'utilisateur
 * @var urlPhoto correspond à l'url absolu de la photo, qui sera ensuite stockée dans la base de données
 */
void AjoutContact::ajouterPhotoContact()
{
    QString recherchePhoto = QFileDialog::getOpenFileName(this, "Choisissez une photo pour le contact", "C://", tr("Images (*.png *.PNG *.jpg *.xpm)"));
    QFileInfo cheminPhoto(recherchePhoto);

    QPixmap pix(recherchePhoto);
    int w = ui->champPhoto->width();
    int h = ui->champPhoto->height();
    ui->champPhoto->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    QString urlPhoto = cheminPhoto.absoluteFilePath();
    ui->champUrlPhoto->setText(urlPhoto);
}


/**
 * @brief fonction permettant de valider la création du nouveau contact, et son enregistrement dans la base de données
 * @see on appelle la fonction VerifsChampsContact() de cette classe pour vérifier que toutes les conditions ont été remplies avant la validation
 * @brief on initialise un contact avec les données saisies par l'utilisateur, pour lui attribuer automtatiquement une date de création et de modification
 * @see on appelle la fonction ajouterNouveauContact() de la classe gestionBDD pour enregistrer le nouveau contact dans la base de données
 */
void AjoutContact::validerNouveauContact()
{
    bool verifOk = VerifsChampsContact();
    if(verifOk == true){

        std::string nomNC = ui->champNom->text().toStdString();
        std::string prenomNC = ui->champPrenom->text().toStdString();
        std::string entrepriseNC = ui->champEntreprise->text().toStdString();
        std::string mailNC = ui->champMail->text().toStdString();
        std::string telNC = ui->champTelephone->text().toStdString();
        std::string urlPhotoNC = ui->champUrlPhoto->text().toStdString();
        contact nouvCont(nomNC, prenomNC, entrepriseNC, mailNC, telNC, urlPhotoNC);

        QString dateCreaNC =  QString::fromStdString(nouvCont.conversionDateCreaToString());
        QString dateModifNC = QString::fromStdString(nouvCont.conversionDateModifToString());

        gbdd->ajouterNouveauContact(ui->champNom->text(),ui->champPrenom->text(),ui->champEntreprise->text(),ui->champMail->text(),ui->champTelephone->text(),ui->champUrlPhoto->text(),dateCreaNC,dateModifNC);
        QMessageBox::information(this,"Contact ajouté !", "Le contact a bien été ajouté à la liste des contacts");
        this->close();
    }
}


/**
 * @brief fonction permettant de vérifier si toutes les conditions nécéssaires à la création d'un contact ont bien été remplies
 * @warning on vérifie que tous les champs du contact sont remplis (pas de valeur nulle)
 * @warning on vérifie que l'on a ajouté une photo pour le contact
 * @warning on vérifie que l'adresse mail possède un "@"
 * @warning on vérifie que le numéro de téléphone se compose de 10 chiffres uniquement
 * @return un booléen avec la valeur "True" si toutes les conditions sont vérifiées, "False" sinon
 */
bool AjoutContact::VerifsChampsContact(){
    bool champsVerifies = false;

    std::string mailContact = ui->champMail->text().toStdString();
    std::string telContact = ui->champTelephone->text().toStdString();
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

    if(ui->champNom->text() == "" || ui->champPrenom->text() == "" || ui->champEntreprise->text() == "" || ui->champMail->text() == "" || ui->champTelephone->text() == ""){
        QMessageBox::critical(this, "Informations manquantes", "Vous devez remplir tous les champs pour créer le contact");
    } else if (ui->champUrlPhoto->text() == ""){
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
 * @brief fonction permettant de fermer la fenêtre
 */
void AjoutContact::retour(){
    this->close();
}


/**
 * @brief surcharge de la fonction close() de la fenêtre
 * @brief on emet le signal fermerFenetre pour actualiser la fenêtre qui va être ouverte à la place
 * @return l'état close() de la fenetre
 */
bool AjoutContact::close(){
    emit fermerFenetre();
    return QWidget::close();
}
