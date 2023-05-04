#!/usr/bin/bash
for tmpfile in $(ls -a | grep -e '\.rebuild.*\.tmp'); do
    try_delete_dir_or_file "$tmpfile"
done

for submodule in kerep utf8proc; do
    cd "$submodule"
    make clean
    cd ..
done
