#include "tui_internal.h"
#include "Dtsod/tui_dtsod_internal.h"

kt_define(DrawingArea, NULL, NULL);
kt_define(UIBorder, NULL, NULL);
kt_define(UITDescriptor, NULL, NULL);

Hashtable* _uit_hashtable=NULL;
u32 __kt_id_first=-1;
Autoarr(Pointer)* __uit_temp_arr=NULL;
UITDescriptor** __uit_descriptors=NULL;

/// call this between kt_beginInit() and kt_endInit()
void kt_beginInitTUI(){
    _uit_hashtable=Hashtable_create();
    __kt_id_first=ktid_last;
    __uit_temp_arr=Autoarr_create(Pointer, 32, 32);
}

/// call this between kt_beginInit() and kt_endInit()
void kt_endInitTUI(){
    __uit_descriptors=(UITDescriptor**)Autoarr_toArray(__uit_temp_arr);
    Autoarr_free(__uit_temp_arr, true);
}

void kt_freeTUI(){
    free(__uit_descriptors);
    Hashtable_free(_uit_hashtable);
}

void __uit_register(ktDescriptor* kt, UITDescriptor* uit){
    uit->type=kt;
    Hashtable_add(_uit_hashtable, cptr_copy(kt->name), UniStackPtr(UITDescriptor, uit));
}

UITDescriptor* UITDescriptor_getById(ktid id){
    if(id > ktid_last)
        throw(ERR_WRONGINDEX);
    if(__uit_descriptors==NULL)
        throw(ERR_NULLPTR);
    return __uit_descriptors[id - __kt_id_first];
}

///@return NULL if not found
UITDescriptor* UITDescriptor_getByName(char* name){
    Unitype val=Hashtable_get(_uit_hashtable, name);
    if(Unitype_isUniNull(val)){
        char* name_ptr=cptr_concat(name,"_Ptr");
        val=Hashtable_get(_uit_hashtable, name_ptr);
        free(name_ptr);
    }
    return val.VoidPtr;
}

void kt_initScolteTypes(){
    // term.h
    kt_register(TerminalSize);
    kt_register(TermCharInfo);
    // tui.h
    kt_register(UITDescriptor)
    kt_register(DrawingArea);
    kt_register(UIBorder);
    kt_register(FrameBuffer);
    kt_register(Renderer);
    kt_register(UIElement);
    uit_register(Grid);
    uit_register(TextBlock);
    // tui_dtsod.h
    kt_register(UIDtsodFileModel);
    kt_register(UIDtsodParser);
    kt_register(UIContext);
}

/// call this between kt_beginInitTUI() and kt_endInitTUI()
void Scolte_init(){
    kt_initScolteTypes();
    UI_enum_tables_init();
}

// call this before kt_free()
void Scolte_free(){
    UI_enum_tables_free();
    kt_freeTUI();
}