

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "carte.h"
#include "joueur.h"
#include "inventaire.h"

// Inclusions standards
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Inclusions du projet
#include "carte.h"
#include "creatures.h"
#include "zone.h"
#include "joueur.h"
#include "inventaire.h"

// Prototypes pour éviter les warnings
const char* nom_type_zone(TypeZone type);
const char* symbole_zone(TypeZone type);

// Cette fonction donne l'emoji selon le type de créature
const char* emoji_creature(CreatureType type) {
    switch(type) {
        case CREATURE_TYPE_SHARK: return "🦈";
        case CREATURE_TYPE_CRAB: return "🦀";
        case CREATURE_TYPE_FISH: return "�";
        case CREATURE_TYPE_BOSS: return "👹";
        default: return "❓";
    }
}
// Ici je mets une structure pour l'affichage sympa
typedef struct {
    char emoji[16];
    char nom[24];
    char statut[24];
} CaseAffichage;

// Fonction pour afficher la carte joliment

// Affiche la carte avec des emojis de créatures si y'en a
void afficher_carte_oceanique_emojis(CarteOceanique *carte, CreatureList *creatures) {
    (void)carte; // évite warning si non utilisé
    printf(" CARTOGRAPHIE OCÉANIQUE - SECTEUR PACIFIQUE\n");
    printf(" ┌─────────┬─────────┬─────────┬─────────┬──────────┐\n");
    for (int y = 0; y < TAILLE_CARTE; y++) {
    // Je fais la ligne des emojis
        printf(" │");
        for (int x = 0; x < TAILLE_CARTE; x++) {
            // Je regarde si une créature est sur cette case
            Creature *c = creatures ? creatures->head : NULL;
            int found = 0;
            while (c) {
                if (c->x == x && c->y == y && c->alive) {
                    printf(" %-7s│", emoji_creature(c->type));
                    found = 1;
                    break;
                }
                c = c->next;
            }
            if (!found) {
                // Si y'a pas de créature, je mets un emoji du terrain
                if (y == 0 && x == 0) printf(" %-7s│", "🏝️");
                else if (y == 0 && x == 3) printf(" %-7s│", "⛵");
                else if (y == 1 && x == 1) printf(" %-7s│", "💰");
                else if (y == 1 && x == 2) printf(" %-7s│", "🌿");
                else if (y == 1 && x == 0) printf(" %-7s│", "🐠");
                else if (y == 1 && x == 3) printf(" %-7s│", "🕳️");
                else printf(" %-7s│", "🌊");
            }
        }
        if (y == 0) printf(" SURFACE │ 0m\n");
        else printf(" ZONE %d   │ %dm\n", y, y*50);
    }
    printf(" └─────────┴─────────┴─────────┴─────────┴──────────┘\n");
}

