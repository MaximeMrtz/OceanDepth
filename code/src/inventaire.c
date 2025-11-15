
// Inclusions standards
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclusions du projet
#include "inventaire.h"

typedef struct {
    char nom[24];
    char effet[24];
    int quantite;
} ObjetAffichage;

typedef struct {
    char nom[24];
    char effet[24];
    int bonus;
} EquipementAffichage;

// J'affiche l'inventaire avec des emojis et les effets
void afficher_inventaire_avance(Inventaire *inv) {
    printf("\nINVENTAIRE SOUS-MARIN\n");
    printf("┌───────────────┬───────────────┬───────────────┐\n");
    printf("│ 🗡️ Harpon: %-8s │ 🛡️ Combi: %-8s │ 💎 Perles: %-4d │\n",
           (inv->harpon_equipe != -1 && !inv->objets[inv->harpon_equipe].est_vide) ? inv->objets[inv->harpon_equipe].nom : "Aucun",
           (inv->combi_equipee != -1 && !inv->objets[inv->combi_equipee].est_vide) ? inv->objets[inv->combi_equipee].nom : "Aucune",
           inv->perles);
    printf("├───────────────┼───────────────┼───────────────┤\n");
    printf("│ ATK: %-9s │ DEF: %-9s │\n",
           (inv->harpon_equipe != -1 && !inv->objets[inv->harpon_equipe].est_vide) ? "Oui" : "Non",
           (inv->combi_equipee != -1 && !inv->objets[inv->combi_equipee].est_vide) ? "Oui" : "Non");
    printf("└───────────────┴───────────────┴───────────────┘\n");

    printf("Objets :\n");
    printf("┌─────────────────────────────┬─────────────────────────────┐\n");
    for (int i = 0; i < MAX_OBJETS; i += 2) {
    // J'affiche la première colonne
        if (!inv->objets[i].est_vide) {
            printf("│ [%d] %-12s x%-2d %-10s ", i+1, inv->objets[i].nom, inv->objets[i].quantite, inv->objets[i].type == TYPE_CONSOMMABLE ? "🧪" : (inv->objets[i].type == TYPE_EQUIPEMENT_HARPON ? "🗡️" : (inv->objets[i].type == TYPE_EQUIPEMENT_COMBI ? "🛡️" : "?")));
        } else {
            printf("│ [%d] %-25s ", i+1, "Vide");
        }
    // J'affiche la deuxième colonne
        if (i+1 < MAX_OBJETS && !inv->objets[i+1].est_vide) {
            printf("│ [%d] %-12s x%-2d %-10s │\n", i+2, inv->objets[i+1].nom, inv->objets[i+1].quantite, inv->objets[i+1].type == TYPE_CONSOMMABLE ? "🧪" : (inv->objets[i+1].type == TYPE_EQUIPEMENT_HARPON ? "🗡️" : (inv->objets[i+1].type == TYPE_EQUIPEMENT_COMBI ? "🛡️" : "?")));
        } else if (i+1 < MAX_OBJETS) {
            printf("│ [%d] %-25s │\n", i+2, "Vide");
        } else {
            printf("│                             │\n");
        }
    }
    printf("└─────────────────────────────┴─────────────────────────────┘\n");
    printf("\n1 : Utiliser objet  2 : Equiper harpon  3 : Equiper combi  4 : Retour\n");
}




// J'affiche les équipements actuels
void afficher_equipement(Inventaire *inv) {
    printf("| EQUIPE: HARPON              | EQUIPE: COMBINAISON                |\n");
    printf("| ");

    if (inv->harpon_equipe != -1) {
        Objet *harpon = &inv->objets[inv->harpon_equipe];
        printf("%-27s", harpon->nom);
    } else {
        printf("Aucun                      ");
    }

    printf(" | ");

    if (inv->combi_equipee != -1) {
        Objet *combi = &inv->objets[inv->combi_equipee];
        printf("%-34s", combi->nom);
    } else {
        printf("Aucune                            ");
    }

    printf(" |\n");

    // J'affiche les stats
    printf("| ");
    if (inv->harpon_equipe != -1) {
        Objet *harpon = &inv->objets[inv->harpon_equipe];
        printf("ATK: %d-%d, O2: -%d/att  ",
               harpon->attaque_min, harpon->attaque_max, harpon->cout_oxygene);
    } else {
        printf("                           ");
    }

    printf(" | ");

    if (inv->combi_equipee != -1) {
        Objet *combi = &inv->objets[inv->combi_equipee];
        printf("DEF: +%d, O2: -%d/tour            ",
               combi->defense, combi->cout_oxygene);
    } else {
        printf("                                  ");
    }

    printf(" |\n");
}

