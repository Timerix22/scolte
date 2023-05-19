#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../../kerep/src/String/string.h"
#include "../../kerep/src/Array/Array.h"
#include "../term/term.h"
#include "../encoding/encoding.h"
#include "UIError.h"


/// initializes type descriptors for this project's types
/// call this function between kt_beginInit() and kt_endInit()
void kt_initScolteTypes();

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

typedef struct UIElement UIElement;
typedef UIElement* UIElement_Ptr;
typedef UI_THROWING_FUNC_DECL((*UIElement_draw_t)(Renderer* renderer, UIElement_Ptr self, const DrawingArea area));

#define UIElement_no_scaling (u16)0

STRUCT(UIElement,
    ktDescriptor* type;
    u16 min_width;
    u16 min_height;
    u16 width_scaling; 
    u16 height_scaling; 
    termcolor color;
    UIBorder border;
    UIElement_draw_t draw;
)

/// proper way to free UIElement and all its members 
void UIElement_destroy(UIElement_Ptr self);
#define UIElement_draw(RENDERER, UIE_PTR, PLACE_RECT) \
    ((UIElement_Ptr)UIE_PTR)->draw(RENDERER, UIE_PTR, PLACE_RECT)

Autoarr_declare(UIElement_Ptr)
Array_declare(UIElement)

//////////////////////////////////////
//      UIElement derivatives       //
//////////////////////////////////////

//////           Grid           //////

STRUCT(Grid,
    UIElement base;
    u16 columns;
    u16 rows;
    UIElement_Ptr* ui_elements; /* UIElement[rows][columns] */
)

Grid* Grid_create(u16 columns, u16 rows, UIElement_Ptr* ui_elements);
UIElement_Ptr Grid_get(Grid* grid, u16 column, u16 row);
void Grid_set(Grid* grid, u16 column, u16 row, UIElement_Ptr value);


//////         TextBlock        //////

STRUCT(TextBlock,
    UIElement base;
    string text;
)

/// creates a TextBlock with a copy of text
TextBlock* TextBlock_create(string text);

#if __cplusplus
}
#endif
