#include <stdio.h>

//Déclaration des types objet
typedef enum {
    TYPE_CONSOMMABLE,
    TYPE_EQUIPEMENT_HARPON,
    TYPE_EQUIPEMENT_COMBI
} TypeObjet;

typedef struct {
    int id;
    char nom[30];
    TypeObjet type;
    int quantite;
    int restaure_pv;
    int restaure_oxygene;
    int reduit_fatigue;
    int attaque_min;
    int attaque_max;
    int defense;
    int cout_oxygene;
    int est_vide;
} Objet;

typedef struct {
    Objet objets[8];
    int harpon_equipe;
    int combi_equipee;
    int perles;
} Inventaire;

//Fonctions inventaire.c
void initialiser_inventaire(Inventaire *inv);
void ouvrir_inventaire(Inventaire *inv);
int ajouter_objet(Inventaire *inv, Objet objet);

Objet creer_capsule_oxygene();
Objet creer_trousse_soin();
Objet creer_stimulant();
Objet creer_antidote();
Objet creer_harpon_rouille();
Objet creer_harpon_electrique();
Objet creer_combi_neoprene();

int main() {
    printf("========================================\n");
    printf("     OCEANDEPTH - Test Inventaire      \n");
    printf("========================================\n\n");

    Inventaire inventaire;

    //Initialiser (test)
    initialiser_inventaire(&inventaire);
    inventaire.perles = 127;

    //Ajouter des objets de départ (test)
    printf("Preparation de l'inventaire...\n");
    ajouter_objet(&inventaire, creer_capsule_oxygene());
    ajouter_objet(&inventaire, creer_capsule_oxygene());
    ajouter_objet(&inventaire, creer_capsule_oxygene());
    ajouter_objet(&inventaire, creer_trousse_soin());
    ajouter_objet(&inventaire, creer_stimulant());
    ajouter_objet(&inventaire, creer_antidote());
    ajouter_objet(&inventaire, creer_harpon_rouille());
    ajouter_objet(&inventaire, creer_combi_neoprene());

    // Boucle de jeu principale (simulation)
    int jouer = 1;
    while (jouer) {
        printf("\n=======================================\n");
        printf("         MENU PRINCIPAL                 \n");
        printf("========================================\n");
        printf("1 : Explorer les profondeurs\n");
        printf("2 : Ouvrir l'inventaire\n");
        printf("3 : Quitter le jeu\n");
        printf("\nVotre choix: ");

        int choix;
        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n');
            printf("Entree invalide!\n");
            continue;
        }
        while (getchar() != '\n');

        switch (choix) {
            case 1:
                printf("\nVous plongez dans les abysses...\n");
                printf("Vous trouvez un Harpon Electrique!\n");
                ajouter_objet(&inventaire, creer_harpon_electrique());
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;

            case 2:
                //Ouvrir l'inventaire
                ouvrir_inventaire(&inventaire);
                break;

            case 3:
                printf("\nMerci d'avoir joue!\n");
                jouer = 0;
                break;

            default:
                printf("Choix invalide!\n");
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
        }
    }

    return 0;
}