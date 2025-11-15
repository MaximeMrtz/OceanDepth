// Déclaration anticipée pour usage croisé
#ifndef INVENTAIRE_STRUCT_DECLARED
#define INVENTAIRE_STRUCT_DECLARED
typedef struct Inventaire Inventaire;
#endif

// Fichier pour gérer l'inventaire du joueur (structures et prototypes)
// Ici je mets tout ce qui sert à l'inventaire, la gestion des objets, l'équipement et l'utilisation d'objets

#ifndef INVENTAIRE_H
#define INVENTAIRE_H

#define MAX_OBJETS 8
#define MAX_NOM 30

//Types objets
typedef enum {
    TYPE_CONSOMMABLE,
    TYPE_EQUIPEMENT_HARPON,
    TYPE_EQUIPEMENT_COMBI
} TypeObjet;

//Structure objet
typedef struct {
    int id;
    char nom[MAX_NOM];
    TypeObjet type;
    int quantite;

    //Attributs consommables
    int restaure_pv;
    int restaure_oxygene;
    int reduit_fatigue;

    //Attributs équipements
    int attaque_min;
    int attaque_max;
    int defense;
    int cout_oxygene;

    int est_vide; //1 si l'emplacement est vide, 0 sinon
} Objet;



#include "zone.h"
struct Inventaire {
    Objet objets[MAX_OBJETS];
    int harpon_equipe; // index de l'objet équipé (-1 si aucun)
    int combi_equipee; // index de l'objet équipé (-1 si aucun)
    int perles;
    struct Zone* zone_actuelle;
    int boss_id;
};


/**
 * @brief Initialise l'inventaire du joueur.
 * @param inv Pointeur vers l'inventaire à initialiser
 */
void initialiser_inventaire(Inventaire *inv);

/**
 * @brief Affiche l'inventaire du joueur dans la console.
 * @param inv Pointeur vers l'inventaire à afficher
 */
void afficher_inventaire(Inventaire *inv);

/**
 * @brief Ajoute un objet à l'inventaire.
 * @param inv Pointeur vers l'inventaire
 * @param objet Objet à ajouter
 * @return 0 si succès, -1 sinon
 */
int ajouter_objet(Inventaire *inv, Objet objet);

/**
 * @brief Utilise un objet de l'inventaire.
 * @param inv Pointeur vers l'inventaire
 * @param index Index de l'objet à utiliser
 * @return 0 si succès, -1 sinon
 */
int utiliser_objet(Inventaire *inv, int index);

/**
 * @brief Équipe un objet (harpon ou combinaison).
 * @param inv Pointeur vers l'inventaire
 * @param index Index de l'objet à équiper
 * @return 0 si succès, -1 sinon
 */
int equiper_objet(Inventaire *inv, int index);

/**
 * @brief Ouvre le menu interactif d'inventaire.
 * @param inv Pointeur vers l'inventaire
 */
void ouvrir_inventaire(Inventaire *inv);

/**
 * @brief Crée une capsule d'oxygène.
 * @return Objet capsule d'oxygène
 */
Objet creer_capsule_oxygene();

/**
 * @brief Crée une trousse de soin.
 * @return Objet trousse de soin
 */
Objet creer_trousse_soin();

/**
 * @brief Crée un stimulant.
 * @return Objet stimulant
 */
Objet creer_stimulant();

/**
 * @brief Crée un antidote.
 * @return Objet antidote
 */
Objet creer_antidote();

/**
 * @brief Crée un harpon rouillé.
 * @return Objet harpon rouillé
 */
Objet creer_harpon_rouille();

/**
 * @brief Crée un harpon électrique.
 * @return Objet harpon électrique
 */
Objet creer_harpon_electrique();

/**
 * @brief Crée une combinaison néoprène.
 * @return Objet combinaison néoprène
 */
Objet creer_combi_neoprene();

/**
 * @brief Crée une combinaison titanium.
 * @return Objet combinaison titanium
 */
Objet creer_combi_titanium();

#endif

/**
 * @brief Libère la mémoire de l'inventaire (placeholder, rien à libérer actuellement).
 * @param inv Pointeur vers l'inventaire
 */
void liberer_inventaire(Inventaire *inv);