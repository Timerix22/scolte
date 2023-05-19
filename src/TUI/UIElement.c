#include "tui_internal.h"

kt_define(UIElement, NULL, NULL);
Autoarr_define(UIElement_Ptr, true);
Array_define(UIElement);


// ktid is needed to check if it is uninitilized by calling ktDescriptor_get
UIElement __UIElement_createDefault(ktid typeId, UIElement_draw_t drawFunc){
    return (UIElement){
        .type=ktDescriptor_get(typeId),
        .min_width=2,
        .min_height=2,
        .width_scaling=UIElement_no_scaling,
        .height_scaling=UIElement_no_scaling,
        .color=kp_fgWhite|kp_bgBlack,
        .border={
            .left=UIBorder_Thin, .right=UIBorder_Thin,
            .top=UIBorder_Thin, .bottom=UIBorder_Thin,
            .color=kp_bgBlack|kp_fgGray
        },
        .draw=drawFunc
    };
}

void UIElement_destroy(UIElement_Ptr self){
    if(self->type->freeMembers)
        self->type->freeMembers(self);
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
