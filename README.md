### ANNOTATIONS-ROBOT

Le but de ce projet est d'afficher les logs des robots sur une vidéo qui a été prise en même temps que les logs, ce qui permet d'avoir des "logs augmentés", et de mieux comprendre les logs par rapport à ce qu'il s'est passé dans la vidéo, et donc le comportement des robots.
Il y a deux packages important dans ce projet : traitement et interface, qui devront fonctionner ensemble dans le futur mais pour le moment nous avons des problèmes de fusion donc ils ne fonctionnt que chacun de leur côté.

# INSTALLATION

Pour l'installation, il faut plusieurs libs dont :

**** à faire ******

# COMPILATION

Pour compiler, il suffit de se placer dans le dossier workspace et d'écrire dans le terminal :

catkin build

Si on veut l'exécutable d'un package, il faut aller dans workspace/build/<package> et changer

**** à faire *****

# LANCEMENT TRAITEMENT

Après avoir passé le BUILD_TRAITEMENT_TOOLS:BOOL à ON dans workspace/build/traitement/CMakeCache.txt et compilé avec catkin build dans workspace, placez vous dans le dossier 2vs1 et entrez la ligne suivante dans le terminal :

../workspace/devel/lib/traitement/test_traitement -c replay.json -f eirlab.json -a 1 -a 1 -t 4

Le premier -a indique si vous souhaitez afficher la position des robots (-a 1 = ON, -a 0 = OFF)
Le deuxième -a indique si vous souhaitez afficher l'orientation des robots (-a 1 = ON, -a 0 = OFF)
Le premier -t indique si vous souhaitez afficher les anciennes positions des robots (-t <nb robot>)

# LANCEMENT INTERFACE

Après avoir passé le BUILD_TRAITEMENT_TOOLS:BOOL à ON dans workspace/build/interface/CMakeCache.txt et compilé avec catkin build dans workspace, il n'y a pas d'arguments spéciaux pour lancer l'interface, vous n'avez qu'à lancer l'éxecutable qui se trouve au chemin suivant :

workspace/devel/lib/interface/main


