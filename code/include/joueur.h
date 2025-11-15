
// Fichier pour gérer le joueur (Plongeur) d'OceanDepth
// Ici je mets la structure du joueur, la gestion de la fatigue, de l'oxygène, des dégâts et la sauvegarde/chargement du joueur

#ifndef OCEAN_DEPTH_JOUEUR_H
#define OCEAN_DEPTH_JOUEUR_H

#include <stdint.h>
#include <stdio.h>

typedef struct Plongeur {
    char nom[50];           // Nom du joueur
    char prenom[50];        // Prénom du joueur
    char date_creation[20]; // Date de création de la partie 
    int points_de_vie;
    int points_de_vie_max;
    int niveau_oxygene;
    int niveau_oxygene_max;
    int niveau_fatigue; // 0..5
    int perles;
    int tours_paralyse; // nombre de tours paralysé (réduit attaques)
    int defense; // bonus défensif (combinaison)
    int avancement;     // Pourcentage d'avancement du jeu (0-100)
    int temps_jeu;      // Temps de jeu en minutes
    int xp;            // Points d'expérience 
    int argent;        // Argent/Or collecté (alias pour perles)
    struct Inventaire* inventaire; // Pointeur vers l'inventaire du joueur
} Plongeur;


// Initialise la structure du joueur
Plongeur *initialiser_joueur(Plongeur *plongeur, const char *nom, const char *prenom, int points_de_vie, int points_de_vie_max, int niveau_oxygene, int niveau_oxygene_max, int niveau_fatigue, int perles);

// Gère la fatigue du joueur selon l'action
int gestion_fatigue(Plongeur *plongeur, int action);

int consommation_oxygene(Plongeur *plongeur, int action, int profondeur);

// Calcule la consommation d'oxygène selon l'action et la profondeur
int consommation_oxygene(Plongeur *plongeur, int action, int profondeur);

/**
 * @brief Calcule les dégâts infligés en fonction des paramètres.
 * @param attaque_min Attaque minimale
 * @param attaque_max Attaque maximale
 * @param defense_creature Défense de la créature
 * @param bonus_arme Bonus d'arme
 * @return Dégâts infligés
 */
int calcul_degats_variation(int attaque_min, int attaque_max, int defense_creature, int bonus_arme);

/**
 * @brief Sauvegarde le joueur dans un fichier.
 * @param out Fichier ouvert en écriture
 * @param plongeur Pointeur vers le joueur
 * @return 0 si succès, -1 sinon
 */
int sauvegarder_joueur(FILE *out, Plongeur *plongeur);

/**
 * @brief Charge le joueur depuis un fichier.
 * @param in Fichier ouvert en lecture
 * @param plongeur Pointeur vers le joueur
 * @return 0 si succès, -1 sinon
 */
int charger_joueur(FILE *in, Plongeur *plongeur);

/**
 * @brief Met à jour l'avancement du joueur selon les zones explorées.
 * @param plongeur Pointeur vers le joueur
 * @param zones_explorees Nombre de zones explorées
 * @param total_zones Nombre total de zones
 */
void mettre_a_jour_avancement(Plongeur *plongeur, int zones_explorees, int total_zones);

#endif // OCEAN_DEPTH_JOUEUR_H
