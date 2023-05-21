#!/bin/bash
set -eo pipefail

# if $lib_project.a doesn't exist or rebuild_* task was executed, builds static lib
function handle_static_dependency {
    local lib_project="$1"
    local lib_build_task="$2"
    local lib_build_rezults="$3"
    if [ ! -f "$OBJDIR/libs/$lib_project.a" ] || [ -f .rebuild_$lib_project.tmp ]; then
        [[ -z "$lib_build_task" ]] && error "lib_build_task is empty" 
        myprint "${BLUE}making $lib_project task <$lib_build_task>"

        cd $lib_project
        if ! make "$lib_build_task"; then
            exit 1
        fi
        cd ..

        cp $lib_build_rezults $OBJDIR/libs/
        myprint "${GREEN}copied ${CYAN}$lib_project.a"
        rm -f .rebuild_$lib_project.tmp
    fi
}

handle_static_dependency kerep "$DEPS_BUILD_TASK" kerep/bin/kerep.a
handle_static_dependency utf8proc libutf8proc.a utf8proc/libutf8proc.a

source tasks/build_view.sh
