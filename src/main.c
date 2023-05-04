#include "../kerep/src/Filesystem/filesystem.h"
#include "TUI/tui.h"
#include <unistd.h>

Maybe tryReadFile(char* filePath){
    if(!file_exists(filePath))
        kprintf("file doesnt exist, creating new\n");
    try(file_open(filePath, FileOpenMode_ReadAppend), _m_file,;);
    FileHandle file=_m_file.value.VoidPtr;
    char* fileContent=NULL;
    try(file_readAll(file, &fileContent), _m_bcount,;)
    try(file_close(file),_m_,;);
    return SUCCESS(UniHeapPtr(char,fileContent));
}
#include <windows.h>
i32 main(const i32 argc, const char* const* argv){
#if _WIN32 || _WIN64
    if(!SetConsoleOutputCP(CP_UTF8)){
        kprintf("\e[93mcan't set console codepage to utf8");
    }
#endif
    if(setlocale(LC_ALL, "C.UTF8")==0){ // doesn't work on windows
        kprintf("\e[93msetlocale failed! (%i)\n", errno);
    }

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

    Renderer* renderer=Renderer_create();
    Canvas* mainCanvas=Canvas_create();
    TextBlock* testTextBlock=TextBlock_create(string_fromCptr("some example text"));
    Canvas_addChild(mainCanvas, (UIElement*)testTextBlock);
    tryLast(UIElement_draw(renderer, (UIElement*)mainCanvas, ((DrawingArea){.x=4, .y=4, .h=4, .w=4})),_);
    tryLast(Renderer_drawFrame(renderer),_2);

    UIElement_destroy((UIElement*)mainCanvas);
    Renderer_destroy(renderer);
    
    kt_free();
    return 0;
}
