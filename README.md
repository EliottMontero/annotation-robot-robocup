### ANNOTATIONS-ROBOT

/!\------/!\

Si vous aviez une version du git d'avant le 15/02/2019, vous allez perdre votre dossier 2vs1
en récupérant toute version postérieure, il faut donc le re-télécharger si vous le voulez, voir
la fin de la partie INSTALLATION.

/!\------/!\


Le but de ce projet est d'afficher les logs des robots sur une vidéo qui a été prise en même temps
que les logs, ce qui permet d'avoir des "logs augmentés", et de mieux comprendre les logs par rapport
à ce qu'il s'est passé dans la vidéo, et donc le comportement des robots.
Il y a deux packages important dans ce projet : traitement et interface, qui devront fonctionner ensemble
dans le futur mais pour le moment nous avons des problèmes de fusion donc ils ne fonctionnent que chacun de leur côté.

# INSTALLATION

Pour l'installation, il y a plusieurs procédures à effectuer:

- D'abord installer pip si vous ne l'avez pas :
			sudo apt install pip
- catkin_tools par le biais de pip :
			pip install -U catkin_tools
- Cloner catkin dans workspace/src :
			cd workspace/src
			git clone https://github.com/ros/catkin.git
- Installer libprotobuf-dev et protobuf-compiler pour les protobuf :
			sudo apt install libprotobuf-dev
			sudo apt install protobuf-compiler
- Installer libjsconcpp-dev pour les json :
			sudo apt install libjsoncpp-dev
- Installer libtclap-dev pour les TCLAP :
			sudo apt install libtclap-dev
- Installer qtmultimedia5-dev pour faire marcher qt :
			sudo apt install qtmultimedia5-dev
- Installer libopencv-dev pour faire marcher opencv :
			sudo apt-get install libopencv-dev

Et normalement avec tout ça on a toutes les dépendances qu'il faut (il est possible
qu'il faille télécharger QT 5.9.5 par le site officiel : https://www.qt.io/download pour compiler l'interface).

On peut rajouter le fichier 2vs1 (qui contient les vidéos et les logs d'un match
2 robots vs 1 robot) dans le dépôt à côté de workspace pour lancer le côté traitement
par exemple, en le récupérant du google drive suivant : https://drive.google.com/drive/folders/1euuFUpFEmJQSxWQ8DGrLt1pGioKHVwfI?usp=sharing

A la source du projet on aura donc les dossiers suivant : 2vs1, rapports et workspace.



Maintenant reste seulement à se placer dans workspace et rentrer les commandes suivantes:

			catkin init
			catkin build

# COMPILATION

Pour compiler, il suffit de se placer dans le dossier workspace et d'écrire dans le terminal :

			catkin build

Si on veut l'exécutable d'un package, il faut aller dans workspace/build/<package> et changer la valeur du BUILD_TRAITEMENT_TOOLS:BOOL à ON.

Les exécutables se trouveront donc dans workspace/devel/lib/<package> .

# LANCEMENT TRAITEMENT

Pour lancer le traitement, il faut au préalable un dossier contenant des vidéos et
logs associés, voir la partie INSTALLATION pour télécharger le dossier 2vs1 contenant lesdites vidéos/logs sur le drive.

Après avoir passé le BUILD_TRAITEMENT_TOOLS:BOOL à ON dans workspace/build/traitement/CMakeCache.txt
et compilé avec catkin build dans workspace, placez vous dans le dossier 2vs1 et entrez la ligne suivante dans le terminal :

			../workspace/devel/lib/traitement/test_traitement -c replay.json -f eirlab.json -a 1 -a 1 -t 4

Le premier -a indique si vous souhaitez afficher la position des robots (-a 1 = ON, -a 0 = OFF)
Le deuxième -a indique si vous souhaitez afficher l'orientation des robots (-a 1 = ON, -a 0 = OFF)
Le -t indique si vous souhaitez afficher l'ancienne position d'un robot (-t <numero_du_robot>)

Même si trace_arg (l'argument -t) est définit comme MultiArg<int>, on ne peut pas afficher
plusieurs robots pour l'instant, nous verrons cette implémentation lorsque chaque robot aura
son objet (pour l'instant on définit qu'un robot, celui que l'on affiche).

# LANCEMENT INTERFACE

Après avoir passé le BUILD_TRAITEMENT_TOOLS:BOOL à ON dans workspace/build/interface/CMakeCache.txt
et compilé avec catkin build dans workspace, il n'y a pas d'arguments spéciaux pour lancer l'interface,
 vous n'avez qu'à lancer l'éxecutable qui se trouve au chemin suivant :

			workspace/devel/lib/interface/main

A noter que pour l'instant l'interface est juste un interface, il n'y pas d'images affichées.
