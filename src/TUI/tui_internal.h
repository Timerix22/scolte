#pragma once

#if __cplusplus
extern "C" {
#endif

#include "tui.h"

extern UIElement _UIElement_initBaseDefault(char* name, UITDescriptor* type);

UI_THROWING_FUNC_DECL( DrawingArea_validate(DrawingArea a) );
UI_THROWING_FUNC_DECL( UIElement_validate(UIElement_Ptr u, DrawingArea a) );

UI_THROWING_FUNC_DECL( UIElement_deserializeBase(Dtsod* dtsod, UIElement* base) );

extern termchar UIBorder_char_h[4];
extern termchar UIBorder_char_v[4];
extern termchar UIBorder_char_lt[4][4];
extern termchar UIBorder_char_rt[4][4];
extern termchar UIBorder_char_rb[4][4];
extern termchar UIBorder_char_lb[4][4];

/// extended kt_register
void __uit_register(ktDescriptor* kt, UITDescriptor* uit);
#define uit_register(TYPE) kt_register(TYPE); __uit_register(&ktDescriptor_##TYPE##_Ptr, &UITDescriptor_##TYPE)


void UI_enum_tables_init();
void UI_enum_tables_free();

int termcolor_getByName(char* color_name);
int UIBorderThickness_getByName(char* name);

#define set_color(name, var){ \
    int color=termcolor_getByName(name); \
    if(color==-1) \
        UI_safethrow(cptr_concat("incorrect color name: ", name), ;); \
    var|=(termcolor)color; \
}

#define set_border_thickness(name, var){ \
    int th=UIBorderThickness_getByName(name); \
    if(th==-1) \
        UI_safethrow(cptr_concat("incorrect UIBorderThickness name: ", name), ;); \
    var=(UIBorderThickness)th; \
}

#if __cplusplus
}
#endif
