#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../../kerep/src/String/string.h"
#include "../../kerep/src/kprint/kprint_colors.h"
#include "../../kerep/src/Array/Array.h"
#include "../term/term.h"
#include "unicode.h"
#include "UIError.h"


/// initializes type descriptors for this project's types
/// call this function between kt_beginInit() and kt_endInit()
void kt_initScolteTypes();

//////////////////////////////////////
//              Enums               //
//////////////////////////////////////

PACKED_ENUM(UIAnchor,
    UIAnchor_Center=0,
    UIAnchor_Left=1,
    UIAnchor_Right=2,
    UIAnchor_Top=4,
    UIAnchor_Bottom=8,
    UIAnchor_RightTop=UIAnchor_Right|UIAnchor_Top,
    UIAnchor_RightBottom=UIAnchor_Right|UIAnchor_Bottom,
    UIAnchor_LeftTop=UIAnchor_Left|UIAnchor_Top,
    UIAnchor_LeftBottom=UIAnchor_Left|UIAnchor_Bottom
)

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
)

//////////////////////////////////////
//             Renderer             //
//////////////////////////////////////

STRUCT(FrameBuffer,
    utfchar* data;
    TerminalSize size;
)

typedef struct Renderer Renderer;

STRUCT(Renderer,
    FrameBuffer frameBuffer;
    UI_THROWING_FUNC_DECL((*drawFrame)(Renderer*));
    UI_THROWING_FUNC_DECL((*set)(Renderer*, utfchar c, u16 x, u16 y));
)

#define Renderer_drawFrame(RENDERER) RENDERER->drawFrame(RENDERER)
#define Renderer_set(RENDERER, CH, X, Y) RENDERER->set(RENDERER, CH, X, Y)

Renderer* Renderer_create();
void Renderer_destroy(Renderer* self);

UI_THROWING_FUNC_DECL(Renderer_fill(Renderer* renderer, utfchar c, DrawingArea area));
UI_THROWING_FUNC_DECL(Renderer_drawLineX(Renderer* renderer, utfchar c, u16 x, u16 y, u16 length));
UI_THROWING_FUNC_DECL(Renderer_drawLineY(Renderer* renderer, utfchar c, u16 x, u16 y, u16 length));
UI_THROWING_FUNC_DECL(Renderer_drawBorder(Renderer* renderer, UIBorder border, DrawingArea area));

//////////////////////////////////////
//    UIElement abstract struct     //
//////////////////////////////////////

typedef struct UIElement UIElement;
typedef UIElement* UIElementPtr;
typedef UI_THROWING_FUNC_DECL((*UIElement_draw_t)(Renderer* renderer, UIElement* self, DrawingArea area));

#define UIElement_stretch (u16)-1

STRUCT(UIElement,
    ktDescriptor* type;
    u16 min_width;
    u16 max_width;
    u16 min_height;
    u16 max_height;
    kp_fgColor fgColor;
    kp_bgColor bgColor;
    UIAnchor anchor;
    UIBorder borders;
    UIElement_draw_t draw;
)

/// proper way to free UIElement and all its members 
void UIElement_destroy(UIElement* self);
#define UIElement_draw(RENDERER, UIE_PTR, PLACE_RECT) \
    ((UIElement*)UIE_PTR)->draw(RENDERER, UIE_PTR, PLACE_RECT)

//////////////////////////////////////
//      UIElement derivatives       //
//////////////////////////////////////

//////          Canvas          //////

STRUCT(Canvas,
    UIElement base;
    /* Autoarr(UIElementPtr) */
    Autoarr(Pointer)* children;
)

Canvas* Canvas_create();
void Canvas_addChild(Canvas* self, UIElement* child);


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
