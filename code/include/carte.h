#include <stdio.h>
#include "creature_types.h"


// ... (définitions des structures)



// Fichier pour gérer la carte du jeu OceanDepth (structures et prototypes)
// Ici je mets tout ce qui sert à la carte, les déplacements, l'exploration, la sauvegarde et l'affichage des zones
// (version modulaire avec carte_utils.c et carte_actions.c)




#ifndef CARTE_H
#define CARTE_H



#include "inventaire.h"
#define TAILLE_CARTE 4


// Forward declarations pour éviter les dépendances circulaires
typedef struct Plongeur Plongeur;


typedef enum {
    DIR_NORD,
    DIR_SUD,
    DIR_EST,
    DIR_OUEST
} Direction;


#include "zone.h"

typedef struct {
    int x;
    int y;
    TypeZone zone_actuelle;
} PositionJoueur;


typedef struct {
    Zone grille[TAILLE_CARTE][TAILLE_CARTE];
    PositionJoueur position_joueur;
    int zones_decouvertes;
} CarteOceanique;

// Libère la mémoire de la carte (pour plus tard si besoin)
void liberer_carte(CarteOceanique *carte);

// Enumération pour le résultat du déplacement
typedef enum {
    DEPLACEMENT_OK,
    DEPLACEMENT_HORS_LIMITES,
    DEPLACEMENT_ZONE_BLOQUEE,
    DEPLACEMENT_EQUIPEMENT_REQUIS
} ResultatDeplacement;


/**
 * @brief Vérifie si le joueur peut se déplacer vers une case donnée.
 * @param carte Carte du jeu
 * @param joueur Pointeur vers le joueur
 * @param x Coordonnée X
 * @param y Coordonnée Y
 * @return 1 si possible, 0 sinon
 */
int peut_se_deplacer_vers(CarteOceanique *carte, Plongeur *joueur, int x, int y);

/**
 * @brief Effectue le déplacement du joueur sur la carte.
 * @param carte Carte du jeu
 * @param nouveau_x Nouvelle coordonnée X
 * @param nouveau_y Nouvelle coordonnée Y
 * @param plongeur Pointeur vers le joueur
 * @return Résultat du déplacement (enum)
 */
ResultatDeplacement se_deplacer(CarteOceanique *carte, int nouveau_x, int nouveau_y, Plongeur *plongeur);

/**
 * @brief Sauvegarde la carte dans un fichier texte.
 * @param fichier Fichier ouvert en écriture
 * @param carte Carte à sauvegarder
 * @return 0 si succès, -1 sinon
 */
int sauvegarder_carte(FILE *fichier, CarteOceanique *carte);

/**
 * @brief Charge la carte depuis un fichier texte.
 * @param fichier Fichier ouvert en lecture
 * @param carte Carte à charger
 * @return 0 si succès, -1 sinon
 */
int charger_carte(FILE *fichier, CarteOceanique *carte);

/**
 * @brief Initialise la carte et la position du joueur.
 * @param carte Carte à initialiser
 * @param joueur Pointeur vers le joueur
 * @return 0 si succès, -1 sinon
 */
int initialiser_carte(CarteOceanique *carte, Plongeur *joueur);

/**
 * @brief Affiche la carte complète dans la console.
 * @param carte Carte à afficher
 */
void afficher_carte_complete(CarteOceanique *carte);

/**
 * @brief Retourne la zone actuelle du joueur.
 * @param carte Carte du jeu
 * @return Pointeur vers la zone actuelle
 */
Zone* get_zone_actuelle(CarteOceanique *carte);

/**
 * @brief Débloque les zones adjacentes à une position donnée.
 * @param carte Carte du jeu
 * @param x Coordonnée X
 * @param y Coordonnée Y
 */
void debloquer_zones_adjacentes(CarteOceanique *carte, int x, int y);

/**
 * @brief Retourne le nom d'un type de zone.
 * @param type Type de zone
 * @return Chaîne de caractères (const)
 */
const char* nom_type_zone(TypeZone type);

/**
 * @brief Retourne le symbole d'un type de zone.
 * @param type Type de zone
 * @return Chaîne de caractères (const)
 */
const char* symbole_zone(TypeZone type);

/**
 * @brief Affiche les créatures présentes dans la zone actuelle.
 * @param carte Carte du jeu
 */
void afficher_creatures_zone(CarteOceanique *carte);

/**
 * @brief Déplace le joueur dans une direction donnée.
 * @param carte Carte du jeu
 * @param direction Direction à prendre
 * @param joueur Pointeur vers le joueur
 * @return 0 si succès, -1 sinon
 */
int deplacer_joueur(CarteOceanique *carte, Direction direction, Plongeur *joueur);

/**
 * @brief Permet d'explorer la zone actuelle.
 * @param carte Carte du jeu
 * @param joueur Pointeur vers le joueur
 * @return 0 si succès, -1 sinon
 */
int explorer_zone(CarteOceanique *carte, Plongeur *joueur);

/**
 * @brief Lance un combat contre les créatures de la zone.
 * @param carte Carte du jeu
 * @param joueur Pointeur vers le joueur
 * @param inventaire Pointeur vers l'inventaire
 * @return 0 si succès, -1 sinon
 */
int combattre_creatures_zone(CarteOceanique *carte, Plongeur *joueur, Inventaire *inventaire);

/**
 * @brief Permet au joueur de récupérer de l'oxygène à la surface.
 * @param carte Carte du jeu
 * @param joueur Pointeur vers le joueur
 * @return 0 si succès, -1 sinon
 */
int recuperer_oxygene(CarteOceanique *carte, Plongeur *joueur);

/**
 * @brief Affiche les déplacements possibles depuis la position actuelle.
 * @param carte Carte du jeu
 */
void afficher_deplacements_possibles(CarteOceanique *carte);

#endif
#ifdef __cplusplus
}
#endif