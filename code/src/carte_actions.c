// Mouvement, exploration et interactions avec l'environnement
 

// Fichier pour gérer les actions du joueur sur la carte (déplacement, exploration, etc)


// Inclusions standards
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclusions du projet
#include "carte.h"
#include "joueur.h"
#include "quetes.h"
#include "inventaire.h"
#include "creatures.h"
#include "combat.h"

// Fonction pour déplacer le joueur sur la carte
int deplacer_joueur(CarteOceanique *carte, Direction direction, Plongeur *joueur) {
    if (!carte || !joueur) return -1;
    
    int nouveau_x = carte->position_joueur.x;
    int nouveau_y = carte->position_joueur.y;
    
    // Je calcule la nouvelle position selon la direction
    switch (direction) {
        case DIR_NORD:
            nouveau_y--;
            printf("🧭 Vous nagez vers le NORD...\n");
            break;
        case DIR_SUD:
            nouveau_y++;
            printf("🧭 Vous nagez vers le SUD...\n");
            break;
        case DIR_EST:
            nouveau_x++;
            printf("🧭 Vous nagez vers l'EST...\n");
            break;
        case DIR_OUEST:
            nouveau_x--;
            printf("🧭 Vous nagez vers l'OUEST...\n");
            break;
        default:
            printf("❌ Direction invalide!\n");
            return -1;
    }
    
    // Je vérifie si on sort de la carte
    if (nouveau_x < 0 || nouveau_x >= TAILLE_CARTE || 
        nouveau_y < 0 || nouveau_y >= TAILLE_CARTE) {
        printf("🚫 Vous ne pouvez pas aller plus loin dans cette direction!\n");
        printf("⚠️  Vous êtes aux limites de la zone océanique explorée.\n");
        return -1;
    }
    
    Zone *nouvelle_zone = &carte->grille[nouveau_y][nouveau_x];
    
    // Je vérifie si la zone est accessible
    if (!nouvelle_zone->debroquee) {
        printf("🔒 Cette zone n'est pas encore accessible!\n");
        printf("💡 Explorez les zones adjacentes pour débloquer de nouvelles zones.\n");
        return -1;
    }
    
    // Je vérifie si la zone est vide
    if (nouvelle_zone->type == ZONE_VIDE) {
        printf("🌊 Cette zone est vide... Vous revenez à votre position.\n");
        return -1;
    }
    
    // Je fais le déplacement
    carte->position_joueur.x = nouveau_x;
    carte->position_joueur.y = nouveau_y;
    carte->position_joueur.zone_actuelle = nouvelle_zone->type;
    
    // J'affiche où on arrive
    printf("\n🌊 === NOUVELLE ZONE ATTEINTE ===\n");
    printf("📍 Vous arrivez à: %s\n", nouvelle_zone->nom);
    printf("🌊 Profondeur: %dm\n", nouvelle_zone->profondeur);
    
    // Je gère la perte d'oxygène selon la profondeur
    int perte_oxygene = nouvelle_zone->profondeur / 50;
    if (perte_oxygene > 0) {
        joueur->niveau_oxygene -= perte_oxygene;
        if (joueur->niveau_oxygene < 0) joueur->niveau_oxygene = 0;
        printf("💨 Oxygène: %d (-%d à cette profondeur)\n", joueur->niveau_oxygene, perte_oxygene);
        
        if (joueur->niveau_oxygene <= 10) {
            printf("⚠️  🚨 ATTENTION: Niveau d'oxygène critique!\n");
        }
    }
    
    // Rien à faire ici, le déblocage se fait à l'exploration
    
    return 0;
}

