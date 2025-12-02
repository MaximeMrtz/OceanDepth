# Ajout des filtres avancés si absents

def filter_contains(data, key, substring):
    return [item for item in data if key in item and isinstance(item[key], str) and substring in item[key]]

def filter_startswith(data, key, prefix):
    return [item for item in data if key in item and isinstance(item[key], str) and item[key].startswith(prefix)]

def filter_endswith(data, key, suffix):
    return [item for item in data if key in item and isinstance(item[key], str) and item[key].endswith(suffix)]

def filter_list_size(data, key, min_size):
    return [item for item in data if key in item and isinstance(item[key], list) and len(item[key]) >= min_size]

def filter_list_avg(data, key, min_avg):
    result = []
    for item in data:
        if key in item and isinstance(item[key], list) and all(isinstance(x, (int, float)) for x in item[key]):
            avg = sum(item[key]) / len(item[key])
            if avg >= min_avg:
                result.append(item)
    return result

def filter_data(data, key, value):
    """
    Filtre les données selon une comparaison simple (>=) sur le champ donné.
    """
    result = []
    for item in data:
        if key in item:
            item_value = item[key]
            if isinstance(item_value, str):
                if item_value >= value:
                    result.append(item)
            elif isinstance(item_value, list):
                if len(item_value) >= int(value):
                    result.append(item)
    return result

def filter_contains(data, key, substring):
    """
    Filtre les données dont le champ contient la sous-chaîne donnée.
    """
    return [item for item in data if key in item and isinstance(item[key], str) and substring in item[key]]

def filter_startswith(data, key, prefix):
    """
    Filtre les données dont le champ commence par le préfixe donné.
    """
    return [item for item in data if key in item and isinstance(item[key], str) and item[key].startswith(prefix)]

def filter_endswith(data, key, suffix):
    """
    Filtre les données dont le champ finit par le suffixe donné.
    """
    return [item for item in data if key in item and isinstance(item[key], str) and item[key].endswith(suffix)]

def filter_list_size(data, key, min_size):
    """
    Filtre les données dont la liste associée au champ a une taille minimale.
    """
    return [item for item in data if key in item and isinstance(item[key], list) and len(item[key]) >= min_size]

def filter_list_avg(data, key, min_avg):
    """
    Filtre les données dont la moyenne des éléments de la liste associée au champ est supérieure ou égale à min_avg.
    """
    result = []
    for item in data:
        if key in item and isinstance(item[key], list) and all(isinstance(x, (int, float)) for x in item[key]):
            avg = sum(item[key]) / len(item[key])
            if avg >= min_avg:
                result.append(item)
    return result
# Ajout des filtres avancés si absents
def filter_contains(data, key, substring):
    return [item for item in data if key in item and isinstance(item[key], str) and substring in item[key]]

def filter_startswith(data, key, prefix):
    return [item for item in data if key in item and isinstance(item[key], str) and item[key].startswith(prefix)]

def filter_endswith(data, key, suffix):
    return [item for item in data if key in item and isinstance(item[key], str) and item[key].endswith(suffix)]

def filter_list_size(data, key, min_size):
    return [item for item in data if key in item and isinstance(item[key], list) and len(item[key]) >= min_size]

def filter_list_avg(data, key, min_avg):
    result = []
    for item in data:
        if key in item and isinstance(item[key], list) and all(isinstance(x, (int, float)) for x in item[key]):
            avg = sum(item[key]) / len(item[key])
            if avg >= min_avg:
                result.append(item)
    return result


def filter_data(data, key, value):
    """
    Filtre les données selon une comparaison simple (>=) sur le champ donné.
    """
    result = []
    for item in data:
        if key in item:
            item_value = item[key]
            if isinstance(item_value, str):
                if item_value >= value:
                    result.append(item)
            elif isinstance(item_value, (int, float)):
                try:
                    if item_value >= float(value):
                        result.append(item)
                except ValueError:
                    pass
            elif isinstance(item_value, list):
                if len(item_value) >= int(value):
                    result.append(item)
    return result


def filter_contains(data, key, substring):
    """
    Filtre les données dont le champ contient la sous-chaîne donnée.
    """
    return [item for item in data if key in item and isinstance(item[key], str) and substring in item[key]]

def filter_startswith(data, key, prefix):
    """
    Filtre les données dont le champ commence par le préfixe donné.
    """
    return [item for item in data if key in item and isinstance(item[key], str) and item[key].startswith(prefix)]

def filter_endswith(data, key, suffix):
    """
    Filtre les données dont le champ finit par le suffixe donné.
    """
    return [item for item in data if key in item and isinstance(item[key], str) and item[key].endswith(suffix)]

def filter_list_size(data, key, min_size):
    """
    Filtre les données dont la liste associée au champ a une taille minimale.
    """
    return [item for item in data if key in item and isinstance(item[key], list) and len(item[key]) >= min_size]

def filter_list_avg(data, key, min_avg):
    """
    Filtre les données dont la moyenne des éléments de la liste associée au champ est supérieure ou égale à min_avg.
    """
    result = []
    for item in data:
        if key in item and isinstance(item[key], list) and all(isinstance(x, (int, float)) for x in item[key]):
            avg = sum(item[key]) / len(item[key])
            if avg >= min_avg:
                result.append(item)
    return result
