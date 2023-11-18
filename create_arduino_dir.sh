#!/bin/bash

# Définit le nom du nouveau dossier
newDir="Arduino"

# Liste des dossiers à ignorer
ignoredDirs=("Arduino" "src/lib/dirent-1.23.2" "src/lib/libcurl" "src/lib/SDL2-2.28.2")

# Créé le nouveau dossier cible s'il n'existe pas déjà
mkdir -p "$newDir"

# Copie tous les fichiers du répertoire courant vers le dossier cible
cp -r * "$newDir"

# Supprime les fichiers et dossiers ignorés
for ignoredDir in "${ignoredDirs[@]}"; do
    rm -rf "$newDir/$ignoredDir"
done

# Renomme PaxOS-8.ino en Arduino.ino dans le dossier cible
mv Arduino/PaxOS-8.ino Arduino/Arduino.ino

echo "Copie terminée avec succès."