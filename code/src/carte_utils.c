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

// Prototypes pour eviter les warnings
const char* nom_type_zone(TypeZone type);
const char* symbole_zone(TypeZone type);

// Cette fonction donne l'emoji selon le type de creature
const char* emoji_creature(CreatureType type) {
    switch(type) {
        case CREATURE_TYPE_SHARK: return "SHARK";
        case CREATURE_TYPE_CRAB: return "CRAB";
        case CREATURE_TYPE_FISH: return "FISH";
        case CREATURE_TYPE_BOSS: return "BOSS";
        default: return "?";
    }
}
// Ici je mets une structure pour l'affichage sympa
typedef struct {
    char emoji[16];
    char nom[24];
    char statut[24];
} CaseAffichage;

// Fonction pour afficher la carte joliment

// Affiche la carte avec des emojis de creatures si y'en a
void afficher_carte_oceanique_emojis(CarteOceanique *carte, CreatureList *creatures) {
    (void)carte; // evite warning si non utilise
    printf(" CARTOGRAPHIE OCEANIQUE - SECTEUR PACIFIQUE\n");
    printf(" +----------+----------+----------+----------+-----------+\n");
    for (int y = 0; y < TAILLE_CARTE; y++) {
    // Je fais la ligne des emojis
        printf(" |");
        for (int x = 0; x < TAILLE_CARTE; x++) {
            // Je regarde si une creature est sur cette case
            Creature *c = creatures ? creatures->head : NULL;
            int found = 0;
            while (c) {
                if (c->x == x && c->y == y && c->alive) {
                    printf(" %-8s|", emoji_creature(c->type));
                    found = 1;
                    break;
                }
                c = c->next;
            }
            if (!found) {
                // Si y'a pas de creature, je mets un emoji du terrain
                if (y == 0 && x == 0) printf(" %-8s|", "BASE");
                else if (y == 0 && x == 3) printf(" %-8s|", "BOAT");
                else if (y == 1 && x == 1) printf(" %-8s|", "CHEST");
                else if (y == 1 && x == 2) printf(" %-8s|", "KELP");
                else if (y == 1 && x == 0) printf(" %-8s|", "REEF");
                else if (y == 1 && x == 3) printf(" %-8s|", "CAVE");
                else printf(" %-8s|", "OCEAN");
            }
        }
        if (y == 0) printf(" SURFACE | 0m\n");
        else printf(" ZONE %d  | %dm\n", y, y*50);
    }
    printf(" +----------+----------+----------+----------+-----------+\n");
}

// Ici je gere les evenements aleatoires et les boss
void evenement_aleatoire(CarteOceanique *carte, Plongeur *joueur) {
    (void)carte; // evite warning si non utilise
    int r = rand() % 100;
    if (r < 10) {
        printf("Courant violent ! Tu perds 5 O2.\n");
        joueur->niveau_oxygene -= 5;
    } else if (r < 20) {
        printf("Decouverte d'une grotte secrete ! Tu trouves un bonus.\n");
        joueur->xp += 10;
    } else if (r < 25) {
    // Ici je fais apparaitre un boss au hasard
        CreatureType boss = (rand()%2) ? CREATURE_TYPE_BOSS : CREATURE_TYPE_SHARK;
        printf("%s Boss special : %s surgit ! Prepare-toi au combat...\n",
            emoji_creature(boss),
            boss == CREATURE_TYPE_BOSS ? "Leviathan des Abysses" : "Requin Geant");
    // Ici je pourrais appeler une fonction speciale pour le boss
    }
    // Sinon il se passe rien de special
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carte.h"
#include "joueur.h"
#include "creatures.h"

// Fonction pour liberer la carte (pour l'instant y'a rien a faire)
void liberer_carte(CarteOceanique *carte) {
    // Wrapper pour compatibilite, rien a liberer ici
    (void)carte;
}

/* Fonction pour initialiser la carte oceanique */
int initialiser_carte(CarteOceanique *carte, Plongeur *joueur) {
    if (!carte || !joueur) return -1;

    // Je definis les types et noms possibles (hors surface/vide)
    TypeZone types_possibles[] = {ZONE_RECIFS, ZONE_EPAVES, ZONE_FORET_ALGUES, ZONE_GROTTES, ZONE_FOSSES};
    const char* noms_possibles[] = {
        "Recif Corallien", "Epave Engloutie", "Foret d'Algues", "Grottes Marines", "Fosses Abyssales",
        "Canyon Bleu", "Plateau Sableux", "Champ de Meduses", "Dome Obscur", "Faille Lumineuse"
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
                // Je mets un nombre de creatures au hasard (sauf surface/vide)
                zone->nb_creatures = 1 + rand() % 3;
            }
        }
    }
    // Je verifie qu'au moins une des deux cases a cote de la base n'est pas vide
    Zone *z_bas = &carte->grille[1][0];
    Zone *z_droite = &carte->grille[0][1];
    if (z_bas->type == ZONE_VIDE && z_droite->type == ZONE_VIDE) {
    // Si besoin, je force celle du bas a etre un recif
        z_bas->type = ZONE_RECIFS;
        strcpy(z_bas->nom, "Recif Corallien");
        z_bas->profondeur = 50;
        z_bas->nb_creatures = 1 + rand() % 3;
    }

    // Je place le joueur a la base (0,0)
    carte->position_joueur.x = 0;
    carte->position_joueur.y = 0;
    carte->position_joueur.zone_actuelle = ZONE_SURFACE;
    carte->zones_decouvertes = 1;

    // Je debloque et j'explore la base de depart
    carte->grille[0][0].debroquee = 1;
    carte->grille[0][0].exploree = 1;
    // Je debloque toujours les deux cases a cote de la base
    if (TAILLE_CARTE > 1) carte->grille[1][0].debroquee = 1;
    if (TAILLE_CARTE > 1) carte->grille[0][1].debroquee = 1;
    // Je debloque les autres cases de la ligne 0 si elles sont pas vides
    for (int x = 1; x < TAILLE_CARTE; x++) {
        if (carte->grille[0][x].type != ZONE_VIDE) carte->grille[0][x].debroquee = 1;
    }

    return 0;
}

