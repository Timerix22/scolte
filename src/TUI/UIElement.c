#include "tui_internal.h"

kt_define(UIElement, NULL, NULL);
Autoarr_define(UIElement_Ptr, true);
Array_define(UIElement);

// ktid is needed to check if it is uninitilized by calling ktDescriptor_get
inline UIElement _UIElement_initBaseDefault(char* name, UITDescriptor* uit){
    return (UIElement){
        .name=name,
        .ui_type=uit,
        .min_width=2,
        .min_height=2,
        .width_scaling=UIElement_no_scaling,
        .height_scaling=UIElement_no_scaling,
        .color=kp_bgBlack|kp_fgGray,
        .border={
            .left=UIBorder_Thin, .right=UIBorder_Thin,
            .top=UIBorder_Thin, .bottom=UIBorder_Thin,
            .color=kp_bgBlack|kp_fgGray
        },
    };
}

void UIElement_destroy(UIElement_Ptr self){
    if(self->ui_type->type->freeMembers)
        self->ui_type->type->freeMembers(self);
    free(self);
}

UI_Maybe DrawingArea_validate(DrawingArea a){
    if(a.h<1) UI_safethrow(UIError_InvalidWidth,;);
    if(a.w<1) UI_safethrow(UIError_InvalidHeight,;);
    return MaybeNull;
}

UI_Maybe UIElement_validate(UIElement_Ptr u, DrawingArea a){
    UI_try(DrawingArea_validate(a),_,;);
    if(u->min_height>a.h)
        UI_safethrow(UIError_InvalidHeight,;);
    if(u->min_width>a.w)
        UI_safethrow(UIError_InvalidWidth,;);
    return MaybeNull;
}


UI_Maybe UIElement_deserializeBase(Dtsod* dtsod, UIElement* base){
    Unitype uni;
    Dtsod_get_necessary(dtsod, "name");
    char* name=uni.VoidPtr;
    Dtsod_get_necessary(dtsod, "type");
    UITDescriptor* ui_type=uni.VoidPtr;
    *base=_UIElement_initBaseDefault(name, ui_type);
    Dtsod_get_optional(dtsod, "min_width")
        base->min_width=uni.UInt64;
    Dtsod_get_optional(dtsod, "min_height")
        base->min_height=uni.UInt64;
    Dtsod_get_optional(dtsod, "width_scaling")
        base->width_scaling=uni.UInt64;
    Dtsod_get_optional(dtsod, "height_scaling")
        base->height_scaling=uni.UInt64;
    Dtsod_get_optional(dtsod, "bg_color")
        set_color(uni.VoidPtr, base->color);
    Dtsod_get_optional(dtsod, "fg_color")
        set_color(uni.VoidPtr, base->color);
    base->border.color=base->color;
    Dtsod_get_optional(dtsod, "border"){
        Dtsod* border_dtsod=uni.VoidPtr;
        Dtsod_get_optional(border_dtsod, "top")
            set_border_thickness(uni.VoidPtr, base->border.top);
        Dtsod_get_optional(border_dtsod, "bottom")
            set_border_thickness(uni.VoidPtr, base->border.bottom);
        Dtsod_get_optional(border_dtsod, "left")
            set_border_thickness(uni.VoidPtr, base->border.left);
        Dtsod_get_optional(border_dtsod, "right")
            set_border_thickness(uni.VoidPtr, base->border.right);
        Dtsod_get_optional(dtsod, "bg_color")
            set_color(uni.VoidPtr, base->border.color);
        Dtsod_get_optional(dtsod, "fg_color")
            set_color(uni.VoidPtr, base->border.color);
    }

    return MaybeNull;
}
