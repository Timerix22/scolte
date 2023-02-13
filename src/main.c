#include "../kerep/src/Filesystem/filesystem.h"

i32 main(const i32 argc, const char* const* argv){
    kprintf("\e[37margs:");
    for(i32 i=0; i<argc; i++)
        kprintf(" %s", argv[i]);
    kprintf("\n");
    if(argc==1)
        return 0;
    char* filePath=argv[argc-1];
    if(!file_exists(filePath))
        kprintf("file doesnt exist, creating new\n");
    tryLast(file_open(filePath, FileOpenMode_ReadWrite), _m_file);
    File* file=_m_file.value.VoidPtr;
    tryLast(file_close(file),_m);
    return 0;
}
