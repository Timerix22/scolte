#include "tui_internal.h"

//////////////////////////////////////
//            FrameBuffer           //
//////////////////////////////////////

void FrameBuffer_freeMembers(void* _self){
    FrameBuffer* self=(FrameBuffer*)_self;
    free(self->data);
}

kt_define(FrameBuffer, FrameBuffer_freeMembers, NULL);

/// creates new frame buffer and fills it with spaces
UI_THROWING_FUNC_DECL(FrameBuffer_create(FrameBuffer* fb));
Maybe FrameBuffer_create(FrameBuffer* fb){
    if(!term_getSize(&fb->size)){
        fb->size=term_default_size;
        //TODO log error
        kprintf("can't get terminal size, fallback to default (%ux%u)\n",
                    term_default_size.cols, term_default_size.rows);
    }
    u32 length=fb->size.cols * fb->size.rows;
    u32 sz=sizeof(*fb->data);
    fb->data=malloc( sz* length);
    
    for(u32 i=0; i<length; i++)
        fb->data[i]=TERMCHAR('#');

    return MaybeNull;
}

void Renderer_freeMembers(void * _self){
    Renderer* self=(Renderer*)_self;
    FrameBuffer_freeMembers(&self->frameBuffer);
}

//////////////////////////////////////
//             Renderer             //
//////////////////////////////////////

UI_Maybe __Renderer_set(Renderer* self, termchar c, u16 x, u16 y) {
    if(x >= self->frameBuffer.size.cols)
        UI_safethrow(UIError_InvalidX,;);
    if(y >= self->frameBuffer.size.rows)
        UI_safethrow(UIError_InvalidY,;);
    self->frameBuffer.data[self->frameBuffer.size.cols * y + x] = c;
    return MaybeNull;
}

#include <errno.h>

UI_Maybe __Renderer_drawFrame(Renderer* self){
    // save current buffer
    FrameBuffer buf=self->frameBuffer;
    // recreate frame buffer
    UI_try(FrameBuffer_create(&self->frameBuffer),_,;);

    for(u16 y=0; y<buf.size.rows; y++){
        for(u16 x=0; x<buf.size.cols; x++){
            termchar c=buf.data[buf.size.cols*y + x];
            int rez=termchar_print(c);
            if(rez<0){
                char* chex=toString_hex(&c, sizeof(c), true, true, true);
                char* errnostr=toString_i64(errno);
                char* errmsg=cptr_concat(nameof(UIError_PrintError)," (errno: ",errnostr,"): can't print char ",chex);
                free(errnostr);
                free(chex);
                UI_safethrow_msg(errmsg,FrameBuffer_freeMembers(&buf));
            }
        }
    }
    fflush(stdout);

    // free saved frame buffer
    FrameBuffer_freeMembers(&buf);
    return MaybeNull;
}

void Renderer_destroy(Renderer* self){
    Renderer_freeMembers(self);
    free(self);
}

kt_define(Renderer, Renderer_freeMembers, NULL);

Renderer* Renderer_create(){
    Renderer* renderer=malloc(sizeof(Renderer));
    tryLast(FrameBuffer_create(&renderer->frameBuffer),_);
    renderer->set=__Renderer_set;
    renderer->drawFrame=__Renderer_drawFrame;
    return renderer;
}

//////////////////////////////////////
//        drawing functions         //
//////////////////////////////////////

UI_Maybe Renderer_fill(Renderer* renderer, termchar c, DrawingArea area){
    UI_try(DrawingArea_validate(area),_,;);
    for(u16 y=area.y; y<area.y+area.h; y++)
        for(u16 x=area.x; x<area.x+area.w; x++){
            UI_try(Renderer_set(renderer, c, x, y),_,;);
        }
    return MaybeNull;
}

UI_Maybe Renderer_drawLineX(Renderer* renderer, termchar c, u16 x, u16 y, u16 length){
    while(length>0){
        UI_try(Renderer_set(renderer, c, x, y),_,;);
        x++; length--;
    }
    return MaybeNull;
}

UI_Maybe Renderer_drawLineY(Renderer* renderer, termchar c, u16 x, u16 y, u16 length){
    while(length>0){
        UI_try(Renderer_set(renderer, c, x, y),_,;);
        y++; length--;
    }
    return MaybeNull;
}

UI_Maybe Renderer_drawBorder(Renderer* renderer, UIBorder border, DrawingArea area){
    UI_try(DrawingArea_validate(area),_0,;);

    //lines
    termchar topChar    = UIBorder_char_h[border.top   ];
    termchar bottomChar = UIBorder_char_h[border.bottom];
    termchar leftChar   = UIBorder_char_v[border.left  ];
    termchar rightChar  = UIBorder_char_v[border.right ];
    // top
    
    // TODO check neighbor borders and insert crossing     chars like '╄'

    UI_try(Renderer_drawLineX(renderer, topChar, area.x+1, area.y, area.w-2),_1,;)
    // bottom
    UI_try(Renderer_drawLineX(renderer, bottomChar, area.x+1, area.y+area.h-1, area.w-2),_2,;)
    // left
    UI_try(Renderer_drawLineY(renderer, leftChar, area.x, area.y+1, area.h-2),_3,;)
    // right
    UI_try(Renderer_drawLineY(renderer, rightChar, area.x+area.w-1, area.y+1, area.h-2),_4,;)

    // corners
    termchar ltCornerChar = UIBorder_char_lt[border.left ][border.top   ];
    termchar rtCornerChar = UIBorder_char_rt[border.right][border.top   ];
    termchar rbCornerChar = UIBorder_char_rb[border.right][border.bottom];
    termchar lbCornerChar = UIBorder_char_lb[border.left ][border.bottom];
    // left top corner
    UI_try(Renderer_set(renderer, ltCornerChar, area.x, area.y),_5,;);
    // right top corner
    UI_try(Renderer_set(renderer, rtCornerChar, area.x+area.w-1, area.y),_6,;);
    // right bottom corner
    UI_try(Renderer_set(renderer, rbCornerChar, area.x+area.w-1, area.y+area.h-1),_7,;);
    // left bottom corner
    UI_try(Renderer_set(renderer, lbCornerChar, area.x, area.y+area.h-1),_8,;);

    return MaybeNull;
}
