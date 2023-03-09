#include "tui_internal.h"

void Canvas_freeMembers(void* _self){
    Canvas* self=(Canvas*)_self;
    Autoarr_foreach(self->children, ch, 
        UIElement_destroy((UIElement*)ch));
    Autoarr_freeWithoutMembers(self->children, true);
}

UI_Maybe Canvas_draw(Renderer* renderer, UIElement* _self, DrawingArea area){
    Canvas* self=(Canvas*)_self;
    Autoarr_foreach(self->children, ch, ({
        if(ch==NULL)
            UI_safethrow(UIError_NullPtr,;);
        UI_try(UIElement_draw(renderer, (UIElement*)ch, area),_,;);
    }));
    return MaybeNull;
}

kt_define(Canvas, Canvas_freeMembers, NULL);

Canvas* Canvas_create(){
    Canvas* canvas=malloc(sizeof(Canvas));
    canvas->base=__UIElement_createDefault(ktid_name(Canvas), Canvas_draw);
    canvas->children=Autoarr_create(Pointer, 32, 64);
    return canvas;
}

void Canvas_addChild(Canvas* self, UIElement* child){
    Autoarr_add(self->children, child);
}
