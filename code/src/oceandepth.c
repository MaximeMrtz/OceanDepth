/* oceandepth.c - JEU PRINCIPAL
 * Jeu d'exploration sous-marine - Version simplifiee et fonctionnelle
 */


// Inclusions standards
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Inclusions du projet
#include "carte.h"
#include "joueur.h"
#include "creatures.h"
#include "combat.h"
#include "quetes.h"
#include "inventaire.h"

// ============================================================================
// FONCTIONS MENU ET INTERFACE
// ============================================================================

void afficher_menu() {
    printf("\n=== OCEANDEPTH - MENU PRINCIPAL ===\n");
    printf("1. Avancer (choisir direction)\n");
    printf("2. Explorer la zone actuelle\n");
    printf("3. Combat avec creatures\n");
    printf("4. Statut du joueur\n");
    printf("5. Creatures presentes\n");
    printf("6. Quete active\n");
    printf("7. Ouvrir inventaire\n");
    printf("8. Sauvegarder\n");
    printf("9. Charger\n");
    printf("0. Quitter\n");
    printf("Votre choix: ");
}

void afficher_statut_complet(Plongeur *joueur, Inventaire *inv) {
    printf("\n=== %s %s ===\n", joueur->prenom, joueur->nom);
    printf("PV: %d/%d | O2: %d/%d | Fatigue: %d/5\n",
           joueur->points_de_vie, joueur->points_de_vie_max,
           joueur->niveau_oxygene, joueur->niveau_oxygene_max,
           joueur->niveau_fatigue);

    printf("Defense: %d", joueur->defense);
    if (inv->combi_equipee >= 0) {
        printf(" (+%d)", inv->objets[inv->combi_equipee].defense);
    }
    printf("\n");

    printf("Perles: %d | XP: %d | Avancement: %d%%\n",
           joueur->perles, joueur->xp, joueur->avancement);

    if (inv->harpon_equipe >= 0) {
        Objet *harpon = &inv->objets[inv->harpon_equipe];
        printf("Arme: %s (ATK: %d-%d)\n", harpon->nom, harpon->attaque_min, harpon->attaque_max);
    }
}

// ============================================================================
// FONCTIONS DEPLACEMENT SIMPLIFIEES
// ============================================================================

void gerer_avancement_simple(CarteOceanique *carte, Plongeur *joueur) {
    printf("\nAvancement - Position: (%d,%d)\n", carte->position_joueur.x, carte->position_joueur.y);
    printf("1 Haut  2 Bas  3 Gauche  4 Droite\n");
    printf("Choix: ");

    char buffer[10];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return;

    int direction = atoi(buffer);
    int new_x = carte->position_joueur.x;
    int new_y = carte->position_joueur.y;

    switch (direction) {
        case 1: new_y--; break;  // Haut
        case 2: new_y++; break;  // Bas
        case 3: new_x--; break;  // Gauche
        case 4: new_x++; break;  // Droite
        default: printf("Direction invalide!\n"); return;
    }

    // Verifier les limites
    if (new_x < 0 || new_x >= TAILLE_CARTE || new_y < 0 || new_y >= TAILLE_CARTE) {
        printf("Limite de la carte atteinte!\n");
        return;
    }

    Zone *zone_cible = &carte->grille[new_y][new_x];

    // Verifier si accessible
    if (!zone_cible->debroquee) {
        printf("Zone non accessible! Explorez d'abord les zones adjacentes.\n");
        return;
    }

    // Effectuer le deplacement
    carte->position_joueur.x = new_x;
    carte->position_joueur.y = new_y;
    printf("Deplacement reussi vers (%d,%d)!\n", new_x, new_y);
    printf("Zone: %s (Profondeur: %dm)\n", nom_type_zone(zone_cible->type), zone_cible->profondeur);
}

void explorer_zone_simple(CarteOceanique *carte, Plongeur *joueur) {
    Zone *zone = get_zone_actuelle(carte);
    if (!zone) return;

    if (zone->exploree) {
        printf("Zone deja exploree: %s\n", nom_type_zone(zone->type));
        return;
    }

    zone->exploree = 1;
    printf("Exploration de %s terminee!\n", nom_type_zone(zone->type));
    printf("Nouvelles zones debloquees!\n");

    // Debloquer zones adjacentes
    debloquer_zones_adjacentes(carte, carte->position_joueur.x, carte->position_joueur.y);

    // Recompense exploration
    joueur->xp += 25;
    joueur->perles += 10;
    printf("+25 XP, +10 perles\n");

    // Mettre a jour avancement
    int zones_explorees = 0;
    for (int y = 0; y < TAILLE_CARTE; y++) {
        for (int x = 0; x < TAILLE_CARTE; x++) {
            if (carte->grille[y][x].exploree) zones_explorees++;
        }
    }
    joueur->avancement = (zones_explorees * 100) / (TAILLE_CARTE * TAILLE_CARTE);
}