// Ici je gère les événements aléatoires et les boss
void evenement_aleatoire(CarteOceanique *carte, Plongeur *joueur) {
    (void)carte; // évite warning si non utilisé
    int r = rand() % 100;
    if (r < 10) {
        printf("🌪️ Courant violent ! Tu perds 5 O2.\n");
        joueur->niveau_oxygene -= 5;
    } else if (r < 20) {
        printf("🕳️ Découverte d'une grotte secrète ! Tu trouves un bonus.\n");
        joueur->xp += 10;
    } else if (r < 25) {
    // Ici je fais apparaître un boss au hasard
        CreatureType boss = (rand()%2) ? CREATURE_TYPE_BOSS : CREATURE_TYPE_SHARK;
        printf("%s Boss spécial : %s surgit ! Prépare-toi au combat...\n",
            emoji_creature(boss),
            boss == CREATURE_TYPE_BOSS ? "Léviathan des Abysses" : "Requin Géant");
    // Ici je pourrais appeler une fonction spéciale pour le boss
    }
    // Sinon il se passe rien de spécial
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carte.h"
#include "joueur.h"
#include "creatures.h"

// Fonction pour libérer la carte (pour l'instant y'a rien à faire)
void liberer_carte(CarteOceanique *carte) {
    // Wrapper pour compatibilité, rien à libérer ici
    (void)carte;
}

/* Fonction pour initialiser la carte océanique */
int initialiser_carte(CarteOceanique *carte, Plongeur *joueur) {
    if (!carte || !joueur) return -1;
    
    // Je définis les types et noms possibles (hors surface/vide)
    TypeZone types_possibles[] = {ZONE_RECIFS, ZONE_EPAVES, ZONE_FORET_ALGUES, ZONE_GROTTES, ZONE_FOSSES};
    const char* noms_possibles[] = {
        "Récif Corallien", "Épave Engloutie", "Forêt d'Algues", "Grottes Marines", "Fosses Abyssales",
        "Canyon Bleu", "Plateau Sableux", "Champ de Méduses", "Dôme Obscur", "Faille Lumineuse"
    };
    int nb_types = sizeof(types_possibles)/sizeof(types_possibles[0]);
    int nb_noms = sizeof(noms_possibles)/sizeof(noms_possibles[0]);

    // J'initialise la grille 4x4
    for (int y = 0; y < TAILLE_CARTE; y++) {
        for (int x = 0; x < TAILLE_CARTE; x++) {
            Zone *zone = &carte->grille[y][x];
            zone->exploree = 0;
            zone->debroquee = 0;
            zone->nb_creatures = 0;

            // Je mets la surface sur les coins, le reste vide
            if (y == 0) {
                zone->profondeur = 0;
                if (x == 0 || x == 3) {
                    zone->type = ZONE_SURFACE;
                    strcpy(zone->nom, "Base Sous-Marine");
                } else {
                    zone->type = ZONE_VIDE;
                    strcpy(zone->nom, "Zone Vide");
                }
            } else {
                // Je mets la profondeur selon la ligne
                zone->profondeur = y == 1 ? 50 : (y == 2 ? 150 : 300);
                // Je tire au hasard le type et le nom
                int idx_type = rand() % nb_types;
                int idx_nom = rand() % nb_noms;
                zone->type = types_possibles[idx_type];
                strcpy(zone->nom, noms_possibles[idx_nom]);
                // Je mets un nombre de créatures au hasard (sauf surface/vide)
                zone->nb_creatures = 1 + rand() % 3;
            }
        }
    }
    // Je vérifie qu'au moins une des deux cases à côté de la base n'est pas vide
    Zone *z_bas = &carte->grille[1][0];
    Zone *z_droite = &carte->grille[0][1];
    if (z_bas->type == ZONE_VIDE && z_droite->type == ZONE_VIDE) {
    // Si besoin, je force celle du bas à être un récif
        z_bas->type = ZONE_RECIFS;
        strcpy(z_bas->nom, "Récif Corallien");
        z_bas->profondeur = 50;
        z_bas->nb_creatures = 1 + rand() % 3;
    }
    
    // Je place le joueur à la base (0,0)
    carte->position_joueur.x = 0;
    carte->position_joueur.y = 0;
    carte->position_joueur.zone_actuelle = ZONE_SURFACE;
    carte->zones_decouvertes = 1;
    
    // Je débloque et j'explore la base de départ
    carte->grille[0][0].debroquee = 1;
    carte->grille[0][0].exploree = 1;
    // Je débloque toujours les deux cases à côté de la base
    if (TAILLE_CARTE > 1) carte->grille[1][0].debroquee = 1;
    if (TAILLE_CARTE > 1) carte->grille[0][1].debroquee = 1;
    // Je débloque les autres cases de la ligne 0 si elles sont pas vides
    for (int x = 1; x < TAILLE_CARTE; x++) {
        if (carte->grille[0][x].type != ZONE_VIDE) carte->grille[0][x].debroquee = 1;
    }
    
    return 0;
}

/* Fonction pour récupérer la zone où est le joueur */
Zone* get_zone_actuelle(CarteOceanique *carte) {
    if (!carte) return NULL;
    return &carte->grille[carte->position_joueur.y][carte->position_joueur.x];
}

/* Fonction pour débloquer les zones autour d'une case */
void debloquer_zones_adjacentes(CarteOceanique *carte, int x, int y) {
    if (!carte) return;
    
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}}; // Je fais les directions haut, bas, gauche, droite
    int nouvelles_zones = 0;
    
    for (int i = 0; i < 4; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];
        
        if (nx >= 0 && nx < TAILLE_CARTE && ny >= 0 && ny < TAILLE_CARTE) {
            Zone *zone_adj = &carte->grille[ny][nx];
            // Je débloque les zones autour si elles sont pas vides
            if (!zone_adj->debroquee && zone_adj->type != ZONE_VIDE) {
                zone_adj->debroquee = 1;
                nouvelles_zones++;
                printf("🗝️ Zone %s débloquée en (%d,%d)!\n", nom_type_zone(zone_adj->type), nx, ny);
            }
        }
    }
    
    // Je débloque aussi les diagonales pour que ce soit plus sympa
    int diagonales[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (int i = 0; i < 4; i++) {
        int nx = x + diagonales[i][0];
        int ny = y + diagonales[i][1];
        
        if (nx >= 0 && nx < TAILLE_CARTE && ny >= 0 && ny < TAILLE_CARTE) {
            Zone *zone_adj = &carte->grille[ny][nx];
            if (!zone_adj->debroquee && zone_adj->type != ZONE_VIDE && 
                carte->grille[y][x].exploree) { // Je débloque la diagonale que si la zone est explorée
                zone_adj->debroquee = 1;
                nouvelles_zones++;
                printf("🌟 Zone bonus %s débloquée en (%d,%d)!\n", nom_type_zone(zone_adj->type), nx, ny);
            }
        }
    }
    
    if (nouvelles_zones > 0) {
        printf("🗝️ Nouvelle zone accessible !\n");
    }
}

