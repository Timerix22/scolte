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
UI_THROWING_FUNC_DECL(UIContext_add(UIContext* context, _UIElement_Ptr new_el));


//////////////////////////////////////
//         Deserialization          //
//////////////////////////////////////

/// universal function for UIElement ancestors deserializing
/// @return Maybe<UIElement_Ptr>
UI_THROWING_FUNC_DECL( UIElement_deserialize(Dtsod* dtsod) );


#define Dtsod_get_necessary(dtsod, key) \
    if(!Hashtable_tryGet(dtsod, key, &uni)) \
        UI_safethrow(UIError_NullPtr, ;);

#define Dtsod_get_optional(dtsod, key) \
    if(Hashtable_tryGet(dtsod, key, &uni))


#if __cplusplus
}
#endif