// Fonction pour explorer la zone où on est
int explorer_zone(CarteOceanique *carte, Plongeur *joueur) {
    if (!carte || !joueur) return -1;
    
    Zone *zone = get_zone_actuelle(carte);
    if (joueur->inventaire) {
        joueur->inventaire->zone_actuelle = zone;
    }
    if (!zone) {
        printf("❌ Impossible de déterminer la zone actuelle!\n");
        return -1;
    }
    
    printf("\n🔍 === EXPLORATION DE LA ZONE ===\n");
    printf("📍 Zone: %s\n", zone->nom);
    
    // Je regarde si la zone a déjà été explorée
    if (zone->exploree) {
        printf("✅ Cette zone a déjà été explorée.\n");
        printf("💭 Vous reconnaissez les lieux...\n");
        if (zone->nb_creatures > 0) {
            printf("🦈 Vous observez encore %d créature(s) dans la zone.\n", zone->nb_creatures);
        }
        printf("🏷️  Nom de la zone : %s\n", zone->nom);
        return 0;
    }

    // Si c'est la première fois qu'on explore
    printf("🚀 Première exploration de cette zone!\n");
    printf("🔍 Vous examinez attentivement les environs...\n");

    // Je marque la zone comme explorée et je débloque autour
    zone->exploree = 1;
    debloquer_zones_adjacentes(carte, carte->position_joueur.x, carte->position_joueur.y);
    carte->zones_decouvertes++;

    // Je change le nom de la zone selon la créature si besoin
    if (zone->nb_creatures > 0) {
        if (zone->profondeur >= 150) {
            strcpy(zone->nom, "Repaire du Requin");
        } else if (zone->profondeur >= 50) {
            strcpy(zone->nom, "Grotte du Crabe");
        } else {
            strcpy(zone->nom, "Banc de Poissons");
        }
    } else {
    // Si c'est calme, j'affiche un message
        if (zone->type == ZONE_FORET_ALGUES) strcpy(zone->nom, "Forêt paisible");
        else if (zone->type == ZONE_EPAVES) strcpy(zone->nom, "Épave sécurisée");
        else if (zone->type == ZONE_GROTTES) strcpy(zone->nom, "Grottes calmes");
        else if (zone->type == ZONE_FOSSES) strcpy(zone->nom, "Fosse tranquille");
        else if (zone->type == ZONE_RECIFS) strcpy(zone->nom, "Récif paisible");
        else strcpy(zone->nom, "Zone paisible");
    }

    // Je donne de l'XP pour l'exploration
    int xp_gagne = 10 + zone->profondeur / 10;
    joueur->xp += xp_gagne;
    printf("✨ +%d XP d'exploration!\n", xp_gagne);

    // J'affiche une description selon le type de zone
    switch (zone->type) {
        case ZONE_SURFACE:
            printf("🏝️ Vous êtes à la base sous-marine. Zone sûre.\n");
            break;
        case ZONE_RECIFS:
            printf("🐠 Magnifiques récifs coralliens! Vie marine abondante.\n");
            break;
        case ZONE_EPAVES:
            printf("💰 Épave d'un ancien navire! Des trésors peuvent s'y cacher.\n");
            break;
        case ZONE_FORET_ALGUES:
            printf("🌿 Dense forêt d'algues géantes. Navigation difficile.\n");
            break;
        case ZONE_GROTTES:
            printf("🕳️ Grottes mystérieuses. L'écho résonne étrangement...\n");
            break;
        case ZONE_FOSSES:
            printf("🐙 Fosses abyssales profondes. Ténèbres inquiétantes...\n");
            break;
        default:
            printf("❓ Zone mystérieuse aux propriétés inconnues.\n");
            break;
    }


    // Ici je gère les événements spéciaux et les boss

    int boss_possible = (carte->zones_decouvertes > 5); // Boss seulement après 5 zones découvertes
    int evenement = rand() % 100;
    if (boss_possible && evenement < 10 && zone->nb_creatures == 0 && zone->profondeur >= 100) {
    // 10% de chance qu'un boss apparaisse si on a assez exploré
        int boss_id = rand() % 3;
        if (boss_id == 0) {
            printf("\n⚠️  Le Léviathan surgit dans les abysses !\n");
            strcpy(zone->nom, "Antre du Léviathan");
        } else if (boss_id == 1) {
            printf("\n⚠️  Le Kraken géant agite les profondeurs !\n");
            strcpy(zone->nom, "Domaine du Kraken");
        } else {
            printf("\n⚠️  Le Requin Fantôme hante la zone !\n");
            strcpy(zone->nom, "Lagon du Requin Fantôme");
        }
        zone->nb_creatures = 1;
        zone->type_creature = CREATURE_TYPE_BOSS;
        zone->boss_id = boss_id;
    } else if (evenement < 20 && zone->nb_creatures == 0) {
    // 10% de chance d'événement spécial (coffre, bonus, etc)
        int type_evt = rand() % 5;
        switch(type_evt) {
            case 0:
                printf("\n🎁 Vous découvrez un coffre mystérieux ! Vous trouvez une capsule d'oxygène et quelques perles.\n");
                if (joueur->inventaire) {
                    ajouter_objet(joueur->inventaire, creer_capsule_oxygene());
                    joueur->perles += 15;
                }
                break;
            case 1:
                printf("\n🌪️ Une tempête soudaine vous fait perdre 10 O2 !\n");
                joueur->niveau_oxygene -= 10;
                if (joueur->niveau_oxygene < 0) joueur->niveau_oxygene = 0;
                break;
            case 2:
                printf("\n🪤 Un piège naturel ! Vous perdez 5 PV.\n");
                joueur->points_de_vie -= 5;
                if (joueur->points_de_vie < 0) joueur->points_de_vie = 0;
                break;
            case 3:
                printf("\n🤝 Vous croisez un plongeur amical qui partage un stimulant !\n");
                if (joueur->inventaire) ajouter_objet(joueur->inventaire, creer_stimulant());
                break;
            case 4:
                printf("\n✨ Vous trouvez un artefact ancien : +20 XP !\n");
                joueur->xp += 20;
                break;
        }
    }

    // Je gère les rencontres de créatures
    if (zone->nb_creatures > 0) {
        if (zone->type_creature == CREATURE_TYPE_BOSS)
            printf("\n👹 ATTENTION: Un BOSS légendaire vous attend ici !\n");
        else
            printf("\n🦈 ATTENTION: %d créature(s) hostile(s) détectée(s)!\n", zone->nb_creatures);
        printf("⚔️  Préparez-vous au combat ou fuyez!\n");
    } else {
        printf("✅ Zone paisible, aucune menace détectée.\n");
    }

    // Je vérifie la progression des quêtes
    // (Appel possible à la fonction de progression de quêtes)

    return 0;
}

