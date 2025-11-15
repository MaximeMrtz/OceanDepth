#include "creature_types.h"

// Prototype pour l'emoji des créatures
const char* emoji_creature(CreatureType type);
#ifdef __cplusplus
extern "C" {
#endif

// ...existing code...



#ifdef __cplusplus
}
#endif

// Fichier pour gérer les créatures du jeu (types, structures, fonctions)
// Ici je mets tout ce qui sert à créer, gérer, sauvegarder/charger et afficher les créatures du jeu

#ifndef OCEAN_DEPTH_CREATURES_H
#define OCEAN_DEPTH_CREATURES_H

#include <stdio.h>
#include <stdint.h>
#include "carte.h"

typedef uint32_t CreatureId;


#include "creature_types.h"

typedef struct Creature {
    CreatureId id;
    CreatureType type;
    int x,y;    // position
    int hp,max_hp;
    int attack,defence;
    int alive;  // 0/1
    struct Creature *next;
} Creature;

typedef struct CreatureList { Creature *head; CreatureId next_id; } CreatureList;

// Structure pour les créatures de combat
typedef struct {
    char nom[50];
    int points_de_vie;
    int attaque_min;
    int attaque_max;
    int defense;
    CreatureType type;
} CreatureInfo;




/**
 * @brief Initialise la liste de créatures.
 * @param list Pointeur vers la liste à initialiser
 */
void creatures_init(CreatureList *list);

/**
 * @brief Crée une nouvelle créature et l'ajoute à la liste.
 * @param list Pointeur vers la liste de créatures
 * @param type Type de créature
 * @param x Position X
 * @param y Position Y
 * @return Pointeur vers la créature créée
 */
Creature *creature_new(CreatureList *list, CreatureType type, int x, int y);

/**
 * @brief Libère une créature de la liste par son identifiant.
 * @param list Pointeur vers la liste de créatures
 * @param id Identifiant de la créature à libérer
 */
void creature_free(CreatureList *list, CreatureId id);

/**
 * @brief Récupère une créature par son identifiant.
 * @param list Pointeur vers la liste de créatures
 * @param id Identifiant de la créature
 * @return Pointeur vers la créature trouvée ou NULL
 */
Creature *creature_get_by_id(CreatureList *list, CreatureId id);

/**
 * @brief Récupère une créature à une position donnée.
 * @param list Pointeur vers la liste de créatures
 * @param x Position X
 * @param y Position Y
 * @return Pointeur vers la créature trouvée ou NULL
 */
Creature *creature_at(CreatureList *list, int x, int y);

/**
 * @brief Déplace une créature vers une nouvelle position.
 * @param c Pointeur vers la créature
 * @param new_x Nouvelle position X
 * @param new_y Nouvelle position Y
 */
void creature_move(Creature *c, int new_x, int new_y);

/**
 * @brief Inflige des dégâts à une créature.
 * @param c Pointeur vers la créature
 * @param dmg Points de dégâts
 * @return Points de vie restants
 */
int creature_damage(Creature *c, int dmg);

/**
 * @brief Effectue une attaque entre deux créatures.
 * @param attacker Pointeur vers l'attaquant
 * @param defender Pointeur vers le défenseur
 * @return 1 si touche, 0 sinon
 */
int creature_hit(Creature *attacker, Creature *defender);

/**
 * @brief Libère toute la mémoire de la liste de créatures.
 * @param list Pointeur vers la liste de créatures
 */
void creatures_clear(CreatureList *list);

/**
 * @brief Sauvegarde la liste de créatures dans un fichier.
 * @param out Fichier ouvert en écriture
 * @param list Pointeur vers la liste de créatures
 * @return 0 si succès, -1 sinon
 */
int creatures_save(FILE *out, CreatureList *list);

/**
 * @brief Charge la liste de créatures depuis un fichier.
 * @param in Fichier ouvert en lecture
 * @param list Pointeur vers la liste de créatures
 * @return 0 si succès, -1 sinon
 */
int creatures_load(FILE *in, CreatureList *list);

/**
 * @brief Génère une créature pour une zone donnée.
 * @param creature Pointeur vers la structure de créature à remplir
 * @param zone Pointeur vers la zone
 */
void generer_creature_pour_zone(CreatureInfo *creature, Zone *zone);

/**
 * @brief Affiche les créatures présentes dans la zone actuelle de la carte.
 * @param carte Pointeur vers la carte océanique
 */
void afficher_creatures_zone(CarteOceanique *carte);

/* Note: plus d'aliases — utiliser les nouveaux noms publics. */

#endif // OCEAN_DEPTH_CREATURES_H
