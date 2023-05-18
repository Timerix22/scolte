#include "tui_internal.h"

void TextBlock_freeMembers(void* _self){
    TextBlock* self=(TextBlock*)_self;
    free(self->text.ptr);
}

UI_Maybe TextBlock_draw(Renderer* renderer, UIElement* _self, const DrawingArea area){
    TextBlock* self=(TextBlock*)_self;
    UI_try(UIElement_validate((UIElement*)self, area),_0,;);
    UI_try(Renderer_fill(renderer, TCI(TERMCHAR(' '), kp_bgBlack|kp_fgGray), area),_2,;);
    UI_try(Renderer_drawBorder(renderer, self->base.border, area),_1,;);
    for(u16 i=0; i<area.w-2 && i<self->text.length; i++){
        u16 x=area.x+1+i;
        u16 y=area.y+area.h/2;
        UI_try(Renderer_set(renderer, TCI(self->text.ptr[i], kp_fgCyan), x, y), _2,;);
    }
    return MaybeNull;
}

kt_define(TextBlock, TextBlock_freeMembers, NULL);

TextBlock* TextBlock_create(string text){
    TextBlock* textBlock=malloc(sizeof(TextBlock));
    textBlock->base=__UIElement_createDefault(ktid_name(TextBlock), TextBlock_draw);
    textBlock->text=string_copy(text);
    return textBlock;
}
