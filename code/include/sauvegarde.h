
// Fichier pour sauvegarder et charger la partie OceanDepth (prototypes)
// Ici je mets tout ce qui sert à sauvegarder et charger la partie ou ses composants (joueur, carte, quêtes, créatures, inventaire)

#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include "joueur.h"
#include "carte.h"
#include "quetes.h"
#include "creatures.h"


// Sauvegarde toute la partie (joueur, carte, créatures, quêtes, inventaire)
void sauvegarder_partie_complete(CarteOceanique *carte, Plongeur *joueur, CreatureList *creatures, SystemeQuetes *quetes, Inventaire *inv);

// Charge toute la partie (joueur, carte, créatures, quêtes, inventaire)
int charger_partie_complete(CarteOceanique *carte, Plongeur *joueur, CreatureList *creatures, SystemeQuetes *quetes, Inventaire *inv);

// Forward declarations
typedef struct InventaireJoueur InventaireJoueur;
// typedef struct SystemeQuetes SystemeQuetes; // déjà défini dans quetes.h


// Sauvegarde juste le joueur et la carte
int sauvegarder_partie(Plongeur *joueur, CarteOceanique *carte);

// Charge juste le joueur et la carte
int charger_partie(Plongeur *joueur, CarteOceanique *carte);


/**
 * @brief Sauvegarde le joueur dans un fichier.
 * @param fichier Fichier ouvert en écriture
 * @param joueur Pointeur vers le joueur
 * @return 0 si succès, -1 sinon
 */
int sauvegarder_joueur(FILE *fichier, Plongeur *joueur);

/**
 * @brief Charge le joueur depuis un fichier.
 * @param fichier Fichier ouvert en lecture
 * @param joueur Pointeur vers le joueur
 * @return 0 si succès, -1 sinon
 */
int charger_joueur(FILE *fichier, Plongeur *joueur);

/**
 * @brief Sauvegarde la carte dans un fichier (y compris les noms de zones).
 * @param fichier Fichier ouvert en écriture
 * @param carte Pointeur vers la carte
 * @return 0 si succès, -1 sinon
 */
int sauvegarder_carte(FILE *fichier, CarteOceanique *carte);

/**
 * @brief Charge la carte depuis un fichier (y compris les noms de zones).
 * @param fichier Fichier ouvert en lecture
 * @param carte Pointeur vers la carte
 * @return 0 si succès, -1 sinon
 */
int charger_carte(FILE *fichier, CarteOceanique *carte);

#endif