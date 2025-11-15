
// Fichier pour gérer les quêtes du jeu OceanDepth (structures et prototypes)
// Ici je mets tout ce qui sert aux quêtes, leur progression, affichage et complétion


#ifndef QUETES_H
#define QUETES_H

#include "joueur.h"
#include "carte.h"

typedef enum {
    QUETE_EXPLORATION_SURFACE,
    QUETE_DESCENDRE_PROFONDEUR,
    QUETE_COMBATTRE_CREATURE,
    QUETE_EXPLORER_EPAVE,
    QUETE_ATTEINDRE_ABYSSES,
    QUETE_BOSS_FINAL,
    NB_QUETES
} TypeQuete;

typedef enum {
    STATUT_NON_ACTIVE,
    STATUT_ACTIVE,
    STATUT_TERMINEE
} StatutQuete;

typedef struct {
    TypeQuete type;
    StatutQuete statut;
    char titre[100];
    char description[200];
    char objectif[150];
    int recompense_xp;
    int recompense_or;
    int objectif_actuel;
    int objectif_requis;
} Quete;

typedef struct {
    Quete quetes[NB_QUETES];
    int quete_active;
    int nb_quetes_terminees;
} SystemeQuetes;


// Initialise le système de quêtes
void initialiser_quetes(SystemeQuetes *systeme);


// Libère la mémoire du système de quêtes (pour plus tard si besoin)
void liberer_quetes(SystemeQuetes *systeme);
/**
 * @brief Vérifie la progression des quêtes en fonction de l'état du joueur et de la carte.
 * @param systeme Pointeur vers le système de quêtes
 * @param joueur Pointeur vers le joueur
 * @param carte Pointeur vers la carte
 */
void verifier_progression_quetes(SystemeQuetes *systeme, Plongeur *joueur, CarteOceanique *carte);

/**
 * @brief Affiche la quête active du joueur.
 * @param systeme Pointeur vers le système de quêtes
 */
void afficher_quete_active(SystemeQuetes *systeme);

/**
 * @brief Marque une quête comme complétée.
 * @param systeme Pointeur vers le système de quêtes
 * @param joueur Pointeur vers le joueur
 * @param type Type de quête à compléter
 */
void completer_quete(SystemeQuetes *systeme, Plongeur *joueur, TypeQuete type);

/**
 * @brief Indique si le jeu est terminé (toutes les quêtes principales accomplies).
 * @param systeme Pointeur vers le système de quêtes
 * @return 1 si terminé, 0 sinon
 */
int jeu_termine(SystemeQuetes *systeme);

#endif