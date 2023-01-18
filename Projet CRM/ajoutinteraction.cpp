/**
 * @file ajoutinteraction.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#include "ajoutinteraction.h"
#include "ui_ajoutinteraction.h"
#include "interraction.h"

#include <QMessageBox>
#include <QTextBlock>
#include <QRegularExpression>
#include <QSqlQuery>

/**
 * @brief le constructeur de la classe
 * @param idContact correspond à l'identifiant du contact pour lequel on veut ajouter une interaction
 * @brief on créé une instance d'interface
 * @warning toutes les connexions signaux/slots liées a des éléments de l'interface sont effectuées dans le constructeur
 */
ajoutInteraction::ajoutInteraction(QWidget *parent,QString idContact) : QDialog(parent), ui(new Ui::ajoutInteraction)
{
    ui->setupUi(this);

    this->identifiantContact = idContact;

    QObject::connect(ui->boutonValider,SIGNAL(clicked()),this,SLOT(validerAjout()));
    QObject::connect(ui->boutonRetour,SIGNAL(clicked()),this,SLOT(retour()));
}


/**
 * @brief le destructeur de la classe
 */
ajoutInteraction::~ajoutInteraction()
{
    delete ui;
}


/**
 * @brief fonction permettant de valider la création d'une nouvelle interaction, et son enregistrement dans la base de données
 * @warning on vérifie que les champs titre et contenu ne sont pas vides avant de valider l'ajout
 * @brief on initialise une interaction avec les données saisies par l'utilisateur, pour lui attribuer automtatiquement une date de modification
 * @see on appelle la fonction ajouterNouvelleInteraction() de la classe gestionBDD pour enregistrer la nouvelle interaction dans la base de données
 * @see on appelle la fonction recupererIdentifiantInteraction() de la classe gestionBDD pour récupérer l'identifiant de l'interaction qui vient d'être créée
 * @brief on emet le signal traiterInteraction pour rechercher dans la nouvelle interaction des tâches à ajouter à la base de données
 */
void ajoutInteraction::validerAjout()
{
    if(ui->champTitreInte->text() == ""){
        QMessageBox::critical(this, "Titre manquant", "Vous devez saisir un titre pour l'interaction");
    } else if (ui->champContenuInte->toPlainText() == ""){
        QMessageBox::critical(this, "Contenu manquant", "Vous devez saisir un contenu pour l'interaction");
    } else {
        std::string titreInte = ui->champTitreInte->text().toStdString();
        std::string contenuInte = ui->champContenuInte->toPlainText().toStdString();
        interraction nouvInte(titreInte, contenuInte);
        QString dateModifNI = QString::fromStdString(nouvInte.conversionDateModifToString());

        gbdd->ajouterNouvelleInteraction(ui->champTitreInte->text(),ui->champContenuInte->toPlainText(),dateModifNI,identifiantContact.toInt());
        int idInte = gbdd->recupererIdentifiantInteraction();

        QObject::connect(this,SIGNAL(traiterInteraction(int &)),this,SLOT(recupererTaches(int &)));
        emit traiterInteraction(idInte);

        QMessageBox::information(this,"Interaction ajoutée !", "L'interaction a bien été ajoutée à la liste des interactions, et les tâches correspondantes à la liste des tâches");
        this->close();
    }
}


/**
 * @brief fonction permettant de parcourir chaque ligne d'une interaction qui vient d'être créée pour récupérer des tâches à ajouter dans la base de données
 * @param idI correspond à l'identifiant de l'interaction que l'on vient de créer
 * @var qdoc correspond au texte complet de la nouvelle interaction
 * @brief on parcourt chaque ligne de l'interaction pour voir si elle débute par "@ todo"
 * @var lTaches est une liste de chaines de caractères contenant toutes les lignes de l'interaction qui concernent une tâche à réaliser
 * @brief on analyse chaque ligne de lTaches pour vérifier si elle contient un "@ date"
 * @brief on découpe la ligne concernant une tache en 2 parties : son contenu et sa date de réalisation
 * @var texteContenu est une chaine de caractères qui contient le contenu de la tâche (ce que l'on trouve après le "@ todo")
 * @var dateTodo est une chaine de caractères qui contient la date de réalisation de la tâche si elle est précisée avec "@ date"
 * @brief on initialise une tâche avec le contenu et la date de réalisation, avant de l'ajouter dans la base de données
 * @see on appelle la fonction ajouterNouvelleTache() de la classe gestionBDD pour enregistrer chaque nouvelle tâche dans la base de données
 */
void ajoutInteraction::recupererTaches(int &idI){
    QTextDocument * qdoc = ui->champContenuInte->document();
    QTextCursor curs(qdoc);
    QTextDocument::FindFlag fcs = (QTextDocument::FindFlag) 0;

    QStringList lTaches;
    QRegularExpression qreg("^@todo .*$");
    while (!curs.isNull()){
        curs = qdoc->find(qreg, curs.position(), fcs);
        if (!curs.isNull()){
            QTextBlock qtb = curs.block();
            lTaches.append(qtb.text());
        }
    }

    for(QString ligne : lTaches){
        std::string ligneDecoupee = ligne.toStdString();
        std::size_t postodo = ligneDecoupee.find("@todo")+6;
        std::string contenuLigne = ligneDecoupee.substr(postodo);

       if (contenuLigne.find("@date") != std::string::npos){
            int position = ligneDecoupee.find("@date");
            int tailleContenu = position - postodo;
            std::string texteContenu = ligneDecoupee.substr(postodo, tailleContenu);

            std::size_t posdate = ligneDecoupee.find("@date");
            std::string dateTodo = contenuLigne.substr(posdate);

            int jour = stoi(dateTodo.substr(0,2));
            int mois = stoi(dateTodo.substr(3,2));
            int annee = stoi(dateTodo.substr(6,4));
            auto dtd = date::year{annee}/mois/jour;

            toDo td(texteContenu, dtd);
            QString dateReaNT = QString::fromStdString(td.conversionDateReaToString());
            gbdd->ajouterNouvelleTache(QString::fromStdString(td.getContenuTD()),dateReaNT,idI);

        } else {
            toDo td(contenuLigne);
            QString dateReaNT = QString::fromStdString(td.conversionDateReaToString());
            gbdd->ajouterNouvelleTache(QString::fromStdString(td.getContenuTD()),dateReaNT,idI);
        }
    }
}


/**
 * @brief fonction permettant de fermer la fenêtre
 */
void ajoutInteraction::retour()
{
    this->close();
}


/**
 * @brief surcharge de la fonction close() de la fenêtre
 * @brief on emet le signal fermerFenetre pour actualiser la fenêtre qui va être ouverte à la place
 * @return l'état close() de la fenetre
 */
bool ajoutInteraction::close()
{
    emit fermerFenetre();
    return QWidget::close();
}


