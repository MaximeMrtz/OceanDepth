
def sort_data(data, key):
    return sorted(data, key=lambda x: x.get(key))

def sort_data(data, key):
    return sorted(data, key=lambda x: x.get(key))

def sort_multi(data, key1, key2):
    return sorted(data, key=lambda x: (x.get(key1), x.get(key2)))

def sort_by_global_value(data):
    # Suppose que chaque item a 'price' et 'quantity'
    return sorted(data, key=lambda x: x.get('price', 0) * x.get('quantity', 0))