/* Fonction pour recuperer la zone ou est le joueur */
Zone* get_zone_actuelle(CarteOceanique *carte) {
    if (!carte) return NULL;
    return &carte->grille[carte->position_joueur.y][carte->position_joueur.x];
}

/* Fonction pour debloquer les zones autour d'une case */
void debloquer_zones_adjacentes(CarteOceanique *carte, int x, int y) {
    if (!carte) return;

    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}}; // Je fais les directions haut, bas, gauche, droite
    int nouvelles_zones = 0;

    for (int i = 0; i < 4; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];

        if (nx >= 0 && nx < TAILLE_CARTE && ny >= 0 && ny < TAILLE_CARTE) {
            Zone *zone_adj = &carte->grille[ny][nx];
            // Je debloque les zones autour si elles sont pas vides
            if (!zone_adj->debroquee && zone_adj->type != ZONE_VIDE) {
                zone_adj->debroquee = 1;
                nouvelles_zones++;
                printf("Zone %s debloquee en (%d,%d)!\n", nom_type_zone(zone_adj->type), nx, ny);
            }
        }
    }

    // Je debloque aussi les diagonales pour que ce soit plus sympa
    int diagonales[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (int i = 0; i < 4; i++) {
        int nx = x + diagonales[i][0];
        int ny = y + diagonales[i][1];

        if (nx >= 0 && nx < TAILLE_CARTE && ny >= 0 && ny < TAILLE_CARTE) {
            Zone *zone_adj = &carte->grille[ny][nx];
            if (!zone_adj->debroquee && zone_adj->type != ZONE_VIDE &&
                carte->grille[y][x].exploree) { // Je debloque la diagonale que si la zone est exploree
                zone_adj->debroquee = 1;
                nouvelles_zones++;
                printf("Zone bonus %s debloquee en (%d,%d)!\n", nom_type_zone(zone_adj->type), nx, ny);
            }
        }
    }

    if (nouvelles_zones > 0) {
        printf("Nouvelle zone accessible !\n");
    }
}

