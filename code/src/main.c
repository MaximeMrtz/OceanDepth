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

// Variable globale pour le joueur (pour accès partout)
Plongeur joueur;
// Fonction pour lire un entier entré par l'utilisateur
int lire_entier(const char* prompt) {
    char buffer[MAX_INPUT];
    printf("%s", prompt);
    
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin)) {
            char *endptr;
            int result = strtol(buffer, &endptr, 10);
            
            // Je vérifie si la conversion est bonne
            if (endptr != buffer && (*endptr == '\n' || *endptr == '\0')) {
                return result;
            }
        }
        printf("❌ Entrée invalide. Veuillez saisir un nombre: ");
    }
}

// Fonction pour afficher le menu principal
void afficher_menu_principal() {
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║          🌊 OCEAN DEPTH 🌊              ║\n");
    printf("║      Exploration des Profondeurs        ║\n");
    printf("║  👤 Joueur : %s\n", joueur.nom);
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  1. 🧭 Voir la carte                     ║\n");
    printf("║  2. ⬆️  Se déplacer                      ║\n");
    printf("║  3. 🔍 Explorer la zone                  ║\n");
    printf("║  4. ⚔️  Combattre les créatures          ║\n");
    printf("║  5. 🎒 Inventaire                        ║\n");
    printf("║  6. 📝 Quêtes                            ║\n");
    printf("║  7. 👤 Statut du joueur                  ║\n");
    printf("║  8. 💨 Récupérer oxygène (surface)       ║\n");
    printf("║  9. 💾 Sauvegarder                       ║\n");
    printf("║ 10. 🔄 Charger sauvegarde                ║\n");
    printf("║  0. 🚪 Quitter                           ║\n");
    printf("╚══════════════════════════════════════════╝\n");
}