void combat_simple(CarteOceanique *carte, Plongeur *joueur, Inventaire *inv) {
    Zone *zone = get_zone_actuelle(carte);
    if (!zone || zone->nb_creatures <= 0) {
        printf("Aucune creature hostile dans cette zone.\n");
        return;
    }

    printf("Un ennemi vous attaque!\n");

    // Stats de base
    int attaque_joueur = 10;
    int defense_joueur = joueur->defense;

    // Bonus equipement
    if (inv->harpon_equipe >= 0) {
        attaque_joueur = inv->objets[inv->harpon_equipe].attaque_max;
    }
    if (inv->combi_equipee >= 0) {
        defense_joueur += inv->objets[inv->combi_equipee].defense;
    }

    // Ennemi simple
    int pv_ennemi = 30 + (zone->profondeur / 10);
    int attaque_ennemi = 8 + (zone->profondeur / 20);

    printf("Ennemi: %d PV, %d ATK\n", pv_ennemi, attaque_ennemi);
    printf("Vous: %d PV, %d ATK, %d DEF\n", joueur->points_de_vie, attaque_joueur, defense_joueur);

    while (joueur->points_de_vie > 0 && pv_ennemi > 0) {
        // Attaque du joueur
        int degats = attaque_joueur;
        pv_ennemi -= degats;
        printf("Vous attaquez: -%d PV (reste: %d)\n", degats, pv_ennemi);

        if (pv_ennemi <= 0) {
            printf("Victoire! +50 XP, +25 perles\n");
            joueur->xp += 50;
            joueur->perles += 25;
            zone->nb_creatures--;
            return;
        }

        // Attaque de l'ennemi
        int degats_recus = attaque_ennemi - defense_joueur;
        if (degats_recus < 1) degats_recus = 1;
        joueur->points_de_vie -= degats_recus;
        printf("Vous subissez: -%d PV (reste: %d)\n", degats_recus, joueur->points_de_vie);
    }

    if (joueur->points_de_vie <= 0) {
        printf("Defaite! Retour a la base...\n");
        joueur->points_de_vie = joueur->points_de_vie_max / 2;
        carte->position_joueur.x = 0;
        carte->position_joueur.y = 0;
    }
}

// ============================================================================
// FONCTION MAIN SIMPLIFIEE
// ============================================================================

int main() {
    srand(time(NULL));

    printf("=== OCEANDEPTH - VERSION SIMPLIFIEE ===\n");
    printf("Jeu d'exploration sous-marine\n\n");

    // Initialiser le jeu
    CarteOceanique carte;
    Plongeur joueur;
    CreatureList creatures;
    SystemeQuetes quetes;
    Inventaire inventaire;

    // Nouvelle partie toujours
    char nom[50], prenom[50];
    printf("Prenom: ");
    if (fgets(prenom, sizeof(prenom), stdin)) {
        prenom[strcspn(prenom, "\n")] = 0;
    }
    printf("Nom: ");
    if (fgets(nom, sizeof(nom), stdin)) {
        nom[strcspn(nom, "\n")] = 0;
    }

    // Initialiser tout
    initialiser_joueur(&joueur, nom, prenom, 100, 100, 80, 80, 0, 50);
    creatures_init(&creatures);
    initialiser_inventaire(&inventaire);
    initialiser_quetes(&quetes);

    if (initialiser_carte(&carte, &joueur) != 0) {
        printf("Erreur d'initialisation!\n");
        return 1;
    }

    // Equipement de depart
    ajouter_objet(&inventaire, creer_harpon_rouille());
    ajouter_objet(&inventaire, creer_combi_neoprene());
    ajouter_objet(&inventaire, creer_capsule_oxygene());
    ajouter_objet(&inventaire, creer_trousse_soin());

    printf("Jeu initialise! Position: Base sous-marine\n");
    printf("Equipement de depart ajoute!\n\n");

    // Boucle de jeu
    int continuer = 1;
    while (continuer) {
        // Afficher carte
        afficher_carte_complete(&carte);

        // Verifier quetes
        verifier_progression_quetes(&quetes, &joueur, &carte);
        if (jeu_termine(&quetes)) {
            printf("VICTOIRE TOTALE! Felicitations!\n");
            break;
        }

        // Menu
        afficher_menu();

        char buffer[10];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue;
        int choix = atoi(buffer);

        switch (choix) {
            case 1:
                gerer_avancement_simple(&carte, &joueur);
                break;
            case 2:
                explorer_zone_simple(&carte, &joueur);
                break;
            case 3:
                combat_simple(&carte, &joueur, &inventaire);
                break;
            case 4:
                afficher_statut_complet(&joueur, &inventaire);
                printf("\nAppuyez sur Entree...");
                getchar();
                break;
            case 5:
                afficher_creatures_zone(&carte);
                printf("\nAppuyez sur Entree...");
                getchar();
                break;
            case 6:
                afficher_quete_active(&quetes);
                printf("\nAppuyez sur Entree...");
                getchar();
                break;
            case 7:
                ouvrir_inventaire(&inventaire);
                break;
            case 8:
                printf("Sauvegarde... (fonction pas encore implementee)\n");
                break;
            case 9:
                printf("Chargement... (fonction pas encore implementee)\n");
                break;
            case 0:
                printf("Au revoir! Merci d'avoir joue!\n");
                continuer = 0;
                break;
            default:
                printf("Choix invalide (0-9)\n");
                break;
        }

        // Gestion de l'oxygene
        Zone *zone_actuelle = get_zone_actuelle(&carte);
        if (zone_actuelle && zone_actuelle->profondeur > 0) {
            joueur.niveau_oxygene -= 1;
            if (joueur.niveau_oxygene < 10) {
                printf("Oxygene faible: %d/%d\n", joueur.niveau_oxygene, joueur.niveau_oxygene_max);
            }
            if (joueur.niveau_oxygene <= 0) {
                printf("Manque d'oxygene! -10 PV\n");
                joueur.points_de_vie -= 10;
                joueur.niveau_oxygene = 0;
            }
        }

        // Game over
        if (joueur.points_de_vie <= 0) {
            printf("GAME OVER - Votre aventure se termine ici...\n");
            break;
        }
    }

    printf("\nStatistiques finales:\n");
    printf("%s %s\n", joueur.prenom, joueur.nom);
    printf("XP: %d | Perles: %d | Avancement: %d%%\n",
           joueur.xp, joueur.perles, joueur.avancement);
    
    return 0;
}