/* Fonction pour afficher toute la carte proprement */
void afficher_carte_complete(CarteOceanique *carte) {
    if (!carte) return;
    
    printf("\n CARTOGRAPHIE OCÉANIQUE - SECTEUR PACIFIQUE\n");
    printf(" ┌─────────┬─────────┬─────────┬─────────┬──────────┐\n");
    
    for (int y = 0; y < TAILLE_CARTE; y++) {
        // Je fais la ligne du contenu
        printf(" │");
        for (int x = 0; x < TAILLE_CARTE; x++) {
            Zone *zone = &carte->grille[y][x];
            // Je mets le symbole de la zone
            if (carte->position_joueur.x == x && carte->position_joueur.y == y) {
                printf(" ♦%s ♦ │", symbole_zone(zone->type));
            } else if (zone->debroquee) {
                printf("  %s   │", symbole_zone(zone->type));
            } else {
                printf("  ❌   │");
            }
        }
        // J'affiche le niveau (surface ou profondeur)
        if (y == 0) printf(" SURFACE │ %dm", carte->grille[y][0].profondeur);
        else printf(" ZONE %d │ %dm", y, carte->grille[y][0].profondeur);
        printf("\n │");

        // Je fais la ligne des noms et états
        for (int x = 0; x < TAILLE_CARTE; x++) {
            Zone *zone = &carte->grille[y][x];
            const char *nom = nom_type_zone(zone->type);
            // Je mets un nom court pour chaque zone
            char nom_court[10];
            if (strcmp(nom, "Récif Corallien") == 0) strcpy(nom_court, "Récif");
            else if (strcmp(nom, "Épave du Navire") == 0) strcpy(nom_court, "Épave");
            else if (strcmp(nom, "Zone Vide") == 0) strcpy(nom_court, "Vide");
            else strncpy(nom_court, nom, 9);
            nom_court[9] = '\0';
            printf(" %-7s│", nom_court);
        }
        printf("         │\n");

        // Je fais la ligne des états (exploré, lock, etc)
        printf(" │");
        for (int x = 0; x < TAILLE_CARTE; x++) {
            Zone *zone = &carte->grille[y][x];
            if (!zone->debroquee) {
                printf(" [LOCK]  │");
            } else if (!zone->exploree) {
                printf("         │");
            } else if (zone->nb_creatures > 0) {
                printf(" [%d ENM] │", zone->nb_creatures);
            } else if (zone->type == ZONE_EPAVES) {
                printf(" [TRÉSOR]│");
            } else if (zone->type == ZONE_SURFACE) {
                printf(" [SAUF]  │");
            } else {
                printf("         │");
            }
        }
        printf("         │\n");

        if (y < TAILLE_CARTE - 1) {
            printf(" ├─────────┼─────────┼─────────┼─────────┼──────────┤\n");
        }
    }

    printf(" └─────────┴─────────┴─────────┴─────────┴──────────┘\n");

    Zone *zone_actuelle = get_zone_actuelle(carte);
    if (zone_actuelle) {
        printf("\nPosition actuelle: [%s] %s (%dm)\n", 
               symbole_zone(zone_actuelle->type),
               zone_actuelle->nom,
               zone_actuelle->profondeur);
        // J'affiche les conditions selon la profondeur
        printf("Conditions: ");
        if (zone_actuelle->profondeur == 0) printf("Courant faible, Visibilité excellente, Température 25°C\n");
        else if (zone_actuelle->profondeur <= 50) printf("Courant faible, Visibilité bonne, Température 23°C\n");
        else if (zone_actuelle->profondeur <= 150) printf("Courant moyen, Visibilité moyenne, Température 18°C\n");
        else printf("Courant fort, Visibilité faible, Température 12°C\n");
    }
}

