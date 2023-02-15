#include "tui.h"

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
            .top=UIBorder_Thick, .bottom=UIBorder_Thin
        },
        .draw=drawFunc
    };
}

void UIElement_destroy(UIElement* self){
    if(self->type->freeMembers)
        self->type->freeMembers(self);
    free(self);
}
