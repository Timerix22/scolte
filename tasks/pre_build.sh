#!/bin/bash

if [ ! -f "kerep/bin/kerep.a" ]  || [ -f .rebuild_kerep ] 
then
    cd kerep
    if ! make "$KEREP_BUILD_TASK"; then
        exit 1
    fi
    cd ..
    rm -rf .rebuild_kerep
fi
cp kerep/bin/kerep.a obj/
printf "${GREEN}copied ${CYAN}kerep.a\n"
