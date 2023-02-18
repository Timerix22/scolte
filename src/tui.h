#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../kerep/src/base/base.h"
#include "../kerep/src/String/string.h"
#include "../kerep/src/kprint/kprint_colors.h"
#include "../kerep/src/Array/Array.h"
#include "terminal.h"


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

PACKED_ENUM(UIBorderType,
    UIBorder_NoBorder,
    UIBorder_Thin,
    UIBorder_Thick,
    UIBorder_Double
)

//////////////////////////////////////
//          Small structs           //
//////////////////////////////////////

STRUCT(DrawingArea,
    /* right-top corner */
    u16 x; u16 y;
    u16 w; u16 h;
)

STRUCT(UIBorderParams,
    UIBorderType right;
    UIBorderType left;
    UIBorderType top;
    UIBorderType bottom;
)

//////////////////////////////////////
//    UIElement abstract struct     //
//////////////////////////////////////

typedef struct UIElement UIElement;
typedef UIElement* UIElementPtr;
typedef void (*UIElement_draw_t)(UIElement* self,DrawingArea place);

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
    UIBorderParams borders;
    UIElement_draw_t draw;
)

UIElement __UIElement_createDefault(ktid typeId, UIElement_draw_t drawFunc);
/// proper way to free UIElement and all its members 
void UIElement_destroy(UIElement* self);
#define UIElement_draw(UIE_PTR, PLACE_RECT) ((UIElement*)UIE_PTR)->draw((UIElement*)UIE_PTR, PLACE_RECT)

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
