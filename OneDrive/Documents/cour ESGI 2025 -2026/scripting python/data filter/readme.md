
 Fonctionnalités réalisées
->Chargement et sauvegarde de fichiers (CSV, JSON, XML, YAML)
->Affichage des statistiques (min, max, moyenne, %True/False, taille des listes)
->Filtrage avancé : comparaison, contient, commence/finit par, taille et moyenne de liste
->Tri simple, multi-critères, ou par valeur globale
->Menu interactif en CLI

Exemple de test
Lancer le programme :
	```
	python main.py
	```

Voici un scénario de test complet à saisir dans le menu pour valider toutes les fonctionnalités avec data.json :
1.
 Charger les données
Votre choix : 1
Format à charger : 2 (JSON)
Nom du fichier à charger : data.json
2  Afficher les données
Votre choix : 6
→ Vous verrez les 3 personnes du fichier.
3. Afficher les statistiques
Votre choix : 3
→ Statistiques sur les âges, apprentis, tailles de listes de notes, etc.
4. Filtrer les données (exemples)
Votre choix : 4
Filtrage :
1 (Comparaison simple)
Champ à filtrer : age
Valeur : 21
→ Affiche Alice et Bob (âge ≥ 21)
2 (Contient)
Champ à filtrer : firstname
Valeur : a
→ Affiche Alice et Charlie (le prénom contient « a »)
5 (Taille minimale liste)
Champ à filtrer : grades
Valeur : 3
→ Affiche Alice et Charlie (au moins 3 notes)
5. Trier les données
Votre choix : 5
Tri :
1 (Par champ simple)
Champ pour tri : age
→ Affiche les personnes triées par âge croissant
6. Sauvegarder les données (optionnel)
Votre choix : 2
Format à sauvegarder : 2 (JSON)
Nom du fichier à sauvegarder : result.json

4. Filtrer, trier, afficher, sauvegarder… tout se fait via le menu.


