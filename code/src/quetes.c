// Inclusions standards
#include <stdio.h>
#include <string.h>

// Inclusions du projet
#include "quetes.h"

// Fonction pour liberer le systeme de quetes (pour plus tard si besoin)
void liberer_quetes(SystemeQuetes *systeme) {
    // Wrapper pour compatibilite, rien a liberer ici
    (void)systeme;
}

void initialiser_quetes(SystemeQuetes *systeme) {
    if (!systeme) return;

    systeme->quete_active = 0;
    systeme->nb_quetes_terminees = 0;

    // Je cree la quete 1 : explorer la surface
    strcpy(systeme->quetes[QUETE_EXPLORATION_SURFACE].titre, "Reconnaissance de Surface");
    strcpy(systeme->quetes[QUETE_EXPLORATION_SURFACE].description,
           "Familiarisez-vous avec les eaux de surface avant de plonger plus profond.");
    strcpy(systeme->quetes[QUETE_EXPLORATION_SURFACE].objectif, "Explorez 2 zones de surface");
    systeme->quetes[QUETE_EXPLORATION_SURFACE].type = QUETE_EXPLORATION_SURFACE;
    systeme->quetes[QUETE_EXPLORATION_SURFACE].statut = STATUT_ACTIVE;
    systeme->quetes[QUETE_EXPLORATION_SURFACE].recompense_xp = 50;
    systeme->quetes[QUETE_EXPLORATION_SURFACE].recompense_or = 100;
    systeme->quetes[QUETE_EXPLORATION_SURFACE].objectif_actuel = 0;
    systeme->quetes[QUETE_EXPLORATION_SURFACE].objectif_requis = 2;

    // Je cree la quete 2 : descendre en profondeur
    strcpy(systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].titre, "Premiers Pas dans les Profondeurs");
    strcpy(systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].description,
           "Il est temps de quitter la surface et d'explorer les zones plus profondes.");
    strcpy(systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].objectif, "Explorez le recif corallien");
    systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].type = QUETE_DESCENDRE_PROFONDEUR;
    systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].statut = STATUT_NON_ACTIVE;
    systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].recompense_xp = 100;
    systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].recompense_or = 200;
    systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].objectif_actuel = 0;
    systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].objectif_requis = 1;

    // Je cree la quete 3 : combattre une creature
    strcpy(systeme->quetes[QUETE_COMBATTRE_CREATURE].titre, "Gardien des Profondeurs");
    strcpy(systeme->quetes[QUETE_COMBATTRE_CREATURE].description,
           "Les creatures marines protegent leurs territoires. Montrez que vous etes digne d'explorer.");
    strcpy(systeme->quetes[QUETE_COMBATTRE_CREATURE].objectif, "Battez 3 creatures marines");
    systeme->quetes[QUETE_COMBATTRE_CREATURE].type = QUETE_COMBATTRE_CREATURE;
    systeme->quetes[QUETE_COMBATTRE_CREATURE].statut = STATUT_NON_ACTIVE;
    systeme->quetes[QUETE_COMBATTRE_CREATURE].recompense_xp = 150;
    systeme->quetes[QUETE_COMBATTRE_CREATURE].recompense_or = 300;
    systeme->quetes[QUETE_COMBATTRE_CREATURE].objectif_actuel = 0;
    systeme->quetes[QUETE_COMBATTRE_CREATURE].objectif_requis = 3;

    // Je cree la quete 4 : explorer l'epave
    strcpy(systeme->quetes[QUETE_EXPLORER_EPAVE].titre, "Tresor de l'Epave Perdue");
    strcpy(systeme->quetes[QUETE_EXPLORER_EPAVE].description,
           "Une epave mysterieuse contient des tresors anciens. Explorez-la pour decouvrir ses secrets.");
    strcpy(systeme->quetes[QUETE_EXPLORER_EPAVE].objectif, "Explorez l'epave du navire");
    systeme->quetes[QUETE_EXPLORER_EPAVE].type = QUETE_EXPLORER_EPAVE;
    systeme->quetes[QUETE_EXPLORER_EPAVE].statut = STATUT_NON_ACTIVE;
    systeme->quetes[QUETE_EXPLORER_EPAVE].recompense_xp = 200;
    systeme->quetes[QUETE_EXPLORER_EPAVE].recompense_or = 500;
    systeme->quetes[QUETE_EXPLORER_EPAVE].objectif_actuel = 0;
    systeme->quetes[QUETE_EXPLORER_EPAVE].objectif_requis = 1;

    // Je cree la quete 5 : atteindre les abysses
    strcpy(systeme->quetes[QUETE_ATTEINDRE_ABYSSES].titre, "Dans les Abysses Interdites");
    strcpy(systeme->quetes[QUETE_ATTEINDRE_ABYSSES].description,
           "Les abysses recelent des creatures et des secrets que peu ont pu observer.");
    strcpy(systeme->quetes[QUETE_ATTEINDRE_ABYSSES].objectif, "Atteignez les abysses (profondeur 150m)");
    systeme->quetes[QUETE_ATTEINDRE_ABYSSES].type = QUETE_ATTEINDRE_ABYSSES;
    systeme->quetes[QUETE_ATTEINDRE_ABYSSES].statut = STATUT_NON_ACTIVE;
    systeme->quetes[QUETE_ATTEINDRE_ABYSSES].recompense_xp = 300;
    systeme->quetes[QUETE_ATTEINDRE_ABYSSES].recompense_or = 750;
    systeme->quetes[QUETE_ATTEINDRE_ABYSSES].objectif_actuel = 0;
    systeme->quetes[QUETE_ATTEINDRE_ABYSSES].objectif_requis = 1;

    // Je cree la quete finale : battre le boss
    strcpy(systeme->quetes[QUETE_BOSS_FINAL].titre, "Le Leviathan des Profondeurs");
    strcpy(systeme->quetes[QUETE_BOSS_FINAL].description,
           "Une creature legendaire regne dans les zones les plus profondes. Affrontez-la pour prouver votre maitrise des oceans.");
    strcpy(systeme->quetes[QUETE_BOSS_FINAL].objectif, "Battez le Leviathan des Abysses");
    systeme->quetes[QUETE_BOSS_FINAL].type = QUETE_BOSS_FINAL;
    systeme->quetes[QUETE_BOSS_FINAL].statut = STATUT_NON_ACTIVE;
    systeme->quetes[QUETE_BOSS_FINAL].recompense_xp = 500;
    systeme->quetes[QUETE_BOSS_FINAL].recompense_or = 1000;
    systeme->quetes[QUETE_BOSS_FINAL].objectif_actuel = 0;
    systeme->quetes[QUETE_BOSS_FINAL].objectif_requis = 1;
}

