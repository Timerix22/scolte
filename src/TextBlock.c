#include "tui.h"


void TextBlock_freeMembers(void* _self){
    TextBlock* self=(TextBlock*)_self;
    free(self->text.ptr);
}

void TextBlock_draw(UIElement* _self, Rect place){
    TextBlock* self=(TextBlock*)_self;
    fputs(self->text.ptr, stdout);
}

kt_define(TextBlock, TextBlock_freeMembers, NULL);

TextBlock* TextBlock_create(string text){
    TextBlock* TextBlock=malloc(sizeof(TextBlock));
    TextBlock->base=__UIElement_createDefault(ktid_name(TextBlock), TextBlock_draw);
    TextBlock->text=string_copy(text);
    return TextBlock;
}