/* Fonction pour donner le nom d'un type de zone */
const char* nom_type_zone(TypeZone type) {
    switch (type) {
        case ZONE_SURFACE: return "Base Sous-Marine";
        case ZONE_RECIFS: return "Récif Corallien";
        case ZONE_EPAVES: return "Épave du Titanic";
        case ZONE_FORET_ALGUES: return "Forêt d'Algues";
        case ZONE_GROTTES: return "Gouffre Mystérieux";
        case ZONE_FOSSES: return "Abysses";
        case ZONE_VIDE: return "Océan Ouvert";
        case ZONE_INCONNUE: return "Fosse Profonde";
        default: return "Zone Mystérieuse";
    }
}

/* Fonction pour donner le symbole d'un type de zone */
const char* symbole_zone(TypeZone type) {
    switch (type) {
        case ZONE_SURFACE: return "🏝️";
        case ZONE_RECIFS: return "🐠";
        case ZONE_EPAVES: return "💰";
        case ZONE_FORET_ALGUES: return "🌿";
        case ZONE_GROTTES: return "🕳️";
        case ZONE_FOSSES: return "🐙";
        case ZONE_VIDE: return "❌";
        case ZONE_INCONNUE: return "❓";
        default: return "🌊";
    }
}

/* Fonction pour afficher les créatures de la zone */
void afficher_creatures_zone(CarteOceanique *carte) {
    Zone *zone = get_zone_actuelle(carte);
    if (!zone) return;
    
    printf("\n🦈 === CRÉATURES DE LA ZONE ===\n");
    printf("📍 Zone: %s\n", zone->nom);
    
    if (zone->nb_creatures > 0) {
        printf("🚨 %d créature(s) hostile(s) présente(s)\n", zone->nb_creatures);
        printf("⚠️  Danger estimé: ");
        if (zone->profondeur < 50) printf("Faible 🟢\n");
        else if (zone->profondeur < 150) printf("Moyen 🟡\n");
        else printf("Élevé 🔴\n");
    } else {
        printf("✅ Zone paisible, aucune créature hostile\n");
    }
}