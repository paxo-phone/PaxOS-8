#!/bin/bash

# Définir le nom du nouveau dossier
nouveau_dossier="Arduino"

# Liste des dossiers à ignorer
dossiers_a_ignorer=("Arduino" "src/lib/dirent-1.23.2" "src/lib/libcurl" "src/lib/SDL2-2.28.2")

# Créer le nouveau dossier s'il n'existe pas déjà
mkdir -p "$nouveau_dossier"

# Copier tous les fichiers du répertoire courant vers le nouveau dossier
cp -r * "$nouveau_dossier"

# Ignorer les fichiers et dossiers spécifiés
for dossier_a_ignorer in "${dossiers_a_ignorer[@]}"; do
    rm -rf "$nouveau_dossier/$dossier_a_ignorer"
done

mv Arduino/PaxOS-8.ino Arduino/Arduino.ino

echo "Copie terminée avec succès."