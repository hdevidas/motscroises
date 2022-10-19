# Mots_Croises

Pour compiler le mémoire au format latex, il faut se placer dans docs/report/memoire_tex puis réaliser la commande suivante : pdflatex -shell-escape main.tex. Autrement, le PDF compilé est disponible sous report/ au nom de "memoire.pdf".

## Table of Contents

1. Description générale
2. Auteurs
3. Installation


### Description générale

Ce projet est réalisé dans le cadre de l'UE de Projet de Programmation en M1 Informatique à l'université de Bordeaux. Le responsable est Monsieur Philippe Narbel, et le projet est suivi par
Madame Wahiba Larbi. Le projet ci-présent se présente comme un générateur de grilles de mots croisés avec interface utilisateur.

### Auteurs

Les auteurs de ce projet sont Clément Albanel, Hugo Devidas, Emma Auzi, Nathanael Alves, Yohan Bornes et Sylvain Coudougnan, tous issus du parcours Génie logiciel.

### Installation

Le projet nécéssite l'utilisation de l'API Qt pour la gestion de l'interface graphique. Celle-ci possède une version open-souce, que nous avons utilisée.

Pour installer Qt, compiler le projet et lancer l'application :

1)  Installation de Qt (Linux version - testé sur Ubuntu 20.04): sudo apt-get install qtbase5-dev && sudo apt-get install qtdeclarative5-dev && sudo apt install qttools5-dev

   - Lancement de l'application:
      1) cd src/build
      2) cmake ..
      3) make
      4) ./CrossedWord term test -p ../../data/patterns/easy/easy2.txt -bd ../../data/databases/word_database.txt (mode terminal)
      5) ./CrossedWord (mode graphique)

D'une manière générale, la commande pour tester en mode terminale est : ./CrossedWord term [easy/normal/hard/test/test_import_export] -p 'pattern_path' -bd 'bd_path

La commande pour lancer les tests unitaires (googletest) est : ./CrossedWordTest
