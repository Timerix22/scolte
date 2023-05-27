#include "../tui_internal.h"
#include "tui_dtsod_internal.h"
#include "../../../kerep/src/Filesystem/filesystem.h"

//////////////////////////////////////
//         UIDtsodFileModel         //
//////////////////////////////////////

void UIDtsodFileModel_freeMembers(void* _fm){
    UIDtsodFileModel* fm=_fm;
    DtsodV24_free(fm->dtsod);
    // free(fm->dtsod_text);
    // free(fm->file_name);
}

kt_define(UIDtsodFileModel, UIDtsodFileModel_freeMembers, NULL);
Autoarr_define(UIDtsodFileModel, false);

//////////////////////////////////////
//          UIDtsodParser           //
//////////////////////////////////////

void _UIDtsodParser_freeMembers(void* _p){
    UIDtsodParser* p=_p;
    Autoarr_free(p->file_models, true);
    if(!p->returned_context && p->context!=NULL){
        UIContext_destroy(p->context);
    }
}

void UIDtsodParser_destroy(UIDtsodParser* p){
    _UIDtsodParser_freeMembers(p);
    free(p);
}

kt_define(UIDtsodParser, _UIDtsodParser_freeMembers, NULL);

UIDtsodParser* UIDtsodParser_create(){
    UIDtsodParser* parser=malloc(sizeof(UIDtsodParser));
    parser->context=NULL;
    parser->returned_context=false;
    parser->file_models=Autoarr_create(UIDtsodFileModel, 64, 32);
    return parser;
}


///@return UI_Maybe<void>
UI_Maybe UIDtsodParser_parseFile(UIDtsodParser* parser, char* file_path){
    UI_try(file_open(file_path, FileOpenMode_Read), _m_file, ;);
    FileHandle file=_m_file.value.VoidPtr;
    char* file_content=NULL;
    UI_try(file_readAll(file, &file_content), _m_bcount, ;)
    UI_try(file_close(file), _m_1563, free(file_content));
    UI_try(UIDtsodParser_parseText(parser, file_path, file_content), _6561, free(file_content));
    return MaybeNull;
}

UI_Maybe UIDtsodParser_parseText(UIDtsodParser* parser, char* file_name_placeholder, char* text){
    UI_try(DtsodV24_deserialize(text), _m_dtsod, ;);
    UIDtsodFileModel fm={
        .file_name=file_name_placeholder,
        .dtsod_text=text,
        .dtsod=_m_dtsod.value.VoidPtr
    };
    Autoarr_add(parser->file_models, fm);
    return MaybeNull;
}

UI_Maybe UIElement_deserialize(Dtsod* dtsod){
    char* type_name;
    Dtsod_tryGet_cptr(dtsod, "type", type_name, true);
    UITDescriptor* type=UITDescriptor_getByName(type_name);
    if(type==NULL)
        UI_safethrow_msg(cptr_concat("invalid type '", type_name, "'"), ;);
    if(type->deserialize==NULL)
        UI_safethrow(UIError_NullPtr, ;);
    UI_try(type->deserialize(dtsod), _m_ui, ;);
    return _m_ui;
}


//////////////////////////////////////
//            UIContext             //
//////////////////////////////////////

void _UIContext_freeMembers(void* _u){
    UIContext* u=_u;
    Hashtable_free(u->ui_elements);
}

void UIContext_destroy(UIContext* u){
    _UIContext_freeMembers(u);
    free(u);
}

kt_define(UIContext, _UIContext_freeMembers, NULL);

UI_Maybe UIDtsodParser_constructUIContext(UIDtsodParser* parser){
    if(parser->context == NULL){
        parser->context=malloc(sizeof(UIContext));
        parser->context->ui_elements=Hashtable_create();
    }

    Autoarr_foreach(parser->file_models, fm,
        i32 file_parser_version;
        Dtsod_tryGet_i64(fm.dtsod, "parser_version", file_parser_version, true);
        if(file_parser_version!=UIDtsodParser_version){
            UI_safethrow(UIError_InvalidVersion,;);
        }
        
        char* file_namespace;
        Dtsod_tryGet_cptr(fm.dtsod, "namespace", file_namespace, true);
        Hashtable_foreach(fm.dtsod, dtsod_elem,
            if(cptr_equals(dtsod_elem.key, "ui")) {
                if(!UniCheckTypePtr(dtsod_elem.value, Autoarr(Unitype)))
                    UI_safethrow(UIError_InvalidFormat, ;);
                Autoarr_Unitype* ui_ar=dtsod_elem.value.VoidPtr;
                Autoarr_foreach(ui_ar, ui_el_dtsod,
                    UI_try(UIElement_deserialize(ui_el_dtsod.VoidPtr),_m_ui,;);
                    UIElement_Ptr new_el=_m_ui.value.VoidPtr;
                    // kprintf("[UIDtsodParser_constructUIContext] %s : %s\n", new_el->name, new_el->type->kt->name);
                    if(new_el->type->onBuild){
                        UI_try(new_el->type->onBuild(new_el, parser->context),_15151,;);
                    }
                    UI_try( UIContext_add(parser->context, file_namespace, new_el), _a76515, ;);
                );
            }
        );
    );

    Autoarr_free(parser->file_models,true);
    parser->file_models=Autoarr_create(UIDtsodFileModel, 64, 32);
    parser->returned_context=true;
    return SUCCESS(UniHeapPtr(UIContext, parser->context));
}


UI_Maybe UIContext_getAny(UIContext* context, char* name){
    Unitype val;
    // check name
    if(!Hashtable_tryGet(context->ui_elements, name, &val))
        UI_safethrow_msg(cptr_concat("can't get <", name, "> from context"), ;);
    return SUCCESS(val);
}

UI_Maybe _UIContext_get(UIContext* context, char* name, ktid type_id){
    UI_try(UIContext_getAny(context, name), _m_ui, ;);
    UIElement* ptr=_m_ui.value.VoidPtr;
    if(ptr->type->kt->id != type_id)
        UI_safethrow_msg(cptr_concat(
            "tried to get ",ktDescriptor_get(type_id)->name, " <",name,"> but it is of type ", ptr->type->kt->name
            ), ;);
    return _m_ui;
}

UI_Maybe UIContext_add(UIContext* context, char* namespace, _UIElement_Ptr _new_el){
    UIElement_Ptr new_el=_new_el;
    Unitype u=UniPtr(new_el->type->kt->id, new_el, true);
    char* namespace_and_name=cptr_concat(namespace, "_", new_el->name);
    if(!Hashtable_tryAdd(context->ui_elements, namespace_and_name, u))
        UI_safethrow_msg(cptr_concat("element with name <", new_el->name, "> already exists in context"), 
        free(namespace_and_name));
    return MaybeNull;
}
