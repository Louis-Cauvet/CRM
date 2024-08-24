# CRM 👨‍💼

> Projet de Conception d'Applications en L3 Informatique

### Contexte 🎯
L'objectif de ce projet était de développer une application dialoguant avec une base de données SQLLite, afin de permettre son utilisateur de gérer une liste composée de ses contacts professionnels, en pouvant attribuer des notes ("interactions") et des tâches à chaque contact. Les fonctionnalités attendues pour les listes de contacts, d'interactions et des tâches étaient *CRUD* (créer, lire, modifier, supprimer), ainsi que des fonctions de tri (par date de création, par ordre alphabétique...) et de recherche (par nom, par entreprise, dans un intervalle de 2 dates...).

De plus, l'utilisateur devait pouvoir, en ecrivant une interaction pour un contact, indiquer si la ligne qu'il etait en train d'écrire constituait une tâche à réaliser (en ajoutant "@todo" devant), et une possible date de réalisation (en ajoutant "@date" devant), et pouvoir exporter une liste au format JSON si il le voulait.

Enfin, il était attendu de notre part une phase de conception exprimée par des diagrammes de classe et de cas d'utilisation, ainsi qu'une documentation Doxygen permettant de détailler le rôle et les composants de chaque classe et chaque fonction.

### Technologies ⚙️
Projet réalisé en **C++**, avec l'aide de l'environnement de développement **QtCreator**, qui a notamment permis de simplifier la création d'interfaces (avec son outil *Designer*). La base de données **SQLite** a été gérée à l'aide de l'outil **DBeaver**, et les commentaires du code répondaient à la norme **Doxygen**.

### Collaborateurs 👥
*Travail en binôme :*
- Félix Daclin
- Louis Cauvet

### Retour personnel 💭
Je suis globalement satisfait du résultat final de ce projet, qui comprend des interfaces efficaces et de nombreuses fonctionnalités complètes.

Petit bémol sur la conception post-développement de notre projet qui n'étaient pas la bonne à aborder, ce qui nous vaut une grosse dépendance au niveau de la persistance des données (à chaque appel de fonction, l'application effectue une requête dans la base de données pour récupérer et afficher les données correspondantes, alors que nous aurions dû passer par une structure de liste en C++, ce qui nous aurait permis de récupérer une seule fois les données depuis la base, et de les traiter directement dans la liste avant de les afficher ensuite. </br>
Cette erreur de conception justifie notamment que la recherche par intervalle de dates et le tri par date sont les seules fonctionnalités qui ne marchent pas dans notre application. 


### Quelques images du projet 📷
![Liste des contacts](https://github.com/Louis-Cauvet/Captures-des-projets/blob/main/CRM/Capture.png)
</br></br>
![Fiche contact](https://github.com/Louis-Cauvet/Captures-des-projets/blob/main/CRM/Capture2.png)
</br></br>
![Création d'interaction](https://github.com/Louis-Cauvet/Captures-des-projets/blob/main/CRM/Capture3.png)
</br></br>
![Liste des tâches](https://github.com/Louis-Cauvet/Captures-des-projets/blob/main/CRM/Capture4.png)
