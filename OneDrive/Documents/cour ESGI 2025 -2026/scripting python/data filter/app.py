# Route pour upload XML
@app.route('/upload_xml', methods=['POST'])
def upload_xml():
    global DATA
    file = request.files['file']
    DATA = loader.load_xml(file)
    return jsonify({"message": "Données XML chargées", "count": len(DATA)})

# Route pour upload YAML
@app.route('/upload_yaml', methods=['POST'])
def upload_yaml():
    global DATA
    file = request.files['file']
    DATA = loader.load_yaml(file)
    return jsonify({"message": "Données YAML chargées", "count": len(DATA)})

# Route pour sauvegarde XML
@app.route('/save_xml', methods=['POST'])
def save_xml():
    filename = 'data.xml'
    loader.save_xml(DATA, filename)
    return jsonify({"message": f"Données sauvegardées dans {filename}"})

# Route pour sauvegarde YAML
@app.route('/save_yaml', methods=['POST'])
def save_yaml():
    filename = 'data.yaml'
    loader.save_yaml(DATA, filename)
    return jsonify({"message": f"Données sauvegardées dans {filename}"})

from flask import Flask, request, jsonify
import loader, stats, filter, sort
import os


app = Flask(__name__)
DATA = []
FILTER_HISTORY = []
REDO_STACK = []

@app.route('/')
def home():
    return "Bienvenue sur Data Filter API"

@app.route('/upload', methods=['POST'])
def upload_file():
    global DATA
    file = request.files['file']
    if file.filename.endswith('.csv'):
        DATA = loader.load_csv(file)
    elif file.filename.endswith('.json'):
        DATA = loader.load_json(file)
    else:
        return jsonify({"error": "Format non supporté"}), 400
    return jsonify({"message": "Données chargées", "count": len(DATA)})

@app.route('/stats', methods=['GET'])
def get_stats():
    if not DATA:
        return jsonify({"error": "Aucune donnée"}), 400
    # On suppose que stats.compute_stats_dict existe
    return jsonify(stats.compute_stats_dict(DATA))

@app.route('/filter', methods=['GET'])

def filter_data_route():
    global FILTER_HISTORY, REDO_STACK
    mode = request.args.get('mode', 'simple')
    key = request.args.get('key')
    value = request.args.get('value')

    if not key or not value:
        return jsonify({"error": "Paramètres 'key' et 'value' requis"}), 400

    if mode == 'simple':
        filtered = filter.filter_data(DATA, key, value)
    elif mode == 'contains':
        filtered = filter.filter_contains(DATA, key, value)
    elif mode == 'startswith':
        filtered = filter.filter_startswith(DATA, key, value)
    elif mode == 'endswith':
        filtered = filter.filter_endswith(DATA, key, value)
    elif mode == 'list_size':
        try:
            filtered = filter.filter_list_size(DATA, key, int(value))
        except ValueError:
            return jsonify({"error": "Valeur doit être un entier"}), 400
    elif mode == 'list_avg':
        try:
            filtered = filter.filter_list_avg(DATA, key, float(value))
        except ValueError:
            return jsonify({"error": "Valeur doit être un nombre"}), 400
    else:
        return jsonify({"error": f"Mode '{mode}' non supporté"}), 400

    FILTER_HISTORY.append(filtered)
    REDO_STACK.clear()
    return jsonify(filtered)

# Route pour undo
@app.route('/filter/undo', methods=['POST'])
def undo_filter():
    global FILTER_HISTORY, REDO_STACK
    if len(FILTER_HISTORY) > 1:
        last = FILTER_HISTORY.pop()
        REDO_STACK.append(last)
        return jsonify(FILTER_HISTORY[-1])
    elif FILTER_HISTORY:
        return jsonify(FILTER_HISTORY[0])
    else:
        return jsonify(DATA)

# Route pour redo
@app.route('/filter/redo', methods=['POST'])
def redo_filter():
    global FILTER_HISTORY, REDO_STACK
    if REDO_STACK:
        redo = REDO_STACK.pop()
        FILTER_HISTORY.append(redo)
        return jsonify(redo)
    elif FILTER_HISTORY:
        return jsonify(FILTER_HISTORY[-1])
    else:
        return jsonify(DATA)


@app.route('/sort', methods=['GET'])
def sort_data():
    mode = request.args.get('mode', 'simple')
    key = request.args.get('key')
    secondary = request.args.get('secondary')

    if not key:
        return jsonify({"error": "Paramètre 'key' requis"}), 400

    if mode == 'simple':
        sorted_data = sort.sort_data(DATA, key)
    elif mode == 'multi' and secondary:
        sorted_data = sort.sort_multi(DATA, key, secondary)
    elif mode == 'value_global':
        # Exemple pour items : price * quantity
        sorted_data = sort.sort_by_global_value(DATA)
    else:
        return jsonify({"error": "Mode non supporté ou paramètres manquants"}), 400

    return jsonify(sorted_data)

@app.route('/save', methods=['POST'])
def save_json():
    filename = request.form.get('filename', 'data.json')
    loader.save_json(DATA, filename)
    return jsonify({"message": f"Données sauvegardées dans {filename}"})

if __name__ == '__main__':
    app.run(debug=True)
