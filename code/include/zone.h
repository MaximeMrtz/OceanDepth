// Types et structure pour les zones de la carte OceanDepth
#ifndef OCEAN_DEPTH_ZONE_H
#define OCEAN_DEPTH_ZONE_H

#include "creature_types.h"

// Les différents types de zones sur la carte
typedef enum {
    ZONE_SURFACE,      // Zone de surface
    ZONE_RECIFS,       // Récifs
    ZONE_EPAVES,       // Épaves
    ZONE_FORET_ALGUES, // Forêt d'algues
    ZONE_GROTTES,      // Grottes
    ZONE_FOSSES,       // Fosses
    ZONE_VIDE,         // Vide
    ZONE_INCONNUE      // Inconnue
} TypeZone;

// Structure pour une zone de la carte
struct Zone {
    TypeZone type;
    char nom[50];
    int profondeur;
    int exploree;
    int debroquee;
    int nb_creatures;
    CreatureType type_creature;
    int boss_id;
};

typedef struct Zone Zone;

#endif // OCEAN_DEPTH_ZONE_H
