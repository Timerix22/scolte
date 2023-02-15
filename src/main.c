#include "../kerep/src/Filesystem/filesystem.h"
#include "tui.h"

Maybe tryReadFile(char* filePath){
    if(!file_exists(filePath))
        kprintf("file doesnt exist, creating new\n");
    try(file_open(filePath, FileOpenMode_ReadAppend), _m_file,;);
    File* file=_m_file.value.VoidPtr;
    char* fileContent=NULL;
    try(file_readAll(file, &fileContent), _m_bcount,;)
    try(file_close(file),_m_,;);
    return SUCCESS(UniHeapPtr(char,fileContent));
}

i32 main(const i32 argc, const char* const* argv){
    kt_beginInit();
    kt_initKerepTypes();
    kt_initScolteTypes();
    kt_endInit();

    kprintf("\e[37margs:");
    for(i32 i=0; i<argc; i++)
        kprintf(" %s", argv[i]);
    kprintf("\n");
    
    /*char* filePath= argc>1 ? argv[argc-1] : "new_file.txt";
    tryLast(tryReadFile(filePath), _m_text);
    char* text=(char*)_m_text.value.VoidPtr;
    fputs(text,stdout);*/

    Canvas* mainCanvas=Canvas_create();
    TextBlock* testTextBlock=TextBlock_create(string_fromCptr("some example text"));
    Canvas_addChild(mainCanvas, (UIElement*)testTextBlock);
    UIElement_draw(mainCanvas, ((Rect){.x=-1, .y=-1, .h=-1, .w=-1}));
    UIElement_destroy((UIElement*)mainCanvas);
    kt_free();
    return 0;
}