/* Fonction pour afficher toute la carte proprement */
void afficher_carte_complete(CarteOceanique *carte) {
    if (!carte) return;

    printf("\n CARTOGRAPHIE OCEANIQUE - SECTEUR PACIFIQUE\n");
    printf(" +----------+----------+----------+----------+-----------+\n");

    for (int y = 0; y < TAILLE_CARTE; y++) {
        // Je fais la ligne du contenu
        printf(" |");
        for (int x = 0; x < TAILLE_CARTE; x++) {
            Zone *zone = &carte->grille[y][x];
            // Je mets le symbole de la zone
            if (carte->position_joueur.x == x && carte->position_joueur.y == y) {
                printf(" *%s * |", symbole_zone(zone->type));
            } else if (zone->debroquee) {
                printf("  %s   |", symbole_zone(zone->type));
            } else {
                printf("  XX   |");
            }
        }
        // J'affiche le niveau (surface ou profondeur)
        if (y == 0) printf(" SURFACE | %dm", carte->grille[y][0].profondeur);
        else printf(" ZONE %d | %dm", y, carte->grille[y][0].profondeur);
        printf("\n |");

        // Je fais la ligne des noms et etats
        for (int x = 0; x < TAILLE_CARTE; x++) {
            Zone *zone = &carte->grille[y][x];
            const char *nom = nom_type_zone(zone->type);
            // Je mets un nom court pour chaque zone
            char nom_court[10];
            if (strcmp(nom, "Recif Corallien") == 0) strcpy(nom_court, "Recif");
            else if (strcmp(nom, "Epave du Navire") == 0) strcpy(nom_court, "Epave");
            else if (strcmp(nom, "Zone Vide") == 0) strcpy(nom_court, "Vide");
            else strncpy(nom_court, nom, 9);
            nom_court[9] = '\0';
            printf(" %-8s|", nom_court);
        }
        printf("         |\n");

        // Je fais la ligne des etats (explore, lock, etc)
        printf(" |");
        for (int x = 0; x < TAILLE_CARTE; x++) {
            Zone *zone = &carte->grille[y][x];
            if (!zone->debroquee) {
                printf(" [LOCK]  |");
            } else if (!zone->exploree) {
                printf("         |");
            } else if (zone->nb_creatures > 0) {
                printf(" [%d ENM] |", zone->nb_creatures);
            } else if (zone->type == ZONE_EPAVES) {
                printf(" [TRESOR]|");
            } else if (zone->type == ZONE_SURFACE) {
                printf(" [SAUF]  |");
            } else {
                printf("         |");
            }
        }
        printf("         |\n");

        if (y < TAILLE_CARTE - 1) {
            printf(" +----------+----------+----------+----------+-----------+\n");
        }
    }

    printf(" +----------+----------+----------+----------+-----------+\n");

    Zone *zone_actuelle = get_zone_actuelle(carte);
    if (zone_actuelle) {
        printf("\nPosition actuelle: [%s] %s (%dm)\n",
               symbole_zone(zone_actuelle->type),
               zone_actuelle->nom,
               zone_actuelle->profondeur);
        // J'affiche les conditions selon la profondeur
        printf("Conditions: ");
        if (zone_actuelle->profondeur == 0) printf("Courant faible, Visibilite excellente, Temperature 25C\n");
        else if (zone_actuelle->profondeur <= 50) printf("Courant faible, Visibilite bonne, Temperature 23C\n");
        else if (zone_actuelle->profondeur <= 150) printf("Courant moyen, Visibilite moyenne, Temperature 18C\n");
        else printf("Courant fort, Visibilite faible, Temperature 12C\n");
    }
}

/* Fonction pour donner le nom d'un type de zone */
const char* nom_type_zone(TypeZone type) {
    switch (type) {
        case ZONE_SURFACE: return "Base Sous-Marine";
        case ZONE_RECIFS: return "Recif Corallien";
        case ZONE_EPAVES: return "Epave du Titanic";
        case ZONE_FORET_ALGUES: return "Foret d'Algues";
        case ZONE_GROTTES: return "Gouffre Mysterieux";
        case ZONE_FOSSES: return "Abysses";
        case ZONE_VIDE: return "Ocean Ouvert";
        case ZONE_INCONNUE: return "Fosse Profonde";
        default: return "Zone Mysterieuse";
    }
}

/* Fonction pour donner le symbole d'un type de zone */
const char* symbole_zone(TypeZone type) {
    switch (type) {
        case ZONE_SURFACE: return "BASE";
        case ZONE_RECIFS: return "REEF";
        case ZONE_EPAVES: return "WRCK";
        case ZONE_FORET_ALGUES: return "KELP";
        case ZONE_GROTTES: return "CAVE";
        case ZONE_FOSSES: return "DEEP";
        case ZONE_VIDE: return "VOID";
        case ZONE_INCONNUE: return "????";
        default: return "WAVE";
    }
}

/* Fonction pour afficher les creatures de la zone */
void afficher_creatures_zone(CarteOceanique *carte) {
    Zone *zone = get_zone_actuelle(carte);
    if (!zone) return;

    printf("\n=== CREATURES DE LA ZONE ===\n");
    printf("Zone: %s\n", zone->nom);

    if (zone->nb_creatures > 0) {
        printf("ALERTE: %d creature(s) hostile(s) presente(s)\n", zone->nb_creatures);
        printf("Danger estime: ");
        if (zone->profondeur < 50) printf("Faible\n");
        else if (zone->profondeur < 150) printf("Moyen\n");
        else printf("Eleve\n");
    } else {
        printf("Zone paisible, aucune creature hostile\n");
    }
}