#!/bin/bash

# Chemin vers le répertoire du projet
PROJET_DIR="."

# Nom du fichier principal
MAIN_FILE="main.cpp"

# Écraser le contenu de main.cpp
echo -n > "$PROJET_DIR/$MAIN_FILE"

# Fonction récursive pour inclure les fichiers "*.cpp"
function include_files_recursively {
  for file in "$1"/*; do
    if [[ "$file" == *"/interface/LovyanGFX"* ]]; then
      # Ignorer le dossier "interface/LovyanGFX"
      continue
    fi

    if [[ "$file" == *"/_"* ]]; then
      # Ignorer les fichiers et dossiers commençant par "_"
      continue
    fi

    if [[ "$file" == "./main.cpp" ]]; then
      continue
    fi

    if [[ -d "$file" ]]; then
      # Si c'est un dossier, appel récursif
      include_files_recursively "$file"
    elif [[ "$file" == *.cpp ]]; then
      # Si c'est un fichier "*.cpp", l'inclure dans main.cpp
      echo "#include \"$file\"" >> "$PROJET_DIR/$MAIN_FILE"
    fi
  done
}

# Inclure "main.hpp" au début de main.cpp
echo "#include \"main.hpp\"" >> "$PROJET_DIR/$MAIN_FILE"

# Appeler la fonction récursive pour inclure les fichiers "*.cpp"
include_files_recursively "$PROJET_DIR"

# Ignorer le script lui-même s'il est dans le projet
if [[ -f "$PROJET_DIR/$(basename $0)" ]]; then
  sed -i "/$(basename $0)/d" "$PROJET_DIR/$MAIN_FILE"
fi


`g++ -w -o a.out main.cpp interface/LovyanGFX/lgfx/Fonts/efont/*.c interface/LovyanGFX/lgfx/Fonts/IPA/*.c interface/LovyanGFX/lgfx/utility/*.c interface/LovyanGFX/lgfx/v1/*.cpp interface/LovyanGFX/lgfx/v1/misc/*.cpp interface/LovyanGFX/lgfx/v1/panel/Panel_Device.cpp interface/LovyanGFX/lgfx/v1/platforms/sdl/*.cpp  -lm -lSDL2  -lSDL_image -Iinterface/LovyanGFX -Iinterface/LovyanGFX/lgfx/v1 -Iinterface/LovyanGFX/lgfx/Fonts/efont -Linterface/LovyanGFX/lgfx -DLGFX_SDL`
echo "Done."