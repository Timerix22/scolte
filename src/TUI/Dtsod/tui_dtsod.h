#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../../../kerep/src/DtsodParser/DtsodV24.h"
#include "../UIError.h"

typedef Hashtable Dtsod;
typedef void* _UIElement_Ptr;

/*
HOW TO USE:
UIDtsodParser* p=UIDtsodParser_create();
UI_try(UIDtsodParser_parseFile("some_file.tui.dtsod"), _91624, UIDtsodParser_destroy(p));
UI_try(UIDtsodParser_parseFile("another_file.tui.dtsod"), _95927, UIDtsodParser_destroy(p));
UI_try(UIDtsodParser_constructUIContext(p), _m_ui_context, UIDtsodParser_destroy(p));
UIContext* ui_context=_m_ui_context.value.VoidPtr;
UIDtsodParser_destroy(p);
UIContext_get(ui_context, example_namespace, text_box, TextBlock, UIContext_destroy(ui_context));
example_namespace_text_box->text=string_fromCptr("text replaced");
*/

//////////////////////////////////////
//          UIDtsodParser           //
//////////////////////////////////////
#define UIDtsodParser_version 1

typedef struct UIDtsodParser UIDtsodParser;

UIDtsodParser* UIDtsodParser_create();
void UIDtsodParser_destroy(UIDtsodParser* p);

///@return UI_Maybe<void>
UI_THROWING_FUNC_DECL(UIDtsodParser_parseFile(UIDtsodParser* parser, char* file_name));

///@return UI_Maybe<void>
UI_THROWING_FUNC_DECL(UIDtsodParser_parseText(UIDtsodParser* parser, char* file_name_placeholder, char* text));

///@return UI_Maybe<UIContext*>
UI_THROWING_FUNC_DECL(UIDtsodParser_constructUIContext(UIDtsodParser* parser));

//////////////////////////////////////
//            UIContext             //
//////////////////////////////////////
typedef struct UIContext UIContext;

void UIContext_destroy(UIContext* u);

///@return UI_Maybe<UIElement*> 
UI_THROWING_FUNC_DECL(_UIContext_get(UIContext* context, char* name, ktid type_id));
#define UIContext_get(CONTEXT, NAMESPACE, NAME, TYPE, FREECALLS_ON_ERROR) \
    UI_try( \
        _UIContext_get(CONTEXT, #NAMESPACE "_" #NAME, ktid_name(TYPE)), \
        _m_##NAMESPACE##_##NAME, \
        FREECALLS_ON_ERROR); \
    TYPE* NAMESPACE##_##NAME=_m_##NAMESPACE##_##NAME.value.VoidPtr;

///@return Maybe<void>
UI_THROWING_FUNC_DECL(UIContext_add(UIContext* context, char* namespace, _UIElement_Ptr new_el));


//////////////////////////////////////
//         Deserialization          //
//////////////////////////////////////

/// universal function for UIElement ancestors deserializing
/// @return Maybe<UIElement_Ptr>
UI_THROWING_FUNC_DECL( UIElement_deserialize(Dtsod* dtsod) );


#define __def_Dtsod_tryGet(dtsod, key, var_name, is_neccecary, TYPE, TYPE_ID, UNI_FIELD, code...) { \
    Unitype uni; \
    if(Hashtable_tryGet(dtsod, key, &uni)) { \
        if(!UniCheckTypeId(uni, TYPE_ID)) \
            UI_safethrow_msg(cptr_concat("tried to get '",key,"' of type '",#TYPE, \
                "', but got invalid type id '",toString_i64(uni.typeId),"'"), ;); \
        var_name=uni.UNI_FIELD; \
        { code; } \
    } \
    else if(is_neccecary) \
        UI_safethrow_msg(cptr_concat("can't find key '", key, "'"), ;); \
}

#define Dtsod_tryGet_i64(dtsod, key, var_name, is_neccecary, code...) \
    __def_Dtsod_tryGet(dtsod, key, var_name, is_neccecary, i64, ktid_name(i64), Int64, code)

#define Dtsod_tryGet_u64(dtsod, key, var_name, is_neccecary, code...) \
    __def_Dtsod_tryGet(dtsod, key, var_name, is_neccecary, u64, ktid_name(u64), UInt64, code)

#define Dtsod_tryGet_f64(dtsod, key, var_name, is_neccecary, code...) \
    __def_Dtsod_tryGet(dtsod, key, var_name, is_neccecary, f64, ktid_name(f64), Float64, code)

#define Dtsod_tryGet_bool(dtsod, key, var_name, is_neccecary, code...) \
    __def_Dtsod_tryGet(dtsod, key, var_name, is_neccecary, bool, ktid_name(bool), Bool, code)

#define Dtsod_tryGet_ptr(dtsod, key, type, var_name, is_neccecary, code...) \
    __def_Dtsod_tryGet(dtsod, key, var_name, is_neccecary, type*, ktid_ptrName(type), VoidPtr, code)

#define Dtsod_tryGet_cptr(dtsod, key, var_name, is_neccecary, code...) \
    Dtsod_tryGet_ptr(dtsod, key, char, var_name, is_neccecary, code)

#define Dtsod_tryGet_Hashtable(dtsod, key, var_name, is_neccecary, code...) \
    Dtsod_tryGet_ptr(dtsod, key, Hashtable, var_name, is_neccecary, code)

#define Dtsod_tryGet_Autoarr(dtsod, key, var_name, is_neccecary, code...) \
    Dtsod_tryGet_ptr(dtsod, key, Autoarr(Unitype), var_name, is_neccecary, code)



#if __cplusplus
}
#endif