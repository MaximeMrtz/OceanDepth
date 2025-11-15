

// Inclusions standards
#include <stdio.h>
#include <string.h>

// Inclusions du projet
#include "quetes.h"

// Fonction pour libérer le système de quêtes (pour plus tard si besoin)
void liberer_quetes(SystemeQuetes *systeme) {
    // Wrapper pour compatibilité, rien à libérer ici
    (void)systeme;
}

void initialiser_quetes(SystemeQuetes *systeme) {
    if (!systeme) return;
    
    systeme->quete_active = 0;
    systeme->nb_quetes_terminees = 0;
    
    // Je crée la quête 1 : explorer la surface
    strcpy(systeme->quetes[QUETE_EXPLORATION_SURFACE].titre, "🏝️ Reconnaissance de Surface");
    strcpy(systeme->quetes[QUETE_EXPLORATION_SURFACE].description, 
           "Familiarisez-vous avec les eaux de surface avant de plonger plus profond.");
    strcpy(systeme->quetes[QUETE_EXPLORATION_SURFACE].objectif, "Explorez 2 zones de surface");
    systeme->quetes[QUETE_EXPLORATION_SURFACE].type = QUETE_EXPLORATION_SURFACE;
    systeme->quetes[QUETE_EXPLORATION_SURFACE].statut = STATUT_ACTIVE;
    systeme->quetes[QUETE_EXPLORATION_SURFACE].recompense_xp = 50;
    systeme->quetes[QUETE_EXPLORATION_SURFACE].recompense_or = 100;
    systeme->quetes[QUETE_EXPLORATION_SURFACE].objectif_actuel = 0;
    systeme->quetes[QUETE_EXPLORATION_SURFACE].objectif_requis = 2;
    
    // Je crée la quête 2 : descendre en profondeur
    strcpy(systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].titre, "🌊 Premiers Pas dans les Profondeurs");
    strcpy(systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].description,
           "Il est temps de quitter la surface et d'explorer les zones plus profondes.");
    strcpy(systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].objectif, "Explorez le récif corallien");
    systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].type = QUETE_DESCENDRE_PROFONDEUR;
    systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].statut = STATUT_NON_ACTIVE;
    systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].recompense_xp = 100;
    systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].recompense_or = 200;
    systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].objectif_actuel = 0;
    systeme->quetes[QUETE_DESCENDRE_PROFONDEUR].objectif_requis = 1;
    
    // Je crée la quête 3 : combattre une créature
    strcpy(systeme->quetes[QUETE_COMBATTRE_CREATURE].titre, "⚔️ Gardien des Profondeurs");
    strcpy(systeme->quetes[QUETE_COMBATTRE_CREATURE].description,
           "Les créatures marines protègent leurs territoires. Montrez que vous êtes digne d'explorer.");
    strcpy(systeme->quetes[QUETE_COMBATTRE_CREATURE].objectif, "Battez 3 créatures marines");
    systeme->quetes[QUETE_COMBATTRE_CREATURE].type = QUETE_COMBATTRE_CREATURE;
    systeme->quetes[QUETE_COMBATTRE_CREATURE].statut = STATUT_NON_ACTIVE;
    systeme->quetes[QUETE_COMBATTRE_CREATURE].recompense_xp = 150;
    systeme->quetes[QUETE_COMBATTRE_CREATURE].recompense_or = 300;
    systeme->quetes[QUETE_COMBATTRE_CREATURE].objectif_actuel = 0;
    systeme->quetes[QUETE_COMBATTRE_CREATURE].objectif_requis = 3;
    
    // Je crée la quête 4 : explorer l'épave
    strcpy(systeme->quetes[QUETE_EXPLORER_EPAVE].titre, "💰 Trésor de l'Épave Perdue");
    strcpy(systeme->quetes[QUETE_EXPLORER_EPAVE].description,
           "Une épave mystérieuse contient des trésors anciens. Explorez-la pour découvrir ses secrets.");
    strcpy(systeme->quetes[QUETE_EXPLORER_EPAVE].objectif, "Explorez l'épave du navire");
    systeme->quetes[QUETE_EXPLORER_EPAVE].type = QUETE_EXPLORER_EPAVE;
    systeme->quetes[QUETE_EXPLORER_EPAVE].statut = STATUT_NON_ACTIVE;
    systeme->quetes[QUETE_EXPLORER_EPAVE].recompense_xp = 200;
    systeme->quetes[QUETE_EXPLORER_EPAVE].recompense_or = 500;
    systeme->quetes[QUETE_EXPLORER_EPAVE].objectif_actuel = 0;
    systeme->quetes[QUETE_EXPLORER_EPAVE].objectif_requis = 1;
    
    // Je crée la quête 5 : atteindre les abysses
    strcpy(systeme->quetes[QUETE_ATTEINDRE_ABYSSES].titre, "🐙 Dans les Abysses Interdites");
    strcpy(systeme->quetes[QUETE_ATTEINDRE_ABYSSES].description,
           "Les abysses recèlent des créatures et des secrets que peu ont pu observer.");
    strcpy(systeme->quetes[QUETE_ATTEINDRE_ABYSSES].objectif, "Atteignez les abysses (profondeur 150m)");
    systeme->quetes[QUETE_ATTEINDRE_ABYSSES].type = QUETE_ATTEINDRE_ABYSSES;
    systeme->quetes[QUETE_ATTEINDRE_ABYSSES].statut = STATUT_NON_ACTIVE;
    systeme->quetes[QUETE_ATTEINDRE_ABYSSES].recompense_xp = 300;
    systeme->quetes[QUETE_ATTEINDRE_ABYSSES].recompense_or = 750;
    systeme->quetes[QUETE_ATTEINDRE_ABYSSES].objectif_actuel = 0;
    systeme->quetes[QUETE_ATTEINDRE_ABYSSES].objectif_requis = 1;
    
    // Je crée la quête finale : battre le boss
    strcpy(systeme->quetes[QUETE_BOSS_FINAL].titre, "👑 Le Léviathan des Profondeurs");
    strcpy(systeme->quetes[QUETE_BOSS_FINAL].description,
           "Une créature légendaire règne dans les zones les plus profondes. Affrontez-la pour prouver votre maîtrise des océans.");
    strcpy(systeme->quetes[QUETE_BOSS_FINAL].objectif, "Battez le Léviathan des Abysses");
    systeme->quetes[QUETE_BOSS_FINAL].type = QUETE_BOSS_FINAL;
    systeme->quetes[QUETE_BOSS_FINAL].statut = STATUT_NON_ACTIVE;
    systeme->quetes[QUETE_BOSS_FINAL].recompense_xp = 500;
    systeme->quetes[QUETE_BOSS_FINAL].recompense_or = 1000;
    systeme->quetes[QUETE_BOSS_FINAL].objectif_actuel = 0;
    systeme->quetes[QUETE_BOSS_FINAL].objectif_requis = 1;
}

