// Inclusions standards
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclusions du projet
#include "combat.h"
#include "inventaire.h"
#include "joueur.h"
#include "creatures.h"
#include "zone.h"

// Quand on gagne un combat, on donne des recompenses (perles, objets, equipements)
void donner_recompenses_victoire(struct Plongeur *plongeur, struct Inventaire *inv, int difficulte) {
    int boss_id = -1;
    if (plongeur->inventaire && plongeur->inventaire->zone_actuelle) {
        struct Zone* zone = (struct Zone*)plongeur->inventaire->zone_actuelle;
        if (zone->type_creature == CREATURE_TYPE_BOSS) {
            boss_id = zone->boss_id;
        }
    }
    if (boss_id == 0) {
        printf("\nVous terrassez le Leviathan ! Recompense : Harpon Electrique et 100 perles !\n");
        ajouter_objet(inv, creer_harpon_electrique());
        plongeur->perles += 100;
    } else if (boss_id == 1) {
        printf("\nVous terrassez le Kraken ! Recompense : Armure Titanium et 120 perles !\n");
        ajouter_objet(inv, creer_combi_titanium());
        plongeur->perles += 120;
    } else if (boss_id == 2) {
        printf("\nVous terrassez le Requin Fantome ! Recompense : Harpon Electrique et 80 perles !\n");
        ajouter_objet(inv, creer_harpon_electrique());
        plongeur->perles += 80;
    } else {
        int perles = 10 + rand() % (10 * difficulte + 1);
        plongeur->perles += perles;
        printf("\nRecompense : +%d perles !\n", perles);
        // 1 chance sur 2 d'obtenir un objet de soin
        if (rand() % 2 == 0) {
            Objet soin = creer_trousse_soin();
            ajouter_objet(inv, soin);
            printf("Vous trouvez une trousse de soin !\n");
        }
        // 1 chance sur 3 d'obtenir une capsule O2
        if (rand() % 3 == 0) {
            Objet o2 = creer_capsule_oxygene();
            ajouter_objet(inv, o2);
            printf("Vous trouvez une capsule d'oxygene !\n");
        }
        // 1 chance sur 4 d'obtenir un equipement
        if (rand() % 4 == 0) {
            if (rand() % 2 == 0) {
                Objet harpon = creer_harpon_rouille();
                ajouter_objet(inv, harpon);
                printf("Vous recuperez un harpon rouille !\n");
            } else {
                Objet combi = creer_combi_neoprene();
                ajouter_objet(inv, combi);
                printf("Vous recuperez une combinaison neoprene !\n");
            }
        }
    }
}
#include "creatures.h"
/* combat.c
 * Implementation simplifiee de l'etape 3 : attaques des creatures marines.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "creatures.h"
#include "joueur.h"

// Calculer degats d'une creature (variabilite)
static int creature_compute_damage(Creature *c) {
    if (!c) return 0;
    int base = c->attack; // Chez moi l'attaque est fixe
    // J'ajoute un peu de hasard (+/-20%)
    int variance = (base * 20) / 100;
    int min = base - variance; if (min < 1) min = 1;
    int max = base + variance;
    return rand() % (max - min + 1) + min;
}

// Ici j'applique des effets speciaux selon le type de creature
static void apply_special_effect(Creature *c, Plongeur *p, int *player_attack_reduction, float *damage_multiplier) {
    (void)player_attack_reduction;
    if (!c || !p) return;
    switch (c->type) {
    case CREATURE_TYPE_BOSS: // Le boss (kraken) attaque deux fois (gere ailleurs)
            break;
    case CREATURE_TYPE_SHARK: // Le requin fait plus mal s'il a moins de 50% de PV
            if (c->hp * 2 < c->max_hp) *damage_multiplier = 1.3f;
            break;
    case CREATURE_TYPE_CRAB: // Le crabe reduit les degats (gere dans creature_damage)
            break;
        default:
            break;
    }
}

// Ici je fais attaquer toutes les creatures vivantes, triees par vitesse
// Ensuite j'applique les effets speciaux si besoin
int phase_attaque_creatures(Plongeur *plongeur, CreatureList *liste) {
    if (!liste || !plongeur) return 0;
    // Je compte combien de creatures sont encore vivantes
    int count = 0;
    for (Creature *c = liste->head; c; c = c->next) if (c->alive) count++;
    if (count == 0) return 0;

    Creature **arr = malloc(sizeof(Creature*) * count);
    if (!arr) {
        fprintf(stderr, "Erreur : echec d'allocation memoire pour le tableau de creatures.\n");
        return 0;
    }
    int i = 0;
    for (Creature *c = liste->head; c; c = c->next) if (c->alive) arr[i++] = c;


    for (int a = 0; a < count-1; ++a) for (int b = a+1; b < count; ++b) {
        int speed_a = 0, speed_b = 0;
        switch (arr[a]->type) {
            case CREATURE_TYPE_FISH: speed_a = 25; break;
            case CREATURE_TYPE_SHARK: speed_a = 30; break; // requin: rapide
            case CREATURE_TYPE_CRAB: speed_a = 10; break;  // crabe: lent mais resistant
            case CREATURE_TYPE_BOSS: speed_a = 20; break;  // kraken: vitesse moyenne
            default: speed_a = 5; break;
        }
        switch (arr[b]->type) {
            case CREATURE_TYPE_FISH: speed_b = 25; break;
            case CREATURE_TYPE_SHARK: speed_b = 30; break;
            case CREATURE_TYPE_CRAB: speed_b = 10; break;
            case CREATURE_TYPE_BOSS: speed_b = 20; break;
            default: speed_b = 5; break;
        }
        if (speed_b > speed_a || (speed_b == speed_a && arr[b]->id < arr[a]->id)) {
            Creature *tmp = arr[a]; arr[a] = arr[b]; arr[b] = tmp;
        }
    }

    int degats_totaux = 0;

    // Chaque creature attaque le joueur
    for (int k = 0; k < count; ++k) {
        Creature *c = arr[k];
        if (!c || !c->alive) continue;

        // calculate damage
        float dmg_mult = 1.0f;
        int player_attack_reduction = 0;
        apply_special_effect(c, plongeur, &player_attack_reduction, &dmg_mult);

        // Kraken special: deux attaques consecutives ("Etreinte tentaculaire")
        int hits = (c->type == CREATURE_TYPE_BOSS) ? 2 : 1;
        for (int h = 0; h < hits; ++h) {
            int dmg = creature_compute_damage(c);
            int effective = (int)(dmg * dmg_mult) - plongeur->defense;

            if (effective < 1) effective = 1;
            plongeur->points_de_vie -= effective;
            degats_totaux += effective;

            // Stress: chaque attaque fait perdre 1-2 oxygene supplementaire
            plongeur->niveau_oxygene -= (rand()%2)+1;

            // Messages descriptifs selon le type de creature
            switch (c->type) {
                case CREATURE_TYPE_BOSS:
                    printf("Le Kraken vous attaque avec ses tentacules!\n");
                    if (hits == 2 && h == 0) printf("   (Etreinte tentaculaire - 1ere attaque)\n");
                    if (hits == 2 && h == 1) printf("   (Etreinte tentaculaire - 2eme attaque)\n");
                    break;
                case CREATURE_TYPE_SHARK:
                    printf("Le Requin fonce sur vous!\n");
                    if (dmg_mult > 1.0f) printf("   (Frenesie sanguinaire - degats augmentes)\n");
                    break;
                case CREATURE_TYPE_CRAB:
                    printf("Le Crabe Geant vous pince!\n");
                    break;
                default:
                    printf("Le Poisson vous attaque!\n");
                    break;
            }

            printf("Une creature vous inflige %d degats!\n", effective);

            if (plongeur->points_de_vie <= 0) {
                printf("Le plongeur est mort !\n");
                free(arr);
                return degats_totaux;
            }
        }
    }

    free(arr);
    return degats_totaux;
}

// Execute un tour de combat complet selon l'ordre decrit
// Simplifications : actions du joueur sont simulees (attaquer une creature la plus proche)
// pour eviter d'ajouter une I/O interactive ici.
void tour_de_combat(CreatureList *liste, Plongeur *plongeur, int profondeur) {
    (void)profondeur;
    if (!liste || !plongeur) return;

    // Verifier s'il y a des creatures
    int nb_creatures = 0;
    for (Creature *c = liste->head; c; c = c->next) {
        if (c->alive && c->hp > 0) nb_creatures++;
    }

    if (nb_creatures == 0) {
        printf("Victoire! Toutes les creatures ont ete vaincues!\n");
        // Recompenses dynamiques
        donner_recompenses_victoire(plongeur, plongeur->inventaire, profondeur > 0 ? profondeur : 1);
        return;
    }

    printf("\n=== TOUR DE COMBAT ===\n");
    printf("Votre etat: %d/%d PV, %d/%d O2, Fatigue: %d\n",
           plongeur->points_de_vie, plongeur->points_de_vie_max,
           plongeur->niveau_oxygene, plongeur->niveau_oxygene_max,
           plongeur->niveau_fatigue);

    // Afficher les creatures presentes
    int index = 1;
    for (Creature *c = liste->head; c; c = c->next) {
        if (c->alive && c->hp > 0) {
            const char *nom = "Creature";
            switch (c->type) {
                case CREATURE_TYPE_FISH: nom = "Poisson"; break;
                case CREATURE_TYPE_SHARK: nom = "Requin"; break;
                case CREATURE_TYPE_CRAB: nom = "Crabe"; break;
                case CREATURE_TYPE_BOSS: nom = "Boss"; break;
                default: nom = "Creature"; break;
            }
            // Affichage emoji dynamique
            printf("%s %s: %d/%d PV\n", emoji_creature(c->type), nom, c->hp, c->max_hp);
            index++;
        }
    }

    printf("\nVos actions:\n");
    printf("1. Attaquer une creature\n");
    printf("2. Se defendre (+2 defense ce tour)\n");
    printf("3. Fuir le combat\n");
    printf("Votre choix: ");
    char buffer[32];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        printf("Entree invalide !\n");
        return;
    }
    char *endptr = NULL;
    int choix = (int)strtol(buffer, &endptr, 10);
    if (endptr == buffer || (*endptr != '\n' && *endptr != '\0')) {
        printf("Veuillez entrer un nombre entre 1 et 3.\n");
        return;
    }
    switch(choix) {
        case 1: {
            // Attaque
            printf("Choisir la cible (1-%d): ", nb_creatures);
            if (!fgets(buffer, sizeof(buffer), stdin)) {
                printf("Entree invalide !\n");
                return;
            }
            char *endptr2 = NULL;
            int cible = (int)strtol(buffer, &endptr2, 10) - 1;
            if (endptr2 == buffer || (*endptr2 != '\n' && *endptr2 != '\0')) {
                printf("Veuillez entrer un nombre valide pour la cible.\n");
                return;
            }
            if (cible >= 0 && cible < nb_creatures) {
                // Trouver la creature par index
                Creature *creature_cible = NULL;
                int current_index = 0;
                for (Creature *c = liste->head; c; c = c->next) {
                    if (c->alive && c->hp > 0) {
                        if (current_index == cible) {
                            creature_cible = c;
                            break;
                        }
                        current_index++;
                    }
                }

                if (creature_cible) {
                    int degats = 15 + rand() % 10; // 15-24 degats
                    printf("Vous infligez %d degats!\n", degats);

                    int mort = creature_damage(creature_cible, degats);
                    if (mort) {
                        printf("Creature eliminee!\n");
                    }
                }
            }
            break;
        }
        case 2:
            // Defense
            printf("Vous vous defendez! (+2 defense temporaire)\n");
            plongeur->defense += 2;
            break;
        case 3:
            // Fuite
            printf("Vous fuyez le combat!\n");
            return;
        default:
            printf("Action invalide!\n");
            break;
    }

    // Phase d'attaque des creatures
    printf("\n=== TOUR DES CREATURES ===\n");
    phase_attaque_creatures(plongeur, liste);

    // Reduire defense temporaire
    if (plongeur->defense > 2) {
        plongeur->defense -= 2;
    }

    // Consommer oxygene
    plongeur->niveau_oxygene -= 2;
    if (plongeur->niveau_oxygene < 0) {
        plongeur->niveau_oxygene = 0;
        plongeur->points_de_vie -= 5; // Degats d'asphyxie
        printf("Vous manquez d'oxygene! -5 PV\n");
    }
}