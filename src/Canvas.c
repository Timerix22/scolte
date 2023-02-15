#include "tui.h"

void Canvas_freeMembers(void* _self){
    Canvas* self=(Canvas*)_self;
    Autoarr_foreach(self->children, ch, 
        UIElement_destroy((UIElement*)ch));
    Autoarr_freeWithoutMembers(self->children, true);
}

void Canvas_draw(UIElement* _self, Rect place){
    Canvas* self=(Canvas*)_self;
    Autoarr_foreach(self->children, ch, ({
        UIElement_draw((UIElement*)ch,place);
        fputc('\n', stdout);
    }));
}

kt_define(Canvas, Canvas_freeMembers, NULL);

Canvas* Canvas_create(){
    Canvas* canvas = malloc(sizeof(Canvas));
    canvas->base = __UIElement_createDefault(ktid_name(Canvas), Canvas_draw);
    canvas->children = Autoarr_create(Pointer, 32, 64);
    return canvas;
}

void Canvas_addChild(Canvas* self, UIElement* child){
    if (child == NULL)
        throw(ERR_NULLPTR);
    Autoarr_add(self->children, child);
}
