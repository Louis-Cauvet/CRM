/**
 * @file detailinteraction.cpp
 * @authors Louis CAUVET - Félix DACLIN
 * @date novembre/decembre 2022
 **/

#include "detailinteraction.h"
#include "ui_detailinteraction.h"
#include "todo.h"

#include <QMessageBox>
#include <QTextBlock>
#include <QRegularExpression>

/**
 * @brief le constructeur de la classe
 * @param idInteraction correspond à l'identifiant de l'interaction que l'on souhaite consulter
 * @brief on créé une instance d'interface
 * @see on appelle la fonction retournerDonneesInteraction() de la classe gestionBDD pour récupérer les données de l'interaction que l'on veut consulter
 * @see on appelle la fonction afficherDonneesInteraction() de cette classe pour afficher par défaut les données de l'interaction que l'on consulte
 * @warning toutes les connexions signaux/slots liées a des éléments de l'interface sont effectuées dans le constructeur
 */
DetailInteraction::DetailInteraction(QWidget *parent,QString idInteraction) : QDialog(parent), ui(new Ui::DetailInteraction)
{
    ui->setupUi(this);

    this->identifiantInteraction = idInteraction;

    QSqlQuery requete = gbdd->retournerDonneesInteraction(idInteraction);
    afficherDonneesInteraction(requete);

    QObject::connect(ui->validerModifs,SIGNAL(clicked()),this,SLOT(validerModifications()));
    QObject::connect(ui->boutonRetour,SIGNAL(clicked()),this,SLOT(retour()));
}


/**
 * @brief le destructeur de la classe
 */
DetailInteraction::~DetailInteraction()
{
    delete ui;
}


/**
 * @brief fonction permettant d'afficher les données de l'interaction que l'on consulte dans les champs associés de l'interface
 * @param qu correspond au résultat de la requête retournant les données de l'interaction à consulter
 */
void DetailInteraction::afficherDonneesInteraction(QSqlQuery &qu){
    if (qu.first()){
        ui->champTitreInte->setText(qu.value(1).toString());
        ui->champContenuInte->setPlainText(qu.value(2).toString());
        ui->dateDerniereModif->setText(qu.value(3).toString());
    }
}


/**
 * @brief fonction permettant de valider les modifications effectuées sur l'interaction consultée
 * @warning on vérifie que les champs titre et contenu ne sont pas vides avant de valider la modification
 * @see on appelle la fonction modifierInteraction() de la classe gestionBDD pour mettre à jour les données de l'interaction (grâce à son identifiant passé en paramètres)
 * @brief on emet le signal traiterInteraction pour rechercher dans l'interaction de nouvelles tâches à ajouter à la base de données
 */
void DetailInteraction::validerModifications()
{
    QMessageBox::StandardButton reponse = QMessageBox::question(this,"Valider les modifications ?","Voulez-vous vraiment valider les modifications de cette interraction ?");

    if(reponse == QMessageBox::Yes){
        if(ui->champTitreInte->text() == ""){
            QMessageBox::critical(this, "Titre manquant", "Vous devez saisir un titre pour l'interaction");
        } else if (ui->champContenuInte->toPlainText() == ""){
            QMessageBox::critical(this, "Contenu manquant", "Vous devez saisir un contenu pour l'interaction");
        } else {
            gbdd->modifierInteraction(identifiantInteraction,ui->champTitreInte->text(),ui->champContenuInte->toPlainText());

            int idInte = gbdd->recupererIdentifiantInteraction();
            QObject::connect(this,SIGNAL(traiterInteraction(int &)),this,SLOT(recupererTaches(int &)));
            emit traiterInteraction(idInte);

            QMessageBox::information(this,"Modifications effectuées !", "L'interaction a bien été modifiée");
            this->close();
        }
    }
}


/**
 * @brief fonction permettant de parcourir chaque ligne d'une interaction qui vient d'être mise à jour pour récupérer des tâches à ajouter dans la base de données
 * @param idI correspond à l'identifiant de l'interaction que l'on vient de mettre à jour
 * @var qdoc correspond au nouveau texte complet de l'interaction
 * @brief on parcourt chaque ligne de l'interaction pour voir si elle débute par "@ todo"
 * @var lTaches est une liste de chaines de caractères contenant toutes les lignes de l'interaction qui concernent une tâche à réaliser
 * @brief on analyse chaque ligne de lTaches pour vérifier si elle contient un "@ date"
 * @brief on découpe la ligne concernant une tache en 2 parties : son contenu et sa date de réalisation
 * @var texteContenu est une chaine de caractères qui contient le contenu de la tâche (ce que l'on trouve après le "@ todo")
 * @var dateTodo est une chaine de caractères qui contient la date de réalisation de la tâche si elle est précisée avec "@ date"
 * @brief on initialise une tâche avec le contenu et la date de réalisation, avant de l'ajouter dans la base de données
 * @see on appelle la fonction ajouterNouvelleTache() de la classe gestionBDD pour enregistrer chaque nouvelle tâche dans la base de données
 */
void DetailInteraction::recupererTaches(int &idI){
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
void DetailInteraction::retour()
{
    this->close();
}


/**
 * @brief surcharge de la fonction close() de la fenêtre
 * @brief on emet le signal fermerFenetre pour actualiser la fenêtre qui va être ouverte à la place
 * @return l'état close() de la fenetre
 */
bool DetailInteraction::close()
{
    emit fermerFenetre();
    return QWidget::close();
}

