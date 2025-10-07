cmake_minimum_required(VERSION 4.0)
project(OceanDepth C)

set(CMAKE_C_STANDARD 11)

add_executable(OceanDepth main.c
        creatures.c
        combat.c
        inventaire.c
        carte.c
        sauvegarde.c
        joueur.c
)
