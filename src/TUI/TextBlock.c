#include "tui_internal.h"

void TextBlock_freeMembers(void* _self){
    TextBlock* self=(TextBlock*)_self;
    free(self->text.ptr);
}

UI_Maybe TextBlock_draw(Renderer* renderer, UIElement* _self, DrawingArea area){
    TextBlock* self=(TextBlock*)_self;
    UI_try(UIElement_validate((UIElement*)self, area),_0,;);
    UI_try(Renderer_fill(renderer, TERMCHAR(' '), area),_2,;);
    UI_try(Renderer_drawBorder(renderer, self->base.borders, area),_1,;);
    return MaybeNull;
}

kt_define(TextBlock, TextBlock_freeMembers, NULL);

TextBlock* TextBlock_create(string text){
    TextBlock* textBlock=malloc(sizeof(TextBlock));
    textBlock->base=__UIElement_createDefault(ktid_name(TextBlock), TextBlock_draw);
    textBlock->text=string_copy(text);
    return textBlock;
}