// Fonction pour gérer le combat contre les créatures de la zone
int combattre_creatures_zone(CarteOceanique *carte, Plongeur *joueur, Inventaire *inventaire) {
    if (!carte || !joueur) return -1;
    Zone *zone = get_zone_actuelle(carte);
    if (inventaire) {
        inventaire->zone_actuelle = zone;
    }
    if (!zone) return -1;
    if (zone->nb_creatures <= 0) {
        printf("✅ Aucune créature hostile dans cette zone.\n");
    // Si la zone n'est pas déjà paisible, je la renomme
        if (zone->exploree) {
            if (zone->type == ZONE_FORET_ALGUES) strcpy(zone->nom, "Forêt paisible");
            else if (zone->type == ZONE_EPAVES) strcpy(zone->nom, "Épave sécurisée");
            else if (zone->type == ZONE_GROTTES) strcpy(zone->nom, "Grottes calmes");
            else if (zone->type == ZONE_FOSSES) strcpy(zone->nom, "Fosse tranquille");
            else if (zone->type == ZONE_RECIFS) strcpy(zone->nom, "Récif paisible");
            else strcpy(zone->nom, "Zone paisible");
        }
        return 0;
    }

    printf("\n⚔️ === COMBAT DANS LA ZONE ===\n");
    printf("📍 Zone: %s (%dm de profondeur)\n", zone->nom, zone->profondeur);
    printf("🦈 %d créature(s) hostile(s) à affronter!\n", zone->nb_creatures);

    // Je crée la liste des créatures présentes
    CreatureList liste;
    creatures_init(&liste);
    for (int i = 0; i < zone->nb_creatures; ++i) {
    // Je choisis le type de créature selon la profondeur
        CreatureType type = CREATURE_TYPE_FISH;
        if (zone->profondeur >= 150) type = CREATURE_TYPE_SHARK;
        else if (zone->profondeur >= 50) type = CREATURE_TYPE_CRAB;
        creature_new(&liste, type, carte->position_joueur.x, carte->position_joueur.y);
    }

    int creatures_vaincues = 0;
    while (1) {
    // Je regarde s'il reste des créatures vivantes
        int vivantes = 0;
        for (Creature *c = liste.head; c; c = c->next) {
            if (c->alive && c->hp > 0) vivantes++;
        }
        if (vivantes == 0) break;

        tour_de_combat(&liste, joueur, zone->profondeur);

    // Si le joueur meurt ou fuit
        if (joueur->points_de_vie <= 0) {
            printf("💀 Vous avez été vaincu et devez fuir!\n");
            carte->position_joueur.x = 0;
            carte->position_joueur.y = 0;
            carte->position_joueur.zone_actuelle = ZONE_SURFACE;
            joueur->points_de_vie = joueur->points_de_vie_max / 2;
            break;
        }
    }

    // Je compte combien de créatures ont été vaincues
    for (Creature *c = liste.head; c; c = c->next) {
        if (!c->alive || c->hp <= 0) creatures_vaincues++;
    }
    zone->nb_creatures -= creatures_vaincues;
    if (zone->nb_creatures < 0) zone->nb_creatures = 0;

    if (creatures_vaincues > 0) {
        int recompense_perles = 10 + rand() % 20;
        int recompense_xp = 25 + rand() % 25;
        joueur->perles += recompense_perles;
        joueur->argent += recompense_perles;
        if (inventaire) inventaire->perles += recompense_perles;
        joueur->xp += recompense_xp;
        printf("💰 +%d perles, +%d XP\n", recompense_perles, recompense_xp);
        printf("\n🏆 Zone sécurisée! %d créature(s) vaincue(s)\n", creatures_vaincues);
    // Je renomme la zone selon la créature vaincue
        if (zone->nb_creatures == 0 && zone->exploree) {
            if (zone->profondeur >= 150) {
                strcpy(zone->nom, "Repaire du Requin");
            } else if (zone->profondeur >= 50) {
                strcpy(zone->nom, "Grotte du Crabe");
            } else {
                strcpy(zone->nom, "Banc de Poissons");
            }
        }
    // 30% de chance d'obtenir un loot
        if (inventaire && (rand() % 100 < 30)) {
            Objet loot;
            int type_loot = rand() % 4;
            switch (type_loot) {
                case 0: loot = creer_capsule_oxygene(); break;
                case 1: loot = creer_trousse_soin(); break;
                case 2: loot = creer_stimulant(); break;
                case 3: loot = creer_antidote(); break;
            }
            if (ajouter_objet(inventaire, loot)) {
                printf("🎁 Vous trouvez un %s!\n", loot.nom);
            } else {
                printf("🎁 Vous trouvez un %s mais votre inventaire est plein!\n", loot.nom);
            }
        }
    }

    // Je nettoie la liste des créatures
    creatures_clear(&liste);

    return creatures_vaincues;
}

