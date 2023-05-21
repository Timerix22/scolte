#!/bin/bash
set -eo pipefail

mkdir -p "obj"
clean_dir "obj/objects"
mkdir -p "src"
mkdir -p "src/generated"

cd resource_embedder
./build.sh
cd ..

embedder_args="-o src/generated/view.h"
for guifile in $(find "view" -name '*.tui.dtsod'); do
    embedder_args="$embedder_args -i $guifile"
done
resource_embedder/resource_embedder $embedder_args
