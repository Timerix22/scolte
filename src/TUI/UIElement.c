#include "tui_internal.h"

void UIElement_appendToStringBuilder(StringBuilder* sb, UIElement_Ptr u){
    char *s0;
    addsf(u,name) addc(' ')
    adds((s0=UITDescriptor_toString(u->type)));
    adduf(u,min_width)
    adduf(u,min_height)
    addif(u,width)
    addif(u,height)
    adds(" color:") adds(kp_bgColor_toString(u->color))
        addc('|') adds(kp_fgColor_toString(u->color))
    adds(" border { t:") addu(u->border.top)
        adds(" b:") addu(u->border.bottom)
        adds(" l:") addu(u->border.left)
        adds(" r:") addu(u->border.right)
        adds(" color:") adds(kp_bgColor_toString(u->color))
            addc('|') adds(kp_fgColor_toString(u->color))
        adds(" }")
    free(s0);
}


kt_define(UIElement, NULL, NULL);
Autoarr_define(UIElement_Ptr, true);
Array_define(UIElement);

// ktid is needed to check if it is uninitilized by calling ktDescriptor_get
inline UIElement _UIElement_initBaseDefault(char* name, UITDescriptor* uit){
    return (UIElement){
        .name=name,
        .type=uit,
        .min_width=2,
        .min_height=2,
        .width=size_enable_scaling(1),
        .height=size_enable_scaling(1),
        .color=kp_bgBlack|kp_fgGray,
        .border={
            .left=UIBorder_Thin, .right=UIBorder_Thin,
            .top=UIBorder_Thin, .bottom=UIBorder_Thin,
            .color=kp_bgBlack|kp_fgGray
        },
    };
}

void UIElement_destroy(UIElement_Ptr self){
    if(self->type->kt->freeMembers)
        self->type->kt->freeMembers(self);
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
    char* name;
    Dtsod_tryGet_cptr(dtsod, "name", name, true);

    char* type_name;
    Dtsod_tryGet_cptr(dtsod, "type", type_name, true);
    UITDescriptor* type=UITDescriptor_getByName(type_name);
    if(type==NULL)
        UI_safethrow_msg(cptr_concat("invalid type '", type_name, "'"), ;);
        
    *base=_UIElement_initBaseDefault(cptr_copy(name), type);

    Dtsod_tryGet_i64(dtsod, "min_width", base->min_width, false);
    Dtsod_tryGet_i64(dtsod, "min_height", base->min_height, false);
    Dtsod_tryGet_ScalingSize(dtsod, "width", base->width, false);
    Dtsod_tryGet_ScalingSize(dtsod, "height", base->height, false);
    char *bg_color_name, *fg_color_name;
    Dtsod_tryGet_cptr(dtsod, "bg_color", bg_color_name, false,
        set_color(bg_color_name, base->color);
    );
    Dtsod_tryGet_cptr(dtsod, "fg_color", fg_color_name, false,
        set_color(fg_color_name, base->color);
    );
    base->border.color=base->color;

    Dtsod* border_dtsod;
    Dtsod_tryGet_Hashtable(dtsod, "border", border_dtsod, false, {
        char* border_thickness_str;
        Dtsod_tryGet_cptr(border_dtsod, "top", border_thickness_str, false,
            set_border_thickness(border_thickness_str, base->border.top);
        );
        Dtsod_tryGet_cptr(border_dtsod, "bottom", border_thickness_str, false,
            set_border_thickness(border_thickness_str, base->border.bottom);
        );
        Dtsod_tryGet_cptr(border_dtsod, "left", border_thickness_str, false,
            set_border_thickness(border_thickness_str, base->border.left);
        );
        Dtsod_tryGet_cptr(border_dtsod, "right", border_thickness_str, false,
            set_border_thickness(border_thickness_str, base->border.right);
        );
        char *bg_color_name, *fg_color_name;
        Dtsod_tryGet_cptr(dtsod, "bg_color", bg_color_name, false,
            set_color(bg_color_name, base->border.color);
        );
        Dtsod_tryGet_cptr(dtsod, "fg_color", fg_color_name, false,
            set_color(fg_color_name, base->border.color);
        );
    });

    return MaybeNull;
}
