#!/bin/bash

SRC_DIR="."
OUT_FILE="main.cpp"

> ${OUT_FILE} # Create output file

printf '#include "main.hpp"\n\n' >> ${OUT_FILE}

for filename in `find ${SRC_DIR}/* -name '*.cpp'`
do
    filename=${filename#${SRC_DIR}/}
    printf '#include "%s"\n' ${filename} >> ${OUT_FILE}
done

echo "Done."