void verifier_progression_quetes(SystemeQuetes *systeme, Plongeur *joueur, CarteOceanique *carte) {
    if (!systeme || !joueur || !carte) return;

    // Je verifie la quete active
    Quete *quete_courante = &systeme->quetes[systeme->quete_active];

    switch (quete_courante->type) {
        case QUETE_EXPLORATION_SURFACE: {
            // Je compte les zones de surface explorees
            int zones_surface = 0;
            for (int x = 0; x < TAILLE_CARTE; x++) {
                if (carte->grille[0][x].exploree && carte->grille[0][x].type == ZONE_SURFACE) {
                    zones_surface++;
                }
            }
            quete_courante->objectif_actuel = zones_surface;
            break;
        }

        case QUETE_DESCENDRE_PROFONDEUR: {
            // Je regarde si le recif a ete explore
            if (carte->grille[1][0].exploree && carte->grille[1][0].type == ZONE_RECIFS) {
                quete_courante->objectif_actuel = 1;
            }
            break;
        }

        case QUETE_EXPLORER_EPAVE: {
            // Je regarde si l'epave a ete exploree
            if (carte->grille[1][1].exploree && carte->grille[1][1].type == ZONE_EPAVES) {
                quete_courante->objectif_actuel = 1;
            }
            break;
        }

        case QUETE_ATTEINDRE_ABYSSES: {
            // Je regarde si une zone d'abysses a ete exploree
            for (int x = 0; x < TAILLE_CARTE; x++) {
                if (carte->grille[2][x].exploree && carte->grille[2][x].type == ZONE_FOSSES) {
                    quete_courante->objectif_actuel = 1;
                    break;
                }
            }
            break;
        }

        default:
            break;
    }

    // Je regarde si la quete est terminee
    if (quete_courante->objectif_actuel >= quete_courante->objectif_requis &&
        quete_courante->statut == STATUT_ACTIVE) {
        completer_quete(systeme, joueur, quete_courante->type);
    }
}

void afficher_quete_active(SystemeQuetes *systeme) {
    if (!systeme) return;

    if (systeme->quete_active >= NB_QUETES) {
        printf("Felicitations! Vous avez termine toutes les quetes!\n");
        return;
    }

    Quete *quete = &systeme->quetes[systeme->quete_active];

    printf("\n=== QUETE ACTIVE ===\n");
    printf("%s\n", quete->titre);
    printf("%s\n", quete->description);
    printf("Objectif: %s (%d/%d)\n", quete->objectif,
           quete->objectif_actuel, quete->objectif_requis);
    printf("Recompense: %d XP + %d or\n", quete->recompense_xp, quete->recompense_or);

    // J'affiche la barre de progression
    int progression = (quete->objectif_actuel * 10) / quete->objectif_requis;
    printf("Progression: [");
    for (int i = 0; i < 10; i++) {
        if (i < progression) printf("#");
        else printf("-");
    }
    printf("] %d%%\n", (quete->objectif_actuel * 100) / quete->objectif_requis);
}

void completer_quete(SystemeQuetes *systeme, Plongeur *joueur, TypeQuete type) {
    if (!systeme || !joueur) return;

    Quete *quete = &systeme->quetes[type];
    quete->statut = STATUT_TERMINEE;
    systeme->nb_quetes_terminees++;

    printf("\n=== QUETE TERMINEE ===\n");
    printf("%s\n", quete->titre);
    printf("Vous avez gagne %d XP et %d or!\n", quete->recompense_xp, quete->recompense_or);

    // Je donne les recompenses au joueur
    joueur->xp += quete->recompense_xp;
    joueur->argent += quete->recompense_or;

    // J'active la quete suivante
    if (systeme->quete_active + 1 < NB_QUETES) {
        systeme->quete_active++;
        systeme->quetes[systeme->quete_active].statut = STATUT_ACTIVE;
        printf("Nouvelle quete disponible: %s\n",
               systeme->quetes[systeme->quete_active].titre);
    } else {
        printf("Vous avez termine toutes les quetes! Vous etes un maitre des oceans!\n");
    }
}

int jeu_termine(SystemeQuetes *systeme) {
    if (!systeme) return 0;
    return systeme->nb_quetes_terminees >= NB_QUETES;
}