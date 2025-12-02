


import csv
import json
import xml.etree.ElementTree as ET
import yaml

def load_csv(filename):
    """Charge un CSV depuis un chemin de fichier."""
    with open(filename, newline='', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        return [row for row in reader]

def load_json(filename):
    """Charge un JSON depuis un chemin de fichier."""
    with open(filename, encoding='utf-8') as f:
        return json.load(f)

def save_json(data, filename):
    """Sauvegarde les données dans un fichier JSON."""
    with open(filename, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=4, ensure_ascii=False)

def load_xml(filename):
    """Charge un XML depuis un chemin de fichier."""
    tree = ET.parse(filename)
    root = tree.getroot()
    return [child.attrib for child in root]

def save_xml(data, filename):
    """Sauvegarde les données dans un fichier XML."""
    root = ET.Element('data')
    for item in data:
        elem = ET.SubElement(root, 'item', attrib={str(k): str(v) for k, v in item.items()})
    tree = ET.ElementTree(root)
    tree.write(filename, encoding='utf-8', xml_declaration=True)

def load_yaml(filename):
    """Charge un YAML depuis un chemin de fichier."""
    with open(filename, encoding='utf-8') as f:
        return yaml.safe_load(f)

def save_yaml(data, filename):
    """Sauvegarde les données dans un fichier YAML."""
    with open(filename, 'w', encoding='utf-8') as f:
        yaml.dump(data, f, allow_unicode=True)