// J'affiche tout l'inventaire
void afficher_inventaire(Inventaire *inv) {
    printf("\n");
    printf("| OBJETS:                                      PERLES: %-4d        |\n", inv->perles);
    printf("+===================================================================+\n");

    // J'affiche les objets dans l'inventaire (grille 2x4)
    int ligne, colonne;
    for (ligne = 0; ligne < 2; ligne++) {
        for (colonne = 0; colonne < 4; colonne++) {
            int index = ligne * 4 + colonne;

            if (colonne == 0) printf("| ");

            if (inv->objets[index].est_vide) {
                printf("[%d] Vide           ", index + 1);
            } else {
                Objet *obj = &inv->objets[index];

                if (obj->type == TYPE_CONSOMMABLE) {
                    printf("[%d] %-12s (x%d)", index + 1, obj->nom, obj->quantite);
                } else {
                    printf("[%d] %-12s     ", index + 1, obj->nom);
                }
            }

            if (colonne < 3) {
                printf(" | ");
            } else {
                printf(" |\n");
            }
        }
        if (ligne == 0) {
            printf("+===================================================================+\n");
        }
    }

    printf("+===================================================================+\n");
    printf("\n1 : Utiliser objet  2 : Equiper harpon  3 : Equiper combi  4 : Retour\n");
}

// J'initialise un inventaire vide
void initialiser_inventaire(Inventaire *inv) {
    inv->perles = 0;
    inv->harpon_equipe = -1;
    inv->combi_equipee = -1;
    for (int i = 0; i < MAX_OBJETS; i++) {
        inv->objets[i].est_vide = 1;
        inv->objets[i].id = -1;
    }
}

// Fonction pour libérer l'inventaire (pour l'instant rien à faire)
void liberer_inventaire(Inventaire *inv) {
    // Wrapper pour compatibilité, rien à libérer ici
    (void)inv;
}

// J'ajoute un objet dans l'inventaire
int ajouter_objet(Inventaire *inv, Objet objet) {
    // Je vérifie si l'objet existe déjà (pour les consommables)
    if (objet.type == TYPE_CONSOMMABLE) {
        for (int i = 0; i < MAX_OBJETS; i++) {
            if (!inv->objets[i].est_vide &&
                strcmp(inv->objets[i].nom, objet.nom) == 0) {
                inv->objets[i].quantite += objet.quantite;
                return 1;
            }
        }
    }

    // Je cherche un emplacement vide
    for (int i = 0; i < MAX_OBJETS; i++) {
        if (inv->objets[i].est_vide) {
            inv->objets[i] = objet;
            inv->objets[i].est_vide = 0;
            return 1;
        }
    }

    printf("Inventaire plein!\n");
    return 0;
}

// J'utilise un objet consommable
int utiliser_objet(Inventaire *inv, int index) {
    if (index < 0 || index >= MAX_OBJETS) return 0;
    if (inv->objets[index].est_vide) return 0;

    Objet *obj = &inv->objets[index];

    if (obj->type != TYPE_CONSOMMABLE) {
        printf("Cet objet ne peut pas etre utilise!\n");
        return 0;
    }

    printf("Vous utilisez %s\n", obj->nom);

    obj->quantite--;

    if (obj->quantite <= 0) {
        obj->est_vide = 1;
    }

    return 1;
}

int equiper_objet(Inventaire *inv, int index) {
    if (index < 0 || index >= MAX_OBJETS) return 0;
    if (inv->objets[index].est_vide) return 0;

    Objet *obj = &inv->objets[index];

    if (obj->type == TYPE_EQUIPEMENT_HARPON) {
        inv->harpon_equipe = index;
        printf("Vous equipez: %s\n", obj->nom);
        return 1;
    } else if (obj->type == TYPE_EQUIPEMENT_COMBI) {
        inv->combi_equipee = index;
        printf("Vous equipez: %s\n", obj->nom);
        return 1;
    } else {
        printf("Cet objet ne peut pas etre equipe!\n");
        return 0;
    }
}

Objet creer_capsule_oxygene() {
    Objet obj = {0};
    strcpy(obj.nom, "Capsule O2");
    obj.type = TYPE_CONSOMMABLE;
    obj.quantite = 1;
    obj.restaure_oxygene = 40;
    obj.est_vide = 0;
    return obj;
}

Objet creer_trousse_soin() {
    Objet obj = {0};
    strcpy(obj.nom, "Trousse Soin");
    obj.type = TYPE_CONSOMMABLE;
    obj.quantite = 1;
    obj.restaure_pv = 25;
    obj.est_vide = 0;
    return obj;
}

Objet creer_stimulant() {
    Objet obj = {0};
    strcpy(obj.nom, "Stimulant");
    obj.type = TYPE_CONSOMMABLE;
    obj.quantite = 1;
    obj.reduit_fatigue = 2;
    obj.est_vide = 0;
    return obj;
}

Objet creer_antidote() {
    Objet obj = {0};
    strcpy(obj.nom, "Antidote");
    obj.type = TYPE_CONSOMMABLE;
    obj.quantite = 1;
    obj.est_vide = 0;
    return obj;
}

Objet creer_harpon_rouille() {
    Objet obj = {0};
    strcpy(obj.nom, "Harpon Rouille");
    obj.type = TYPE_EQUIPEMENT_HARPON;
    obj.attaque_min = 12;
    obj.attaque_max = 18;
    obj.cout_oxygene = 2;
    obj.est_vide = 0;
    return obj;
}

