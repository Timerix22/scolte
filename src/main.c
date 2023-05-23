#include "TUI/tui.h"
#include "TUI/Dtsod/tui_dtsod.h"
#include "../kerep/src/Filesystem/filesystem.h"
#include <unistd.h>
#include <errno.h>
#if _WIN32 || _WIN64
#include <windows.h>
#endif

#define EMBEDDED_RESOURCE_POSTFIX view 
#include "generated/view.h"

void on_exit(){
    Scolte_free();
    kt_free();
    term_resetColors();
    term_cursorHide(false);
}

// Maybe tryReadFile(char* filePath){
//     if(!file_exists(filePath))
//         kprintf("file doesnt exist, creating new\n");
//     try(file_open(filePath, FileOpenMode_ReadAppend), _m_file,;);
//     FileHandle file=_m_file.value.VoidPtr;
//     char* fileContent=NULL;
//     try(file_readAll(file, &fileContent), _m_bcount,;)
//     try(file_close(file),_m_,;);
//     return SUCCESS(UniHeapPtr(char,fileContent));
// }

UI_THROWING_FUNC_DECL(test_example_ui_build());
UI_Maybe test_example_ui_build() {

    // ui from dtsod build example
    UIDtsodParser* p=UIDtsodParser_create();
    for(unsigned int i=0; i<EmbeddedResourceFile_table_view_count; i++){
        EmbeddedResourceFile rs=EmbeddedResourceFile_table_view[i];
        UI_try(UIDtsodParser_parseText(p, rs.path, rs.data),
            _91624, UIDtsodParser_destroy(p));
    }
    UI_try(UIDtsodParser_constructUIContext(p), _m_ui_context, UIDtsodParser_destroy(p));
    UIContext* ui_context=_m_ui_context.value.VoidPtr;
    UIDtsodParser_destroy(p);
    UIContext_get(ui_context, example, textblock, TextBlock, UIContext_destroy(ui_context));
    example_textblock->text=string_fromCptr("text replaced");
    UIContext_destroy(ui_context);
    return MaybeNull;
}

i32 main(const i32 argc, const char* const* argv){
#if _WIN32 || _WIN64
    if(!SetConsoleOutputCP(CP_UTF8)){
        kprintf("\e[93mcan't set console codepage to utf8");
    }
#endif
    if(setlocale(LC_ALL, "C.UTF8")==0){ // doesn't work on windows
        kprintf("\e[93msetlocale failed! (%i)\n", errno);
    }

    term_cursorHide(true);
    term_clear();

    // kerep type system
    kt_beginInit();
    kt_initKerepTypes();
    kt_beginInitTUI();
    Scolte_init();
    kt_endInitTUI();
    kt_endInit();

    // print command line arguments
    kprintf("\e[37margs:");
    for(i32 i=0; i<argc; i++)
        kprintf(" %s", argv[i]);
    kprintf("\n");
    
    // create file
    /* char* filePath= argc>1 ? argv[argc-1] : "new_file.txt";
    tryLast(tryReadFile(filePath), _m_text, on_exit());
    char* text=(char*)_m_text.value.VoidPtr;
    fputs(text,stdout); */

    // render ui
    // Renderer* renderer=Renderer_create();
    // TextBlock* testTextBlock=TextBlock_create("TextBlock1", string_fromCptr("some example text"));
    // Autoarr(UIElement_Ptr)* grid_content=Autoarr_create(UIElement_Ptr, 1, 64);
    // Autoarr_add(grid_content, (UIElement_Ptr)testTextBlock);
    // Grid* mainGrid=Grid_create("MainGrid", 1,1, Autoarr_toArray(grid_content));
    // Autoarr_free(grid_content, true);
    // tryLast(UIElement_draw(renderer, (UIElement_Ptr)mainGrid, 
    //        ((DrawingArea){.x=10, .y=4, .h=7, .w=24})),
    //    _, on_exit());
    // tryLast(Renderer_drawFrame(renderer),_2);
    // // free ui memory
    // UIElement_destroy((UIElement_Ptr)mainGrid);
    // Renderer_destroy(renderer);
    
    // TODO signal( SIGWINCH, redraw );

    tryLast(test_example_ui_build(), _6981751, on_exit());
    
    on_exit();
    return 0;
}
