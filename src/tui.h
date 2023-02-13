#include "../kerep/src/base/base.h"
#include "../kerep/src/String/string.h"
#include "../kerep/src/kprint/kprint_colors.h"

PACKED_ENUM(UIAnchor,
    UI_Center=0,
    UI_Left=1,
    UI_Right=2,
    UI_Top=4,
    UI_Bottom=8,
    UI_RightTop=UI_Right|UI_Top,
    UI_RightBottom=UI_Right|UI_Bottom,
    UI_LeftTop=UI_Left|UI_Top,
    UI_LeftBottom=UI_Left|UI_Bottom
)

typedef struct UIElement; 

#define UIElement_stretch (u16)-1

STRUCT(Rect, // Rectangle
    // right-top corner
    u16 x; u16 y;
    u16 w; u16 h;
)

typedef void (*UIElement_draw_t)(UIElement,Rect);

PACKED_ENUM(UIBorderType,
    UIBorderType_NoBorder,
    UIBorderType_Thin,
    UIBorderType_Thick,
    UIBorderType_Double
)

STRUCT(UIBorderParams,
    UIBorderType right;
    UIBorderType left;
    UIBorderType top;
    UIBorderType bottom;
)

STRUCT(UIElement,
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

STRUCT(TextBox,
    UIElement base;
    string text;
)

void UIElement_draw(UIElement uie); 
