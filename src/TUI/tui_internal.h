#pragma once

#if __cplusplus
extern "C" {
#endif

#include "tui.h"

UIElement __UIElement_createDefault(ktid typeId, UIElement_draw_t drawFunc);
UI_THROWING_FUNC_DECL(DrawingArea_validate(DrawingArea a));
UI_THROWING_FUNC_DECL(UIElement_validate(UIElement_Ptr u, DrawingArea a));

extern termchar UIBorder_char_h[4];
extern termchar UIBorder_char_v[4];
extern termchar UIBorder_char_lt[4][4];
extern termchar UIBorder_char_rt[4][4];
extern termchar UIBorder_char_rb[4][4];
extern termchar UIBorder_char_lb[4][4];

#if __cplusplus
}
#endif
