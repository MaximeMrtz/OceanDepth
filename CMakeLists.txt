cmake_minimum_required(VERSION 3.10)
project(OceanDepth C)

set(CMAKE_C_STANDARD 11)

include_directories(code/include)


add_executable(OceanDepth
        code/src/main.c
        code/src/carte.c
        code/src/carte_actions.c
        code/src/carte_utils.c
        code/src/combat.c
        code/src/creatures.c
        code/src/inventaire.c
        code/src/joueur.c
        #code/src/oceandepth.c
        code/src/quetes.c
        code/src/sauvegarde.c
)