#include <stdio.h>
#include <string.h>

#define MAX_OBJETS 8
#define MAX_NOM 30

//Types objets
typedef enum {
    TYPE_CONSOMMABLE,
    TYPE_EQUIPEMENT_HARPON,
    TYPE_EQUIPEMENT_COMBI
} TypeObjet;

//Structure objet
typedef struct {
    int id;
    char nom[MAX_NOM];
    TypeObjet type;
    int quantite;

    //Attributs consommables
    int restaure_pv;
    int restaure_oxygene;
    int reduit_fatigue;

    //Attributs équipements
    int attaque_min;
    int attaque_max;
    int defense;
    int cout_oxygene;

    int est_vide; //1 si l'emplacement est vide, 0 sinon
} Objet;

// Structure inventaire
typedef struct {
    Objet objets[MAX_OBJETS];
    int harpon_equipe; // index de l'objet équipé (-1 si aucun)
    int combi_equipee; // index de l'objet équipé (-1 si aucun)
    int perles;
} Inventaire;




//Initialise un inventaire vide
void initialiser_inventaire(Inventaire *inv) {
    inv->perles = 0;
    inv->harpon_equipe = -1;
    inv->combi_equipee = -1;

    for (int i = 0; i < MAX_OBJETS; i++) {
        inv->objets[i].est_vide = 1;
        inv->objets[i].id = -1;
    }
}

//Affiche équipements actuels
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

    //Afficher stats
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

//Affiche l'inventaire complet
void afficher_inventaire(Inventaire *inv) {
    printf("\n");
    printf("+===================================================================+\n");
    printf("|              INVENTAIRE SOUS-MARIN                                |\n");
    printf("+===================================================================+\n");

    //Afficher les équipements
    afficher_equipement(inv);

    printf("+===================================================================+\n");
    printf("| OBJETS:                                      PERLES: %-4d        |\n", inv->perles);
    printf("+===================================================================+\n");

    //Afficher les objets dans l'inventaire (grille 2x4)
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

        //Afficher détails sur une deuxième ligne
        for (colonne = 0; colonne < 4; colonne++) {
            int index = ligne * 4 + colonne;

            if (colonne == 0) printf("| ");

            if (!inv->objets[index].est_vide) {
                Objet *obj = &inv->objets[index];

                if (obj->type == TYPE_CONSOMMABLE) {
                    if (obj->restaure_pv > 0) {
                        printf("   +%d PV          ", obj->restaure_pv);
                    } else if (obj->restaure_oxygene > 0) {
                        printf("   +%d O2          ", obj->restaure_oxygene);
                    } else if (obj->reduit_fatigue > 0) {
                        printf("   Fatigue -%d     ", obj->reduit_fatigue);
                    } else {
                        printf("   Antidote       ");
                    }
                } else {
                    printf("                   ");
                }
            } else {
                printf("                   ");
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

//Ajoute objet dans l'inventaire
int ajouter_objet(Inventaire *inv, Objet objet) {
    // Vérifier si l'objet existe déjà (pour les consommables)
    if (objet.type == TYPE_CONSOMMABLE) {
        for (int i = 0; i < MAX_OBJETS; i++) {
            if (!inv->objets[i].est_vide &&
                strcmp(inv->objets[i].nom, objet.nom) == 0) {
                inv->objets[i].quantite += objet.quantite;
                return 1;
            }
        }
    }

    //Trouver emplacement vide
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

//Utilise un objet consommable
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

//Fonction d'ouverture de l'inventaire
void ouvrir_inventaire(Inventaire *inv) {
    int choix = 0;
    int continuer = 1;

    while (continuer) {
        //Afficher l'inventaire
        afficher_inventaire(inv);

        //Demander choix
        printf("\nVotre choix: ");

        if (scanf("%d", &choix) != 1) {
            // Nettoyer le buffer en cas d'entrée invalide
            while (getchar() != '\n');
            printf("Entree invalide!\n");
            continue;
        }

        while (getchar() != '\n');

        switch (choix) {
            case 1: { //Utiliser objet
                int index;
                printf("Quel objet utiliser (1-8)? ");
                if (scanf("%d", &index) == 1) {
                    while (getchar() != '\n');
                    utiliser_objet(inv, index - 1); // -1 car affichage commence à 1
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar();
                } else {
                    while (getchar() != '\n');
                    printf("Entree invalide!\n");
                }
                break;
            }

            case 2: { //Équiper harpon
                int index;
                printf("Quel harpon equiper (1-8)? ");
                if (scanf("%d", &index) == 1) {
                    while (getchar() != '\n');
                    if (inv->objets[index - 1].type == TYPE_EQUIPEMENT_HARPON && !inv->objets[index - 1].est_vide) {
                        equiper_objet(inv, index - 1);
                    } else {
                        printf("Cet objet n'est pas un harpon!\n");
                    }
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar();
                } else {
                    while (getchar() != '\n');
                    printf("Entree invalide!\n");
                }
                break;
            }

            case 3: { //Équiper combi
                int index;
                printf("Quelle combinaison equiper (1-8)? ");
                if (scanf("%d", &index) == 1) {
                    while (getchar() != '\n');
                    if (inv->objets[index - 1].type == TYPE_EQUIPEMENT_COMBI && !inv->objets[index - 1].est_vide) {
                        equiper_objet(inv, index - 1);
                    } else {
                        printf("[X] Cet objet n'est pas une combinaison!\n");
                    }
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar();
                } else {
                    while (getchar() != '\n');
                    printf("Entree invalide!\n");
                }
                break;
            }

            case 4: //Retour
                printf("Fermeture de l'inventaire...\n");
                continuer = 0;
                break;

            default:
                printf("Choix invalide! Choisissez entre 1 et 4.\n");
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
        }
    }
}