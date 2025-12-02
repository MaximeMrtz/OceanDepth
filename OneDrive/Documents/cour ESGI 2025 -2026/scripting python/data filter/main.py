

# Importation des modules nécessaires

import loader
import stats
import filter
import sort
import os

DATA = []

if __name__ == "__main__":
    print("\n=== MENU PRINCIPAL ===")
    print("1. Charger un fichier (CSV/JSON/XML/YAML)")
    print("2. Sauvegarder les données (CSV/JSON/XML/YAML)")
    print("3. Afficher les statistiques")
    print("4. Filtrer les données")
    print("5. Trier les données")
    print("6. Afficher les données")
    print("7. Quitter")

def charger_fichier():
    global DATA
    print("\nFormat à charger : 1=CSV, 2=JSON, 3=XML, 4=YAML")
    fmt = input("Choix : ")
    filename = input("Nom du fichier à charger : ")
    if not os.path.exists(filename):
        print("Fichier introuvable.")
        return
    if fmt == "1":
        DATA = loader.load_csv(filename)
    elif fmt == "2":
        DATA = loader.load_json(filename)
    elif fmt == "3":
        DATA = loader.load_xml(filename)
    elif fmt == "4":
        DATA = loader.load_yaml(filename)
    else:
        print("Format non supporté.")
        return
    print(f"{len(DATA)} éléments chargés.")

def sauvegarder_fichier():
    print("\nFormat à sauvegarder : 1=CSV, 2=JSON, 3=XML, 4=YAML")
    fmt = input("Choix : ")
    filename = input("Nom du fichier à sauvegarder : ")
    if fmt == "1":
        # CSV : on sauvegarde seulement les champs plats
        loader.save_json(DATA, filename + ".tmp.json")
        print("Astuce : pour CSV, convertissez d'abord en JSON plat si besoin.")
    elif fmt == "2":
        loader.save_json(DATA, filename)
    elif fmt == "3":
        loader.save_xml(DATA, filename)
    elif fmt == "4":
        loader.save_yaml(DATA, filename)
    else:
        print("Format non supporté.")
        return
    print(f"Données sauvegardées dans {filename}")

def afficher_stats():
    if not DATA:
        print("Aucune donnée chargée.")
        return
    stats.afficher_stats(DATA)

def filtrer_donnees():
    if not DATA:
        print("Aucune donnée chargée.")
        return
    print("\nFiltrage :")
    print("1. Comparaison simple (>=)")
    print("2. Contient (chaîne)")
    print("3. Commence par (chaîne)")
    print("4. Finit par (chaîne)")
    print("5. Taille minimale (liste)")
    print("6. Moyenne minimale (liste)")
    sub_choice = input("Choix : ")
    key = input("Champ à filtrer : ")
    value = input("Valeur : ")
    if sub_choice == "1":
        result = filter.filter_data(DATA, key, value)
    elif sub_choice == "2":
        result = filter.filter_contains(DATA, key, value)
    elif sub_choice == "3":
        result = filter.filter_startswith(DATA, key, value)
    elif sub_choice == "4":
        result = filter.filter_endswith(DATA, key, value)
    elif sub_choice == "5":
        result = filter.filter_list_size(DATA, key, int(value))
    elif sub_choice == "6":
        result = filter.filter_list_avg(DATA, key, float(value))
    else:
        print("Filtre non supporté.")
        return
    print(f"Résultat filtré ({len(result)} éléments) :")
    for item in result:
        print(item)

def trier_donnees():
    if not DATA:
        print("Aucune donnée chargée.")
        return
    print("\nTri :")
    print("1. Par champ simple")
    print("2. Multi-critères (champ1 puis champ2)")
    print("3. Par valeur globale (ex: price*quantity)")
    choice = input("Choix : ")
    if choice == "1":
        key = input("Champ pour tri : ")
        result = sort.sort_data(DATA, key)
    elif choice == "2":
        key1 = input("Champ principal : ")
        key2 = input("Champ secondaire : ")
        result = sort.sort_multi(DATA, key1, key2)
    elif choice == "3":
        result = sort.sort_by_global_value(DATA)
    else:
        print("Tri non supporté.")
        return
    print("Données triées :")
    for item in result:
        print(item)

def afficher_donnees():
    if not DATA:
        print("Aucune donnée chargée.")
        return
    for item in DATA:
        print(item)

def main():
    while True:
        afficher_menu()
        choix = input("Votre choix : ")
        if choix == "1":
            charger_fichier()
        elif choix == "2":
            sauvegarder_fichier()
        elif choix == "3":
            afficher_stats()
        elif choix == "4":
            filtrer_donnees()
        elif choix == "5":
            trier_donnees()
        elif choix == "6":
            afficher_donnees()
        elif choix == "7":
            print("Au revoir !")
            break
        else:
            print("Choix invalide.")


def afficher_menu():
    print("\n=== MENU PRINCIPAL ===")
    print("1. Charger un fichier (CSV/JSON/XML/YAML)")
    print("2. Sauvegarder les données (CSV/JSON/XML/YAML)")
    print("3. Afficher les statistiques")
    print("4. Filtrer les données")
    print("5. Trier les données")
    print("6. Afficher les données")
    print("7. Quitter")

if __name__ == "__main__":
    main()