// Fonction pour récupérer de l'oxygène (seulement en surface)
int recuperer_oxygene(CarteOceanique *carte, Plongeur *joueur) {
    if (!carte || !joueur) return -1;
    
    Zone *zone = get_zone_actuelle(carte);
    if (!zone || zone->type != ZONE_SURFACE) {
        printf("💨 Vous ne pouvez récupérer de l'oxygène qu'en surface!\n");
        printf("🏃 Retournez à la base sous-marine (0,0)\n");
        return -1;
    }
    
    printf("\n💨 === RÉCUPÉRATION D'OXYGÈNE ===\n");
    printf("🏝️ Vous émergez à la surface...\n");
    
    int oxygene_avant = joueur->niveau_oxygene;
    joueur->niveau_oxygene = joueur->niveau_oxygene_max; // Je remets l'oxygène au max
    
    printf("✅ Oxygène rechargé: %d → %d\n", oxygene_avant, joueur->niveau_oxygene);
    printf("😌 Vous prenez une profonde inspiration d'air frais.\n");
    
    return 0;
}

// Fonction pour afficher les déplacements possibles
void afficher_deplacements_possibles(CarteOceanique *carte) {
    if (!carte) return;
    
    printf("\n🧭 === DÉPLACEMENTS POSSIBLES ===\n");
    int x = carte->position_joueur.x;
    int y = carte->position_joueur.y;
    
    // Je regarde chaque direction possible
    const char* noms_dir[4] = {"NORD", "SUD", "EST", "OUEST"};
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {-1, 1, 0, 0};
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        if (nx >= 0 && nx < TAILLE_CARTE && ny >= 0 && ny < TAILLE_CARTE) {
            Zone *zone_cible = &carte->grille[ny][nx];
            
            if (zone_cible->debroquee && zone_cible->type != ZONE_VIDE) {
                printf("✅ %s - %s (%dm)\n", noms_dir[i], zone_cible->nom, zone_cible->profondeur);
            } else if (!zone_cible->debroquee) {
                printf("🔒 %s - Zone verrouillée\n", noms_dir[i]);
            } else {
                printf("❌ %s - Zone vide\n", noms_dir[i]);
            }
        } else {
            printf("🚫 %s - Limites de la carte\n", noms_dir[i]);
        }
    }
}