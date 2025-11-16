// Inclusions standards
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Inclusions du projet
#include "joueur.h"
#include "carte.h"
#include "inventaire.h"
#include "quetes.h"
#include "creatures.h"
#include "combat.h"
#include "sauvegarde.h"

#define MAX_INPUT 100

// Variable globale pour le joueur (pour acces partout)
Plongeur joueur;
// Fonction pour lire un entier entre par l'utilisateur
int lire_entier(const char* prompt) {
    char buffer[MAX_INPUT];
    printf("%s", prompt);

    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin)) {
            char *endptr;
            int result = strtol(buffer, &endptr, 10);

            // Je verifie si la conversion est bonne
            if (endptr != buffer && (*endptr == '\n' || *endptr == '\0')) {
                return result;
            }
        }
        printf("Entree invalide. Veuillez saisir un nombre: ");
    }
}

// Fonction pour afficher le menu principal
void afficher_menu_principal() {
    printf("\n+==========================================+\n");
    printf("|          OCEAN DEPTH                     |\n");
    printf("|      Exploration des Profondeurs        |\n");
    printf("|  Joueur : %s\n", joueur.nom);
    printf("+==========================================+\n");
    printf("|  1. Voir la carte                        |\n");
    printf("|  2. Se deplacer                          |\n");
    printf("|  3. Explorer la zone                     |\n");
    printf("|  4. Combattre les creatures              |\n");
    printf("|  5. Inventaire                           |\n");
    printf("|  6. Quetes                               |\n");
    printf("|  7. Statut du joueur                     |\n");
    printf("|  8. Recuperer oxygene (surface)          |\n");
    printf("|  9. Sauvegarder                          |\n");
    printf("| 10. Charger sauvegarde                   |\n");
    printf("|  0. Quitter                              |\n");
    printf("+==========================================+\n");
}

// Fonction pour afficher le menu de deplacement
Direction choisir_direction() {
    printf("\n=== CHOISIR UNE DIRECTION ===\n");
    printf("1. NORD\n");
    printf("2. SUD\n");
    printf("3. EST\n");
    printf("4. OUEST\n");
    printf("0. Retour\n");

    int choix = lire_entier("Votre choix: ");

    switch (choix) {
        case 1: return DIR_NORD;
        case 2: return DIR_SUD;
        case 3: return DIR_EST;
        case 4: return DIR_OUEST;
    default: return -1; // Retour
    }
}

// Fonction pour afficher les infos du joueur
void afficher_statut_joueur(Plongeur *joueur, CarteOceanique *carte) {
    Zone *zone_actuelle = get_zone_actuelle(carte);

    printf("\n=== STATUT DU PLONGEUR ===\n");
    printf("Nom: %s\n", joueur->nom);
    printf("Sante: %d/%d\n", joueur->points_de_vie, joueur->points_de_vie_max);
    printf("Oxygene: %d/%d\n", joueur->niveau_oxygene, joueur->niveau_oxygene_max);
    printf("Experience: %d XP\n", joueur->xp);
    printf("Argent: %d pieces\n", joueur->argent);

    if (zone_actuelle) {
        printf("\n=== POSITION ACTUELLE ===\n");
        printf("Position: (%d,%d)\n", carte->position_joueur.x, carte->position_joueur.y);
        printf("Zone: %s\n", zone_actuelle->nom);
        printf("Profondeur: %dm\n", zone_actuelle->profondeur);
        printf("Exploration: %s\n", zone_actuelle->exploree ? "Exploree" : "Non exploree");
    }

    printf("\n=== PROGRESSION ===\n");
    printf("Zones decouvertes: %d/16\n", carte->zones_decouvertes);
    printf("Quetes actives: Voir menu quetes\n");
}

