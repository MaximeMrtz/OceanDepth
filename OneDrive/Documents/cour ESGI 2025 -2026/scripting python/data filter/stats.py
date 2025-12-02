def afficher_stats(data):
    compute_stats(data)

def compute_stats(data):
    print("\n=== Statistiques ===")
    if not data:
        print("Aucune donnée")
        return

    keys = data[0].keys()
    for key in keys:
        values = [item[key] for item in data]
        if all(isinstance(v, (int, float)) for v in values):
            print(f"{key}: min={min(values)}, max={max(values)}, moyenne={sum(values)/len(values):.2f}")
        elif all(isinstance(v, bool) for v in values):
            true_count = sum(values)
            print(f"{key}: %True={(true_count/len(values))*100:.2f}%, %False={100-(true_count/len(values))*100:.2f}%")
        elif all(isinstance(v, list) for v in values):
            sizes = [len(v) for v in values]
            print(f"{key}: taille min={min(sizes)}, max={max(sizes)}, moyenne={sum(sizes)/len(sizes):.2f}")
        else:
            print(f"{key}: type non numérique, pas de stats")
