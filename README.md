

# OceanDepth

## C'est quoi ce projet ?

C'est un jeu d'exploration sous-marine en C que j'ai codé pour m'entraîner et m'amuser. On explore une carte, on affronte des créatures, on gère son inventaire, on fait des quêtes, etc.

## Les exécutables

- **oceandepth_enhanced.exe** : la version finale complète du jeu
- **oceandepth_explorer.exe** : version où tu peux explorer la carte à la main
- **test_carte.exe** : pour tester juste la carte

## Compiler le projet

Pour compiler vite fait :
```sh
gcc -o oceandepth_final.exe test_carte.c carte.c combat.c creatures.c joueur.c sauvegarde.c inventaire.c
```
Sinon tu peux utiliser le Makefile dans le dossier `code/`.

## Tester les fuites mémoire (Valgrind)

1. Installe WSL (Ubuntu) si tu l'as pas déjà.
2. Ouvre Ubuntu et tape :
    ```sh
    sudo apt update
    sudo apt install build-essential make valgrind
    ```
3. Va dans le dossier du projet :
    ```sh
    cd /mnt/c/Users/roiss/OceanDepth/code
    ```
4. Compile sous Linux :
    ```sh
    make clean
    make
    ```
5. Lance le jeu avec Valgrind :
    ```sh
    valgrind --leak-check=full ./oceandepth_modular.exe
    ```
6. Joue un peu puis quitte pour voir le rapport mémoire.

Si tu vois :
```
All heap blocks were freed -- no leaks are possible
```
Ça veut dire qu'il n'y a aucune fuite mémoire !

## Lancer la démo

```sh
./oceandepth_final.exe
```

## Organisation du dossier

OceanDepth/
├──  FICHIERS DE JEU
│   ├── test_carte.c           → Fichier principal
│   ├── oceandepth_final.exe   → Exécutable final
│   └── partie_oceandepth.save → Sauvegarde
│
├──  MODULES C
│   ├── carte.c / carte.h      → Cartographie
│   ├── combat.c / combat.h    → Combat
│   ├── creatures.c / creatures.h → Créatures
│   ├── joueur.c / joueur.h    → Joueur
│   ├── sauvegarde.c           → Sauvegarde
│   └── inventaire.c           → Inventaire
│
├──  include/                  → Tous les .h du projet
├──  src/                      → Tous les .c du projet
├──  saves/                    → Sauvegardes
├──  README.md                 → Ce fichier

## Astuces

- Si tu veux tout nettoyer : `make clean`
- Si tu veux recompiler : `make`
- Si tu veux juste tester une partie du code, tu peux compiler un seul .c

Voilà, amuse-toi bien avec OceanDepth !
