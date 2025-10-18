/* structure du joueur
 * points de vie
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int rand_range(int min, int max) {
    if (min > max) {
        int tmp = min; min = max; max = tmp;
    }
    return (rand() % (max - min + 1)) + min;
}

typedef struct {
    int points_de_vie;
    int points_de_vie_max;
    int niveau_oxygene;
    int niveau_oxygene_max;

    int niveau_fatigue; //0 à 5
    int perles; // monnaie du jeu
} Plongeur;

/* Initialise un Plongeur; retourne le pointeur passé (ou NULL si NULL) */
Plongeur *initialiser_joueur(Plongeur *plongeur, int points_de_vie, int points_de_vie_max, int niveau_oxygene, int niveau_oxygene_max, int niveau_fatigue, int perles) {
    if (!plongeur) return NULL;
    plongeur->points_de_vie = points_de_vie;
    plongeur->points_de_vie_max = points_de_vie_max;
    plongeur->niveau_oxygene = niveau_oxygene;
    plongeur->niveau_oxygene_max = niveau_oxygene_max;
    plongeur->niveau_fatigue = niveau_fatigue;
    plongeur->perles = perles;
    return plongeur;
}

int gestion_fatigue(Plongeur *plongeur, int action) {
    int max_attack = 0;
 if (plongeur->niveau_fatigue == 0 || plongeur->niveau_fatigue == 1) {
   max_attack = 3;
   return max_attack;
 } else if (plongeur->niveau_fatigue == 2 || plongeur->niveau_fatigue == 3) {
   max_attack = 2;
   return max_attack;
 } else if (plongeur->niveau_fatigue == 4 || plongeur->niveau_fatigue == 5) {
   max_attack = 1;
   return max_attack;
 }
 return -1;
}

int consommation_oxygene(Plongeur *plongeur, int action, int profondeur) {

    if (action == 1) { /* normale */
        if (profondeur < 5) {
            plongeur->niveau_oxygene -= rand_range(2, 4); /* 2..4 */
        } else if (profondeur < 10 && profondeur >= 5) {
            plongeur->niveau_oxygene -= rand_range(5, 8); /* 5..8 */
        }
    }

    if (plongeur->niveau_oxygene <= 10) {
        printf(" ALERTE CRITIQUE\n");
    }

    if (plongeur->niveau_oxygene <= 0) {
        plongeur->niveau_oxygene = 0;
        plongeur->points_de_vie -= 5;
    }

    return plongeur->niveau_oxygene;
}

int calcul_degats_variation(int attaque_min, int attaque_max, int defense_creature, int bonus_arme) {
    if (attaque_min > attaque_max) {
        int tmp = attaque_min; attaque_min = attaque_max; attaque_max = tmp;
    }

    int degats_base = rand() % (attaque_max - attaque_min + 1) + attaque_min;
    degats_base += bonus_arme;
    int degats = degats_base - defense_creature;
    if (degats < 1) degats = 1;
    return degats;
}