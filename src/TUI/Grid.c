#include "tui_internal.h"

static inline UIElement_Ptr* _Grid_getPtr(Grid* grid, u16 column, u16 row){
    if(row >= grid->rows)
        throw(ERR_WRONGINDEX);
    if(column >= grid->columns)
        throw(ERR_WRONGINDEX);
    return grid->content + grid->columns*row + column;
}

UIElement_Ptr Grid_get(Grid* grid, u16 column, u16 row){
    return *_Grid_getPtr(grid, column, row);
}

void Grid_set(Grid* grid, u16 column, u16 row, UIElement_Ptr value){
    *_Grid_getPtr(grid, column, row)=value;
}

void Grid_freeMembers(void* _self){
    Grid* self=(Grid*)_self;
    Grid_foreach(self, el,
        // if(el==NULL) 
        //     throw(ERR_NULLPTR);
        UIElement_destroy(el);
    );
    free(self->content);
}

UI_Maybe Grid_draw(Renderer* renderer, UIElement_Ptr _self, const DrawingArea area){
    Grid* self=(Grid*)_self;
    Grid_foreach(self, el,
        if(el==NULL)
            UI_safethrow(UIError_NullPtr,;);
        UI_try(UIElement_draw(renderer, el, area),_,;);
    );
    return MaybeNull;
}

UI_Maybe Grid_deserialize(Dtsod* dtsod){
    Grid gr;
    Autoarr(UIElement_Ptr)* content=Autoarr_create(UIElement_Ptr, 64, 8);
    u16 columns=0, rows=0;

    UI_try(UIElement_deserializeBase(dtsod, &gr.base), _91875, ;);
    /*Autoarr(Unitype)* _content;
    Dtsod_tryGet_Autoarr(dtsod, "content", _content, true, {
        Autoarr_foreach(_content, _row,
            Autoarr(Unitype)* row=_row.VoidPtr;
            Autoarr_foreach(row, _elem_d, 
                if(!UniCheckTypePtr(_elem_d,Hashtable))
                    UI_safethrow_msg(
                        cptr_concat("expected type 'Hashtable', but have got type id '",
                            toString_i64(_elem_d.typeId),"'"),
                        Autoarr_freeWithoutMembers(content, true));
                Dtsod* elem_dtsod=_elem_d.VoidPtr;
                UI_try(UIElement_deserialize(elem_dtsod), _m_uie, ;);
                Autoarr_add(content, (UIElement_Ptr)_m_uie.value.VoidPtr);
            )
        );
    });*/
    gr.content=Autoarr_toArray(content);
    Autoarr_freeWithoutMembers(content, true);
    gr.columns=columns;
    gr.rows=rows;

    Grid* ptr=malloc(sizeof(*ptr));
    *ptr=gr;
    return SUCCESS(UniHeapPtr(Grid, ptr));
}


uit_define(Grid, Grid_freeMembers, NULL, Grid_draw, Grid_deserialize);

Grid* Grid_create(char* name, u16 columns, u16 rows, UIElement_Ptr* content){
    Grid* grid=malloc(sizeof(Grid));
    *grid=(Grid){
        .base=_UIElement_initBaseDefault(name, &UITDescriptor_Grid),
        .columns=columns,
        .rows=rows,
        .content=content
    };
    return grid;
}