/* Boucle principale du jeu */
void boucle_principale() {

    // Initialisation
    srand(time(NULL));
    // joueur est maintenant global
    CarteOceanique carte;
    SystemeQuetes systeme_quetes;
    Inventaire inventaire;

    joueur.inventaire = (Inventaire*)&inventaire;

    // Menu de chargement si sauvegarde presente
    FILE *f = fopen("partie_oceandepth.save", "r");
    int chargee = 0;
    if (f) {
        fclose(f);
        printf("\nUne sauvegarde a ete detectee !\n");
        printf("1. Charger la sauvegarde\n2. Nouvelle partie\n");
        int choix = lire_entier("Votre choix : ");
        if (choix == 1) {
            if (charger_partie_complete(&carte, &joueur, NULL, &systeme_quetes, &inventaire) == 0) {
                printf("Sauvegarde chargee !\n");
                // Correction position joueur si hors limites
                if (carte.position_joueur.x < 0 || carte.position_joueur.x >= TAILLE_CARTE) carte.position_joueur.x = 0;
                if (carte.position_joueur.y < 0 || carte.position_joueur.y >= TAILLE_CARTE) carte.position_joueur.y = 0;
                // Affichage recapitulatif
                printf("\n--- Recapitulatif de la sauvegarde chargee ---\n");
                afficher_statut_joueur(&joueur, &carte); // Utilise la globale
                printf("\nInventaire :\n");
                afficher_inventaire(&inventaire);
                printf("\nQuetes :\n");
                afficher_quete_active(&systeme_quetes);
                printf("---------------------------------------------\n");
                chargee = 1;
            } else {
                printf("Erreur lors du chargement, nouvelle partie lancee.\n");
            }
        }
    }

    if (!chargee) {
        // Je cree le joueur
        printf("Bienvenue dans Ocean Depth!\n");
        printf("Entrez le nom de votre plongeur: ");
        fgets(joueur.nom, sizeof(joueur.nom), stdin);
        // J'enleve le retour a la ligne a la fin du nom
        joueur.nom[strcspn(joueur.nom, "\n")] = 0;

        // J'initialise le joueur
        if (initialiser_joueur(&joueur, joueur.nom, "", 100, 100, 100, 100, 0, 0) == NULL) {
            printf("Erreur lors de l'initialisation du joueur!\n");
            return;
        }

        // J'initialise les systemes du jeu
        if (initialiser_carte(&carte, &joueur) != 0) {
            printf("Erreur lors de l'initialisation de la carte!\n");
            return;
        }

        initialiser_quetes(&systeme_quetes);
        printf("Quetes initialisees!\n");

        initialiser_inventaire(&inventaire);
        printf("Inventaire initialise!\n");

        printf("\nInitialisation terminee!\n");
        printf("Vous commencez a la base sous-marine.\n");
    }

    // Je lance la boucle principale du jeu
    int continuer = 1;
    while (continuer) {
    // Je fais les verifications de base
        if (joueur.points_de_vie <= 0) {
            printf("\nGAME OVER! Votre sante est epuisee.\n");
            printf("Retour a la base pour recuperation...\n");
            joueur.points_de_vie = 50;
            carte.position_joueur.x = 0;
            carte.position_joueur.y = 0;
        }

        if (joueur.niveau_oxygene <= 0) {
            printf("\nATTENTION! Votre oxygene est epuise!\n");
            printf("Retour automatique en surface!\n");
            joueur.niveau_oxygene = 20;
            joueur.points_de_vie -= 10;
            carte.position_joueur.x = 0;
            carte.position_joueur.y = 0;
        }

    // J'affiche le menu
        afficher_menu_principal();

        int choix = lire_entier("Votre choix: ");

        switch (choix) {
            case 1: // Voir la carte
                afficher_carte_complete(&carte);
                afficher_deplacements_possibles(&carte);
                break;

            case 2: { // Se deplacer
                Direction dir = choisir_direction();
                if ((int)dir != -1) {
                    deplacer_joueur(&carte, dir, &joueur);
                }
                break;
            }

            case 3: // Explorer
                explorer_zone(&carte, &joueur);
                verifier_progression_quetes(&systeme_quetes, &joueur, &carte);
                break;

            case 4: // Combattre
                combattre_creatures_zone(&carte, &joueur, &inventaire);
                verifier_progression_quetes(&systeme_quetes, &joueur, &carte);
                break;

            case 5: // Inventaire
                ouvrir_inventaire(&inventaire);
                break;

            case 6: // Quetes
                afficher_quete_active(&systeme_quetes);
                break;

            case 7: // Statut
                afficher_statut_joueur(&joueur, &carte); // Utilise la globale
                break;

            case 8: // Recuperer oxygene
                recuperer_oxygene(&carte, &joueur);
                break;

            case 9: // Sauvegarder
                sauvegarder_partie_complete(&carte, &joueur, NULL, &systeme_quetes, &inventaire);
                break;

            case 10: // Charger
                charger_partie_complete(&carte, &joueur, NULL, &systeme_quetes, &inventaire);
                break;

            case 0: // Quitter
                printf("\nMerci d'avoir joue a Ocean Depth!\n");
                printf("Les profondeurs vous attendent...\n");
                continuer = 0;
                break;

            default:
                printf("Choix invalide!\n");
                break;
        }

    // Petite pause pour la lisibilite
        if (continuer && choix != 0) {
            printf("\nAppuyez sur Entree pour continuer...");
            getchar();
        }
    }

    // Je libere la memoire a la fin
    liberer_carte(&carte);
    liberer_quetes(&systeme_quetes);
    creatures_clear(NULL); // Pas de liste de creatures globale, donc NULL
    liberer_inventaire(&inventaire);
}

// Fonction principale du programme
int main() {
    printf("Demarrage d'Ocean Depth...\n");

    boucle_principale();
    printf("Au revoir!\n");
    return 0;
}