#!/bin/bash

SRC_DIR="."
OUT_FILE="main.cpp"

#> ${OUT_FILE} # Create output file

#printf '#include "main.hpp"\n\n' >> ${OUT_FILE}

#for filename in `find ${SRC_DIR}/* -name '*.cpp'`
#do
#    filename=${filename#${SRC_DIR}/}
#    printf '#include "%s"\n' ${filename} >> ${OUT_FILE}
#done

echo "Done."

`g++ -w -o a.out main.cpp interface/LovyanGFX/lgfx/Fonts/efont/*.c interface/LovyanGFX/lgfx/Fonts/IPA/*.c interface/LovyanGFX/lgfx/utility/*.c interface/LovyanGFX/lgfx/v1/*.cpp interface/LovyanGFX/lgfx/v1/misc/*.cpp interface/LovyanGFX/lgfx/v1/panel/Panel_Device.cpp interface/LovyanGFX/lgfx/v1/platforms/sdl/*.cpp  -lm -lSDL2  -lSDL_image -Iinterface/LovyanGFX -Iinterface/LovyanGFX/lgfx/v1 -Iinterface/LovyanGFX/lgfx/Fonts/efont -Linterface/LovyanGFX/lgfx -DLGFX_SDL`