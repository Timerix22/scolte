#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../../kerep/src/String/string.h"
#include "../../kerep/src/Array/Array.h"
#include "../term/term.h"
#include "../encoding/encoding.h"
#include "Dtsod/tui_dtsod.h"
#include "UIError.h"

//////////////////////////////////////
//            Prototypes            //
//////////////////////////////////////

typedef struct UIElement UIElement;
typedef UIElement* UIElement_Ptr;
typedef struct Renderer Renderer;
typedef struct DrawingArea DrawingArea;

///@return Maybe<UIElement_Ptr>
typedef UI_THROWING_FUNC_DECL( (*UIElement_draw_t)(Renderer* renderer, UIElement_Ptr self, const DrawingArea area) );

///@return Maybe<UIElement_Ptr>
typedef UI_THROWING_FUNC_DECL( (*UIElement_deserialize_t)(Dtsod* dtsod) );


//////////////////////////////////////
//          UI Type System          //
//////////////////////////////////////

STRUCT(UITDescriptor,
    ktDescriptor* type;
    UIElement_draw_t draw;
    UIElement_deserialize_t deserialize;
)

#define uit_declare(TYPE) extern UITDescriptor UITDescriptor_##TYPE;

#define uit_define(TYPE, FREE_F, TOSTRING_F, DRAW_F, DESERIALIZE_F) kt_define(TYPE, FREE_F, TOSTRING_F) \
    UITDescriptor UITDescriptor_##TYPE={ \
        .draw=DRAW_F, \
        .deserialize=DESERIALIZE_F \
    };

/// call this between kt_beginInit() and kt_endInit()
void kt_beginInitTUI();
/// call this between kt_beginInitTUI() and kt_endInitTUI()
void Scolte_init();
/// call this between kt_beginInit() and kt_endInit()
void kt_endInitTUI();
// call this before kt_free()
void Scolte_free();

UITDescriptor* UITDescriptor_getById(ktid i);
UITDescriptor* UITDescriptor_getByName(char* name);

//////////////////////////////////////
//              Enums               //
//////////////////////////////////////

PACKED_ENUM(UIBorderThickness,
    UIBorder_Hidden, /* blank space */
    UIBorder_Thin,
    UIBorder_Thick,
    UIBorder_Double,
    UiBorder_NoBorder /* no space */
)


//////////////////////////////////////
//          Small structs           //
//////////////////////////////////////

STRUCT(DrawingArea,
    /* right-top corner */
    u16 x; u16 y;
    u16 w; u16 h;
)

STRUCT(UIBorder,
    UIBorderThickness right;
    UIBorderThickness left;
    UIBorderThickness top;
    UIBorderThickness bottom;
    termcolor color;
)


//////////////////////////////////////
//             Renderer             //
//////////////////////////////////////

STRUCT(FrameBuffer,
    TermCharInfo* data;
    TerminalSize size;
)

typedef struct Renderer Renderer;

STRUCT(Renderer,
    FrameBuffer frameBuffer;
    UI_THROWING_FUNC_DECL((*drawFrame)(Renderer*));
    UI_THROWING_FUNC_DECL((*set)(Renderer*, TermCharInfo tci, u16 x, u16 y));
)

#define Renderer_drawFrame(RENDERER) RENDERER->drawFrame(RENDERER)
#define Renderer_set(RENDERER, TCI, X, Y) RENDERER->set(RENDERER, TCI, X, Y)

Renderer* Renderer_create();
void Renderer_destroy(Renderer* self);

UI_THROWING_FUNC_DECL(Renderer_fill(Renderer* renderer, TermCharInfo tci, const DrawingArea area));
UI_THROWING_FUNC_DECL(Renderer_drawLineX(Renderer* renderer, TermCharInfo tci, u16 x, u16 y, u16 length));
UI_THROWING_FUNC_DECL(Renderer_drawLineY(Renderer* renderer, TermCharInfo tci, u16 x, u16 y, u16 length));
UI_THROWING_FUNC_DECL(Renderer_drawBorder(Renderer* renderer, UIBorder border, const DrawingArea area));


//////////////////////////////////////
//    UIElement abstract struct     //
//////////////////////////////////////

STRUCT(UIElement,
    UITDescriptor* ui_type;
    char* name;
    u16 min_width;
    u16 min_height;
    u16 width_scaling; 
    u16 height_scaling; 
    termcolor color;
    UIBorder border;
)
Autoarr_declare(UIElement_Ptr)
Array_declare(UIElement)

#define UIElement_no_scaling (u16)0

/// proper way to free UIElement and all its members 
void UIElement_destroy(UIElement_Ptr self);

#define UIElement_draw(RENDERER, UIE_PTR, PLACE_RECT) \
    (UIE_PTR)->ui_type->draw(RENDERER, UIE_PTR, PLACE_RECT)


//////////////////////////////////////
//      UIElement derivatives       //
//////////////////////////////////////

//////           Grid           //////

STRUCT(Grid,
    UIElement base;
    u16 columns;
    u16 rows;
    bool is_bound;
    char** content_names;
    UIElement_Ptr* content; /* UIElement[rows][columns] */
)
uit_declare(Grid);

Grid* Grid_create(char* name, u16 columns, u16 rows, UIElement_Ptr* content);
///@return Maybe<UIElement*>
UI_THROWING_FUNC_DECL( Grid_get(Grid* grid, u16 column, u16 row) );
///@return maybe<void>
UI_THROWING_FUNC_DECL( Grid_set(Grid* grid, u16 column, u16 row, UIElement_Ptr value));
///@return Maybe<char*>
UI_THROWING_FUNC_DECL( _Grid_getName(Grid* grid, u16 column, u16 row) );
///@return maybe<void>
UI_THROWING_FUNC_DECL( _Grid_bindContent(Grid* grid, UIContext* context) );

#define Grid_foreach(GRID, ELEM_VAR_NAME, CODE...) { \
    if(!GRID->is_bound) \
        UI_safethrow_msg(cptr_concat("grid '", GRID->base.ui_type->type->name, \
        "' content has not been bound"),;); \
    for(u16 _g_r = 0; _g_r < GRID->rows; _g_r++){ \
        for(u16 _g_c = 0; _g_c < GRID->columns; _g_c++){ \
            UI_try(Grid_get(GRID, _g_c, _g_r), _m_g_el,;); \
            UIElement_Ptr ELEM_VAR_NAME=_m_g_el.value.VoidPtr; \
            { CODE; } \
        } \
    } \
}

#define Grid_foreachName(GRID, ELEM_VAR_NAME, CODE...) { \
    if(GRID->is_bound) \
        UI_safethrow_msg(cptr_concat("grid '", GRID->base.ui_type->type->name, \
        "' content has been already bound"),;); \
    for(u16 _g_r = 0; _g_r < GRID->rows; _g_r++){ \
        for(u16 _g_c = 0; _g_c < GRID->columns; _g_c++){ \
            UI_try(_Grid_getName(GRID, _g_c, _g_r), _m_g_nm,;); \
            char* ELEM_VAR_NAME=_m_g_nm.value.VoidPtr; \
            { CODE; } \
        } \
    } \
}

//////         TextBlock        //////

STRUCT(TextBlock,
    UIElement base;
    string text;
)
uit_declare(TextBlock);

/// creates a TextBlock with a copy of text
TextBlock* TextBlock_create(char* name, string text);

#if __cplusplus
}
#endif
