

// Fichier pour gérer le système de combat d'OceanDepth
// Ici je mets tout ce qui sert pour les combats entre le joueur et les créatures

#ifndef OCEAN_DEPTH_COMBAT_H
#define OCEAN_DEPTH_COMBAT_H



#ifdef __cplusplus
extern "C" {
#endif

// Déclarations anticipées pour éviter les dépendances croisées
struct Plongeur;
struct Inventaire;

#include "creatures.h"

void donner_recompenses_victoire(struct Plongeur *plongeur, struct Inventaire *inv, int difficulte);
void combat(struct Plongeur *joueur, Creature *creature, struct Inventaire *inv);

#ifdef __cplusplus
}
#endif
#include "creatures.h"


// Lance un tour de combat entre le joueur et les créatures
void tour_de_combat(CreatureList *liste, struct Plongeur *plongeur, int profondeur);

// Gère la phase d'attaque des créatures contre le joueur
int phase_attaque_creatures(struct Plongeur *plongeur, CreatureList *creatures);

#endif /* OCEAN_DEPTH_COMBAT_H */