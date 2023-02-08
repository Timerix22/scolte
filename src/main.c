#include "../kerep/src/Filesystem/filesystem.h"

int main(const int argc, const char* const* argv){
    kprintf("\e[37margs:");
    for(int i=0; i<argc; i++)
        kprintf(" %s", argv[i]);
    kprintf("\n");
    
    return 0;
}