Objet creer_harpon_electrique() {
    Objet obj = {0};
    strcpy(obj.nom, "Harpon Electrique");
    obj.type = TYPE_EQUIPEMENT_HARPON;
    obj.attaque_min = 28;
    obj.attaque_max = 42;
    obj.cout_oxygene = 4;
    obj.est_vide = 0;
    return obj;
}

Objet creer_combi_neoprene() {
    Objet obj = {0};
    strcpy(obj.nom, "Neoprene Basic");
    obj.type = TYPE_EQUIPEMENT_COMBI;
    obj.defense = 5;
    obj.cout_oxygene = 1;
    obj.est_vide = 0;
    return obj;
}

Objet creer_combi_titanium() {
    Objet obj = {0};
    strcpy(obj.nom, "Armure Titanium");
    obj.type = TYPE_EQUIPEMENT_COMBI;
    obj.defense = 25;
    obj.cout_oxygene = 2;
    obj.est_vide = 0;
    return obj;
}

// Fonction pour ouvrir l'inventaire
void ouvrir_inventaire(Inventaire *inv) {
    int choix = 0;
    int continuer = 1;
    while (continuer) {
    afficher_inventaire_avance(inv);
        printf("\nVotre choix: ");
        char buffer[16];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("❌ Entrée invalide !\n");
            continue;
        }
    // Je vérifie la conversion
        char *endptr = NULL;
        choix = (int)strtol(buffer, &endptr, 10);
        if (endptr == buffer || (*endptr != '\n' && *endptr != '\0')) {
            printf("❌ Veuillez entrer un nombre entre 1 et 4.\n");
            continue;
        }
        switch (choix) {
            case 1: { // Utiliser un objet
                printf("Quel objet utiliser (1-8)? ");
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    char *endptr2 = NULL;
                    int index = (int)strtol(buffer, &endptr2, 10);
                    if (endptr2 == buffer || (*endptr2 != '\n' && *endptr2 != '\0')) {
                        printf("❌ Veuillez entrer un nombre entre 1 et 8.\n");
                    } else if (index >= 1 && index <= 8) {
                        utiliser_objet(inv, index - 1);
                        printf("\nAppuyez sur Entrée pour continuer...");
                        fgets(buffer, sizeof(buffer), stdin);
                    } else {
                        printf("❌ Index invalide ! (1-8)\n");
                    }
                } else {
                    printf("❌ Entrée invalide !\n");
                }
                break;
            }

            case 2: { // Équiper le harpon
                printf("Quel harpon équiper (1-8)? ");
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    char *endptr2 = NULL;
                    int index = (int)strtol(buffer, &endptr2, 10);
                    if (endptr2 == buffer || (*endptr2 != '\n' && *endptr2 != '\0')) {
                        printf("❌ Veuillez entrer un nombre entre 1 et 8.\n");
                    } else if (index >= 1 && index <= 8 && !inv->objets[index - 1].est_vide) {
                        if (inv->objets[index - 1].type == TYPE_EQUIPEMENT_HARPON) {
                            equiper_objet(inv, index - 1);
                        } else {
                            printf("❌ Cet objet n'est pas un harpon !\n");
                        }
                    } else {
                        printf("❌ Index invalide ou emplacement vide !\n");
                    }
                    printf("\nAppuyez sur Entrée pour continuer...");
                    fgets(buffer, sizeof(buffer), stdin);
                } else {
                    printf("❌ Entrée invalide !\n");
                }
                break;
            }

            case 3: { // Équiper la combinaison
                printf("Quelle combinaison équiper (1-8)? ");
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    char *endptr2 = NULL;
                    int index = (int)strtol(buffer, &endptr2, 10);
                    if (endptr2 == buffer || (*endptr2 != '\n' && *endptr2 != '\0')) {
                        printf("❌ Veuillez entrer un nombre entre 1 et 8.\n");
                    } else if (index >= 1 && index <= 8 && !inv->objets[index - 1].est_vide) {
                        if (inv->objets[index - 1].type == TYPE_EQUIPEMENT_COMBI) {
                            equiper_objet(inv, index - 1);
                        } else {
                            printf("❌ Cet objet n'est pas une combinaison !\n");
                        }
                    } else {
                        printf("❌ Index invalide ou emplacement vide !\n");
                    }
                    printf("\nAppuyez sur Entrée pour continuer...");
                    fgets(buffer, sizeof(buffer), stdin);
                } else {
                    printf("❌ Entrée invalide !\n");
                }
                break;
            }

            case 4: // Retour au menu
                printf("Fermeture de l'inventaire...\n");
                continuer = 0;
                break;
            default:
                printf("❌ Choix invalide ! Choisissez entre 1 et 4.\n");
                printf("\nAppuyez sur Entrée pour continuer...");
                fgets(buffer, sizeof(buffer), stdin);
                break;
        }
    }
}