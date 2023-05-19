#include "TUI/tui.h"
#include "../kerep/src/Filesystem/filesystem.h"
#include <unistd.h>
#include <errno.h>
#if _WIN32 || _WIN64
#include <windows.h>
#endif

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

i32 main(const i32 argc, const char* const* argv){
#if _WIN32 || _WIN64
    if(!SetConsoleOutputCP(CP_UTF8)){
        kprintf("\e[91mcan't set console codepage to utf8");
        return 1;
    }
#endif
    if(setlocale(LC_ALL, "C.UTF8")==0){ // doesn't work on windows
        kprintf("\e[91msetlocale failed! (%i)\n", errno);
        return 1;
    }

    // term_cursorHide(true);
    // term_clear();

    // kerep type system
    kt_beginInit();
    kt_initKerepTypes();
    kt_initScolteTypes();
    kt_endInit();

    // print command line arguments
    kprintf("\e[37margs:");
    for(i32 i=0; i<argc; i++)
        kprintf(" %s", argv[i]);
    kprintf("\n");
    
    // create file
    /* char* filePath= argc>1 ? argv[argc-1] : "new_file.txt";
    tryLast(tryReadFile(filePath), _m_text);
    char* text=(char*)_m_text.value.VoidPtr;
    fputs(text,stdout); */

    // render ui
    Renderer* renderer=Renderer_create();
    TextBlock* testTextBlock=TextBlock_create(string_fromCptr("some example text"));
    Autoarr(UIElement_Ptr)* grid_content=Autoarr_create(UIElement_Ptr, 1, 64);
    Autoarr_add(grid_content, (UIElement_Ptr)testTextBlock);
    Grid* mainGrid=Grid_create(1,1, Autoarr_toArray(grid_content));
    Autoarr_free(grid_content, true);
    tryLast(UIElement_draw(renderer, (UIElement_Ptr)mainGrid, ((DrawingArea){.x=10, .y=4, .h=7, .w=24})),_);
    tryLast(Renderer_drawFrame(renderer),_2);
    // free ui memory
    UIElement_destroy((UIElement_Ptr)mainGrid);
    Renderer_destroy(renderer);
    
    // TODO signal( SIGWINCH, redraw );

    // exit
    kt_free();
    term_resetColors();
    term_cursorHide(false);
    return 0;
}