void verifier_progression_quetes(SystemeQuetes *systeme, Plongeur *joueur, CarteOceanique *carte) {
    if (!systeme || !joueur || !carte) return;
    
    // Je vérifie la quête active
    Quete *quete_courante = &systeme->quetes[systeme->quete_active];
    
    switch (quete_courante->type) {
        case QUETE_EXPLORATION_SURFACE: {
            // Je compte les zones de surface explorées
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
            // Je regarde si le récif a été exploré
            if (carte->grille[1][0].exploree && carte->grille[1][0].type == ZONE_RECIFS) {
                quete_courante->objectif_actuel = 1;
            }
            break;
        }
        
        case QUETE_EXPLORER_EPAVE: {
            // Je regarde si l'épave a été explorée
            if (carte->grille[1][1].exploree && carte->grille[1][1].type == ZONE_EPAVES) {
                quete_courante->objectif_actuel = 1;
            }
            break;
        }
        
        case QUETE_ATTEINDRE_ABYSSES: {
            // Je regarde si une zone d'abysses a été explorée
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
    
    // Je regarde si la quête est terminée
    if (quete_courante->objectif_actuel >= quete_courante->objectif_requis && 
        quete_courante->statut == STATUT_ACTIVE) {
        completer_quete(systeme, joueur, quete_courante->type);
    }
}

void afficher_quete_active(SystemeQuetes *systeme) {
    if (!systeme) return;
    
    if (systeme->quete_active >= NB_QUETES) {
        printf("🎊 Félicitations! Vous avez terminé toutes les quêtes!\n");
        return;
    }
    
    Quete *quete = &systeme->quetes[systeme->quete_active];
    
    printf("\n📋 === QUÊTE ACTIVE === 📋\n");
    printf("🎯 %s\n", quete->titre);
    printf("📝 %s\n", quete->description);
    printf("🎯 Objectif: %s (%d/%d)\n", quete->objectif, 
           quete->objectif_actuel, quete->objectif_requis);
    printf("💰 Récompense: %d XP + %d or\n", quete->recompense_xp, quete->recompense_or);
    
    // J'affiche la barre de progression
    int progression = (quete->objectif_actuel * 10) / quete->objectif_requis;
    printf("📊 Progression: [");
    for (int i = 0; i < 10; i++) {
        if (i < progression) printf("█");
        else printf("▱");
    }
    printf("] %d%%\n", (quete->objectif_actuel * 100) / quete->objectif_requis);
}

void completer_quete(SystemeQuetes *systeme, Plongeur *joueur, TypeQuete type) {
    if (!systeme || !joueur) return;
    
    Quete *quete = &systeme->quetes[type];
    quete->statut = STATUT_TERMINEE;
    systeme->nb_quetes_terminees++;
    
    printf("\n🎉 === QUÊTE TERMINÉE === 🎉\n");
    printf("🏆 %s\n", quete->titre);
    printf("✨ Vous avez gagné %d XP et %d or!\n", quete->recompense_xp, quete->recompense_or);
    
    // Je donne les récompenses au joueur
    joueur->xp += quete->recompense_xp;
    joueur->argent += quete->recompense_or;
    
    // J'active la quête suivante
    if (systeme->quete_active + 1 < NB_QUETES) {
        systeme->quete_active++;
        systeme->quetes[systeme->quete_active].statut = STATUT_ACTIVE;
        printf("📋 Nouvelle quête disponible: %s\n", 
               systeme->quetes[systeme->quete_active].titre);
    } else {
        printf("🎊 Vous avez terminé toutes les quêtes! Vous êtes un maître des océans!\n");
    }
}

int jeu_termine(SystemeQuetes *systeme) {
    if (!systeme) return 0;
    return systeme->nb_quetes_terminees >= NB_QUETES;
}