// Fonction pour afficher le menu de déplacement
Direction choisir_direction() {
    printf("\n🧭 === CHOISIR UNE DIRECTION ===\n");
    printf("1. ⬆️  NORD\n");
    printf("2. ⬇️  SUD\n");
    printf("3. ➡️  EST\n");
    printf("4. ⬅️  OUEST\n");
    printf("0. 🔙 Retour\n");
    
    int choix = lire_entier("👉 Votre choix: ");
    
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
    
    printf("\n👤 === STATUT DU PLONGEUR ===\n");
    printf("🏷️  Nom: %s\n", joueur->nom);
    printf("❤️  Santé: %d/%d\n", joueur->points_de_vie, joueur->points_de_vie_max);
    printf("💨 Oxygène: %d/%d\n", joueur->niveau_oxygene, joueur->niveau_oxygene_max);
    printf("⭐ Expérience: %d XP\n", joueur->xp);
    printf("💰 Argent: %d pièces\n", joueur->argent);
    
    if (zone_actuelle) {
        printf("\n📍 === POSITION ACTUELLE ===\n");
        printf("🌍 Position: (%d,%d)\n", carte->position_joueur.x, carte->position_joueur.y);
        printf("🏷️  Zone: %s\n", zone_actuelle->nom);
        printf("🌊 Profondeur: %dm\n", zone_actuelle->profondeur);
        printf("🔍 Exploration: %s\n", zone_actuelle->exploree ? "✅ Explorée" : "❓ Non explorée");
    }
    
    printf("\n🗺️  === PROGRESSION ===\n");
    printf("🎯 Zones découvertes: %d/16\n", carte->zones_decouvertes);
    printf("📝 Quêtes actives: Voir menu quêtes\n");
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

    // Menu de chargement si sauvegarde présente
    FILE *f = fopen("partie_oceandepth.save", "r");
    int chargee = 0;
    if (f) {
        fclose(f);
        printf("\n💾 Une sauvegarde a été détectée !\n");
        printf("1. Charger la sauvegarde\n2. Nouvelle partie\n");
        int choix = lire_entier("👉 Votre choix : ");
        if (choix == 1) {
            if (charger_partie_complete(&carte, &joueur, NULL, &systeme_quetes, &inventaire) == 0) {
                printf("✅ Sauvegarde chargée !\n");
                // Correction position joueur si hors limites
                if (carte.position_joueur.x < 0 || carte.position_joueur.x >= TAILLE_CARTE) carte.position_joueur.x = 0;
                if (carte.position_joueur.y < 0 || carte.position_joueur.y >= TAILLE_CARTE) carte.position_joueur.y = 0;
                // Affichage récapitulatif
                printf("\n--- Récapitulatif de la sauvegarde chargée ---\n");
                afficher_statut_joueur(&joueur, &carte); // Utilise la globale
                printf("\n🎒 Inventaire :\n");
                afficher_inventaire(&inventaire);
                printf("\n📝 Quêtes :\n");
                afficher_quete_active(&systeme_quetes);
                printf("---------------------------------------------\n");
                chargee = 1;
            } else {
                printf("❌ Erreur lors du chargement, nouvelle partie lancée.\n");
            }
        }
    }

    if (!chargee) {
        // Je crée le joueur
        printf("🌊 Bienvenue dans Ocean Depth! 🌊\n");
        printf("Entrez le nom de votre plongeur: ");
        fgets(joueur.nom, sizeof(joueur.nom), stdin);
        // J'enlève le retour à la ligne à la fin du nom
        joueur.nom[strcspn(joueur.nom, "\n")] = 0;

        // J'initialise le joueur
        if (initialiser_joueur(&joueur, joueur.nom, "", 100, 100, 100, 100, 0, 0) == NULL) {
            printf("❌ Erreur lors de l'initialisation du joueur!\n");
            return;
        }

        // J'initialise les systèmes du jeu
        if (initialiser_carte(&carte, &joueur) != 0) {
            printf("❌ Erreur lors de l'initialisation de la carte!\n");
            return;
        }

        initialiser_quetes(&systeme_quetes);
        printf("✅ Quêtes initialisées!\n");

        initialiser_inventaire(&inventaire);
        printf("✅ Inventaire initialisé!\n");

        printf("\n✅ Initialisation terminée!\n");
        printf("🏝️  Vous commencez à la base sous-marine.\n");
    }
    
    // Je lance la boucle principale du jeu
    int continuer = 1;
    while (continuer) {
    // Je fais les vérifications de base
        if (joueur.points_de_vie <= 0) {
            printf("\n💀 GAME OVER! Votre santé est épuisée.\n");
            printf("🔄 Retour à la base pour récupération...\n");
            joueur.points_de_vie = 50;
            carte.position_joueur.x = 0;
            carte.position_joueur.y = 0;
        }
        
        if (joueur.niveau_oxygene <= 0) {
            printf("\n💨 ATTENTION! Votre oxygène est épuisé!\n");
            printf("🚨 Retour automatique en surface!\n");
            joueur.niveau_oxygene = 20;
            joueur.points_de_vie -= 10;
            carte.position_joueur.x = 0;
            carte.position_joueur.y = 0;
        }
        
    // J'affiche le menu
        afficher_menu_principal();
        
        int choix = lire_entier("👉 Votre choix: ");
        
        switch (choix) {
            case 1: // Voir la carte
                afficher_carte_complete(&carte);
                afficher_deplacements_possibles(&carte);
                break;

            case 2: { // Se déplacer
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

            case 6: // Quêtes
                afficher_quete_active(&systeme_quetes);
                break;

            case 7: // Statut
                afficher_statut_joueur(&joueur, &carte); // Utilise la globale
                break;

            case 8: // Récupérer oxygène
                recuperer_oxygene(&carte, &joueur);
                break;

            case 9: // Sauvegarder
                sauvegarder_partie_complete(&carte, &joueur, NULL, &systeme_quetes, &inventaire);
                break;

            case 10: // Charger
                charger_partie_complete(&carte, &joueur, NULL, &systeme_quetes, &inventaire);
                break;

            case 0: // Quitter
                printf("\n🌊 Merci d'avoir joué à Ocean Depth!\n");
                printf("🐠 Les profondeurs vous attendent...\n");
                continuer = 0;
                break;

            default:
                printf("❌ Choix invalide!\n");
                break;
        }
        
    // Petite pause pour la lisibilité
        if (continuer && choix != 0) {
            printf("\nAppuyez sur Entrée pour continuer...");
            getchar();
        }
    }

    // Je libère la mémoire à la fin
    liberer_carte(&carte);
    liberer_quetes(&systeme_quetes);
    creatures_clear(NULL); // Pas de liste de créatures globale, donc NULL
    liberer_inventaire(&inventaire);
}

// Fonction principale du programme
int main() {
    printf("🚀 Démarrage d'Ocean Depth...\n");
    
    boucle_principale();
    printf("👋 Au revoir!\n");
    return 0;
}