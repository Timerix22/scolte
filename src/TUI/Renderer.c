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
void FrameBuffer_recreate(FrameBuffer* fb){
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
        fb->data[i]=TCI(TERMCHAR(' '), kp_bgGray|kp_fgGray);
}

void Renderer_freeMembers(void * _self){
    Renderer* self=(Renderer*)_self;
    FrameBuffer_freeMembers(&self->frameBuffer);
}

//////////////////////////////////////
//             Renderer             //
//////////////////////////////////////

UI_Maybe __Renderer_set(Renderer* self, TermCharInfo tci, u16 x, u16 y) {
    if(x >= self->frameBuffer.size.cols)
        UI_safethrow(UIError_InvalidX,;);
    if(y >= self->frameBuffer.size.rows)
        UI_safethrow(UIError_InvalidY,;);
    self->frameBuffer.data[self->frameBuffer.size.cols * y + x] = tci;
    return MaybeNull;
}

#include <errno.h>

UI_Maybe __Renderer_drawFrame(Renderer* self){
    // save current buffer
    FrameBuffer buf=self->frameBuffer;
    // recreate frame buffer
    FrameBuffer_recreate(&self->frameBuffer);

    for(u16 y=0; y<buf.size.rows; y++){
        for(u16 x=0; x<buf.size.cols; x++){
            TermCharInfo tci=buf.data[buf.size.cols*y + x];
            int rez=TCI_print(tci);
            if(rez<0){
                char* chex=toString_hex(&tci.ch, sizeof(tci.ch), true, true, true);
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
    FrameBuffer_recreate(&renderer->frameBuffer);
    renderer->set=__Renderer_set;
    renderer->drawFrame=__Renderer_drawFrame;
    return renderer;
}

//////////////////////////////////////
//        drawing functions         //
//////////////////////////////////////

UI_Maybe Renderer_fill(Renderer* renderer, TermCharInfo tci, const DrawingArea area){
    UI_try(DrawingArea_validate(area),_,;);
    for(u16 y=area.y; y<area.y+area.h; y++)
        for(u16 x=area.x; x<area.x+area.w; x++){
            UI_try(Renderer_set(renderer, tci, x, y),_,;);
        }
    return MaybeNull;
}

UI_Maybe Renderer_drawLineX(Renderer* renderer, TermCharInfo tci, u16 x, u16 y, u16 length){
    while(length>0){
        UI_try(Renderer_set(renderer, tci, x, y),_,;);
        x++; length--;
    }
    return MaybeNull;
}

UI_Maybe Renderer_drawLineY(Renderer* renderer, TermCharInfo tci, u16 x, u16 y, u16 length){
    while(length>0){
        UI_try(Renderer_set(renderer, tci, x, y),_,;);
        y++; length--;
    }
    return MaybeNull;
}

UI_Maybe Renderer_drawBorder(Renderer* renderer, UIBorder border, const DrawingArea area){
    UI_try(DrawingArea_validate(area),_0,;);

    //lines
    termchar topChar    = UIBorder_char_h[border.top   ];
    termchar bottomChar = UIBorder_char_h[border.bottom];
    termchar leftChar   = UIBorder_char_v[border.left  ];
    termchar rightChar  = UIBorder_char_v[border.right ];
    // top
    
    // TODO check neighbor borders and insert crossing     chars like '╄'

    UI_try(Renderer_drawLineX(renderer, TCI(topChar, border.color), area.x+1, area.y, area.w-2),_1,;)
    // bottom
    UI_try(Renderer_drawLineX(renderer, TCI(bottomChar, border.color), area.x+1, area.y+area.h-1, area.w-2),_2,;)
    // left
    UI_try(Renderer_drawLineY(renderer, TCI(leftChar, border.color), area.x, area.y+1, area.h-2),_3,;)
    // right
    UI_try(Renderer_drawLineY(renderer, TCI(rightChar, border.color), area.x+area.w-1, area.y+1, area.h-2),_4,;)

    // corners
    termchar ltCornerChar = UIBorder_char_lt[border.left ][border.top   ];
    termchar rtCornerChar = UIBorder_char_rt[border.right][border.top   ];
    termchar rbCornerChar = UIBorder_char_rb[border.right][border.bottom];
    termchar lbCornerChar = UIBorder_char_lb[border.left ][border.bottom];
    // left top corner
    UI_try(Renderer_set(renderer, TCI(ltCornerChar, border.color), area.x, area.y),_5,;);
    // right top corner
    UI_try(Renderer_set(renderer, TCI(rtCornerChar, border.color), area.x+area.w-1, area.y),_6,;);
    // right bottom corner
    UI_try(Renderer_set(renderer, TCI(rbCornerChar, border.color), area.x+area.w-1, area.y+area.h-1),_7,;);
    // left bottom corner
    UI_try(Renderer_set(renderer, TCI(lbCornerChar, border.color), area.x, area.y+area.h-1),_8,;);

    return MaybeNull;
}
