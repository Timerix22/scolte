#include "tui_internal.h"

kt_define(UIElement, NULL, NULL);


// ktid is needed to check if it is uninitilized by calling ktDescriptor_get
UIElement __UIElement_createDefault(ktid typeId, UIElement_draw_t drawFunc){
    return (UIElement){
        .type=ktDescriptor_get(typeId),
        .min_width=2,
        .max_width=UIElement_stretch,
        .min_height=2,
        .max_height=UIElement_stretch,
        .fgColor=kp_fgWhite,
        .bgColor=kp_bgBlack,
        .anchor=UIAnchor_Center,
        .borders={
            .left=UIBorder_Thin, .right=UIBorder_Thin,
            .top=UIBorder_Thin, .bottom=UIBorder_Thin
        },
        .draw=drawFunc
    };
}

void UIElement_destroy(UIElement* self){
    if(self->type->freeMembers)
        self->type->freeMembers(self);
    free(self);
}


UI_Maybe DrawingArea_validate(DrawingArea a){
    if(a.h<1) UI_safethrow(UIError_InvalidWidth,;);
    if(a.w<1) UI_safethrow(UIError_InvalidHeight,;);
    return MaybeNull;
}

UI_Maybe UIElement_validate(UIElement* u, DrawingArea a){
    UI_try(DrawingArea_validate(a),_,;);
    if(u->max_height<u->min_height)
        UI_safethrow(UIError_InvalidHeight,;);
    if(u->max_width<u->min_width)
        UI_safethrow(UIError_InvalidWidth,;);
    if(u->min_height>a.h)
        UI_safethrow(UIError_InvalidHeight,;);
    if(u->min_width>a.w)
        UI_safethrow(UIError_InvalidWidth,;);
    return MaybeNull;
}
