// Fonctions pour sauvegarder et charger la partie (j'ai deplace ca depuis carte.c)

// Inclusions standards
#include <stdio.h>
#include <string.h>

// Inclusions du projet
#include "sauvegarde.h"
#include "carte.h"
#include "inventaire.h"
#include "quetes.h"
#include "creatures.h"

// Je sauvegarde toute la partie
void sauvegarder_partie_complete(CarteOceanique *carte, Plongeur *joueur, CreatureList *creatures, SystemeQuetes *quetes, Inventaire *inv) {
	// J'ouvre le fichier de sauvegarde
	FILE *fichier = fopen("partie_oceandepth.save", "w");
	if (!fichier) {
		printf("Erreur lors de la creation du fichier de sauvegarde!\n");
		return;
	}
	// J'ecris l'en-tete de sauvegarde
	fprintf(fichier, "OCEANDEPTH_SAVE_V2\n");
	// Je sauvegarde le joueur
	if (sauvegarder_joueur(fichier, joueur) != 0) {
		printf("Erreur lors de la sauvegarde du joueur!\n");
		fclose(fichier);
		return;
	}
	// Je sauvegarde les creatures
	if (creatures && creatures_save(fichier, creatures) != 0) {
		printf("Erreur lors de la sauvegarde des creatures!\n");
		fclose(fichier);
		return;
	}
	// Je sauvegarde la carte
	if (sauvegarder_carte(fichier, carte) != 0) {
		printf("Erreur lors de la sauvegarde de la carte!\n");
		fclose(fichier);
		return;
	}
	// Je sauvegarde l'inventaire
	fprintf(fichier, "INVENTAIRE_START\n");
	fprintf(fichier, "%d %d %d\n", inv->harpon_equipe, inv->combi_equipee, inv->perles);
	for (int i = 0; i < MAX_OBJETS; i++) {
		Objet *obj = &inv->objets[i];
		fprintf(fichier, "%d %s %d %d %d %d %d %d %d %d %d %d\n",
				obj->est_vide, obj->nom, obj->type, obj->quantite,
				obj->restaure_pv, obj->restaure_oxygene, obj->reduit_fatigue,
				obj->attaque_min, obj->attaque_max, obj->defense, obj->cout_oxygene, obj->id);
	}
	// Je termine la sauvegarde de l'inventaire et des quetes
	fprintf(fichier, "INVENTAIRE_END\n");
	fprintf(fichier, "QUETES_START\n");
	fprintf(fichier, "%d %d\n", quetes->quete_active, quetes->nb_quetes_terminees);
	for (int i = 0; i < NB_QUETES; i++) {
		Quete *q = &quetes->quetes[i];
		fprintf(fichier, "%d %d %d %d %d %d\n",
				q->type, q->statut, q->recompense_xp, q->recompense_or,
				q->objectif_actuel, q->objectif_requis);
	}
	fprintf(fichier, "QUETES_END\n");
	fclose(fichier);
	printf("Partie sauvegardee dans 'partie_oceandepth.save'!\n");
}

// Je charge toute la partie
int charger_partie_complete(CarteOceanique *carte, Plongeur *joueur, CreatureList *creatures, SystemeQuetes *quetes, Inventaire *inv) {
	// J'ouvre le fichier de sauvegarde en lecture
	FILE *fichier = fopen("partie_oceandepth.save", "r");
	if (!fichier) {
		printf("Aucune sauvegarde trouvee!\n");
		return -1;
	}
	// Je lis la premiere ligne pour verifier le format
	char ligne[256];
	if (!fgets(ligne, sizeof(ligne), fichier) ||
		(strncmp(ligne, "OCEANDEPTH_SAVE_V2", 18) != 0 && strncmp(ligne, "OCEANDEPTH_SAVE_V1", 18) != 0)) {
		printf("Format de sauvegarde incompatible!\n");
		fclose(fichier);
		return -1;
	}
	// Je charge le joueur
	if (charger_joueur(fichier, joueur) != 0) {
		printf("Erreur lors du chargement du joueur!\n");
		fclose(fichier);
		return -1;
	}
	// Je charge les creatures
	if (creatures && creatures_load(fichier, creatures) != 0) {
		printf("Erreur lors du chargement des creatures!\n");
		fclose(fichier);
		return -1;
	}
	// Je charge la carte
	if (charger_carte(fichier, carte) != 0) {
		printf("Erreur lors du chargement de la carte!\n");
		fclose(fichier);
		return -1;
	}
	// Je verifie si c'est la version 2 de la sauvegarde
	if (strncmp(ligne, "OCEANDEPTH_SAVE_V2", 18) == 0) {
		// Je cherche le debut de l'inventaire
		while (fgets(ligne, sizeof(ligne), fichier)) {
			if (strncmp(ligne, "INVENTAIRE_START", 16) == 0) break;
		}
		// Je lis l'inventaire
		if (fscanf(fichier, "%d %d %d", &inv->harpon_equipe, &inv->combi_equipee, &inv->perles) == 3) {
			for (int i = 0; i < MAX_OBJETS; i++) {
				Objet *obj = &inv->objets[i];
				fscanf(fichier, "%d %s %d %d %d %d %d %d %d %d %d %d",
					   &obj->est_vide, obj->nom, (int*)&obj->type, &obj->quantite,
					   &obj->restaure_pv, &obj->restaure_oxygene, &obj->reduit_fatigue,
					   &obj->attaque_min, &obj->attaque_max, &obj->defense, &obj->cout_oxygene, &obj->id);
			}
		}
		// Je cherche le debut des quetes
		while (fgets(ligne, sizeof(ligne), fichier)) {
			if (strncmp(ligne, "QUETES_START", 12) == 0) break;
		}
		// Je lis les quetes
		if (fscanf(fichier, "%d %d", &quetes->quete_active, &quetes->nb_quetes_terminees) == 2) {
			for (int i = 0; i < NB_QUETES; i++) {
				Quete *q = &quetes->quetes[i];
				fscanf(fichier, "%d %d %d %d %d %d",
					   (int*)&q->type, (int*)&q->statut, &q->recompense_xp, &q->recompense_or,
					   &q->objectif_actuel, &q->objectif_requis);
			}
			initialiser_quetes(quetes);
		}
	} else {
	// Si c'est une ancienne sauvegarde, j'initialise l'inventaire et les quetes
	initialiser_inventaire(inv);
	initialiser_quetes(quetes);
	}
	// Je ferme le fichier et j'affiche un message de succes
	fclose(fichier);
	printf("Partie chargee avec succes!\n");
	// Je repare les zones debloquees
	for (int y = 0; y < TAILLE_CARTE; y++) {
		for (int x = 0; x < TAILLE_CARTE; x++) {
			if (carte->grille[y][x].exploree) {
				debloquer_zones_adjacentes(carte, x, y);
			}
		}
	}
	printf("Reparation des zones debloquees terminee.\n");
	return 0;
}
//
// Created by maxim on 07/10/2025.
//