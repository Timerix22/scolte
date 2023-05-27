#include "tui_internal.h"

void TextBlock_freeMembers(void* _self){
    // TextBlock* self=(TextBlock*)_self;
    // free(self->text.ptr);
}

UI_Maybe TextBlock_draw(Renderer* renderer, UIElement_Ptr _self, const DrawingArea area){
    TextBlock* self=(TextBlock*)_self;
    UI_try(UIElement_validate((UIElement_Ptr)self, area),_0,;);
    UI_try(Renderer_fill(renderer, TCI(TERMCHAR(' '), kp_bgBlack|kp_fgGray), area),_2,;);
    UI_try(Renderer_drawBorder(renderer, self->base.border, area),_1,;);
    for(u16 i=0; i<area.w-2 && i<self->text.length; i++){
        u16 x=area.x+1+i;
        u16 y=area.y+area.h/2;
        UI_try(Renderer_set(renderer, TCI(self->text.ptr[i], kp_fgCyan), x, y), _2,;);
    }
    return MaybeNull;
}

UI_Maybe TextBlock_deserialize(Dtsod* dtsod){
    TextBlock tb;

    UI_try(UIElement_deserializeBase(dtsod, &tb.base), _8751, ;);
    char* _text;
    Dtsod_tryGet_cptr(dtsod, "text", _text, true);
    tb.text=string_fromCptr(_text);
    TextBlock* ptr=malloc(sizeof(*ptr));
    
    *ptr=tb;
    return SUCCESS(UniHeapPtr(TextBlock, ptr));
}

char* TextBlock_toString(void* _tb, kp_fmt fmt){
    TextBlock* tb=_tb;
    StringBuilder* sb=StringBuilder_create();
    adds("TextBlock {")
    UIElement_appendToStringBuilder(sb, &tb->base);
    addstrf(tb,text)
    adds(" }")
    return StringBuilder_build(sb).ptr;
}

uit_define(TextBlock, TextBlock_freeMembers, TextBlock_toString, TextBlock_draw, TextBlock_deserialize, NULL);

TextBlock* TextBlock_create(char* name, string text){
    TextBlock* textBlock=malloc(sizeof(TextBlock));
    textBlock->base=_UIElement_initBaseDefault(name, &UITDescriptor_TextBlock);
    textBlock->text=text;
    return textBlock;
}
