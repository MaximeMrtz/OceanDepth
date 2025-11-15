


// Inclusions standards
#include <stdio.h>
#include <string.h>

// Inclusions du projet
#include "carte.h"

// Fonction pour sauvegarder la carte dans un fichier texte
int sauvegarder_carte(FILE *out, CarteOceanique *carte) {
	if (!out || !carte) return -1;
	fprintf(out, "DECOUVERTES:%d\n", carte->zones_decouvertes);
	for (int y = 0; y < TAILLE_CARTE; y++) {
		for (int x = 0; x < TAILLE_CARTE; x++) {
			Zone *zone = &carte->grille[y][x];
			fprintf(out, "ZONE:%d:%d:%d:%d:%d:%d:%d:%s\n", x, y, zone->type, zone->profondeur, zone->exploree, zone->debroquee, zone->nb_creatures, zone->nom);
		}
	}
	return 0;
}

// Fonction pour charger la carte depuis un fichier texte
int charger_carte(FILE *in, CarteOceanique *carte) {
	if (!in || !carte) return -1;
	char ligne[256];
	if (fgets(ligne, sizeof(ligne), in)) {
		if (sscanf(ligne, "DECOUVERTES:%d", &carte->zones_decouvertes) != 1) {
			return -1;
		}
	}
	for (int i = 0; i < TAILLE_CARTE * TAILLE_CARTE; i++) {
		if (fgets(ligne, sizeof(ligne), in)) {
			int x, y, type, profondeur, exploree, debroquee, nb_creatures;
			char nom[50] = "";
			if (sscanf(ligne, "ZONE:%d:%d:%d:%d:%d:%d:%d:%49[^\n]", &x, &y, &type, &profondeur, &exploree, &debroquee, &nb_creatures, nom) >= 7) {
				Zone *zone = &carte->grille[y][x];
				zone->type = (TypeZone)type;
				zone->profondeur = profondeur;
				zone->exploree = exploree;
				zone->debroquee = debroquee;
				zone->nb_creatures = nb_creatures;
				if (strlen(nom) > 0) strncpy(zone->nom, nom, sizeof(zone->nom)-1);
			}
		}
	}
	return 0;
}

// Fichier principal pour la gestion de la carte et du jeu complet (carto, combat, créatures, etc)


#include <stdio.h>

