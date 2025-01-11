# Projet C++ 24/25 – "Piece Out"

Il s'agit d'un projet du cours "IFEAY050 - Langages à Objet Avancés" pour l'année universitaire 2024-2025 à l'Université Paris Cité.

## Qu'est qu'il me faut pour jouer ?

Un ordinateur moderne, et une souris ! (mais il faut d'abord compiler la source...)

Le rapport PDF se trouve dans le même dossier.

## Comment compiler le projet ?

Il faut que vous avez déjà installé **GNU Make** et le compilateur **GCC**;
n'oubliez pas de les inclure dans votre variable système `PATH`.

Lorsque votre `g++` et `make` sont configurés dans votre terminal unix,
changez votre répertoire courant pour le dossier du projet par la commande
```cd $(YOUR_DIRECTORY)/ObjetAvancee```
et puis exécutez la commande `make` dans le terminal.

La bibliothèque `SFML` en version 2.6.1 est déjà incluse dans le projet.

Après, vous pouvez effacer tous les fichers générés par `make clean`.

## Comment executer le projet ?

Exécutez la commande `make test` dans le terminal depuis le dossier du projet,
ou trouver l'executable vous-même :)
