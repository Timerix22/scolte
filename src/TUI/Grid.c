#include "tui_internal.h"

static inline UIElement_Ptr* _Grid_getPtr(Grid* grid, u16 column, u16 row){
    if(row >= grid->rows)
        throw(ERR_WRONGINDEX);
    if(column >= grid->columns)
        throw(ERR_WRONGINDEX);
    return grid->ui_elements + grid->columns*row + column;
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
        if(el==NULL) // todo UI_throw
            throw(ERR_NULLPTR);
        UIElement_destroy(el);
    );
    free(self->ui_elements);
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
    Unitype uni;
    Autoarr(UIElement_Ptr)* content=Autoarr_create(UIElement_Ptr, 64, 8);

    UI_try(UIElement_deserializeBase(dtsod, &gr.base), _91875, ;);
    Dtsod_get_necessary(dtsod, "content"){
        Autoarr(Unitype)* _content=uni.VoidPtr;
        Autoarr_foreach(_content, _row,
            Autoarr(Unitype)* row=_row.VoidPtr;
            Autoarr_foreach(row, _elem_d, 
                Dtsod* elem_dtsod=_elem_d.VoidPtr;
                UI_try(UIElement_deserialize(elem_dtsod), _m_uie, ;);
                Autoarr_add(content, (UIElement_Ptr)_m_uie.value.VoidPtr);
            )
        );
    }

    Grid* ptr=malloc(sizeof(*ptr));
    *ptr=gr;
    return SUCCESS(UniHeapPtr(TextBlock, ptr));
}


uit_define(Grid, Grid_freeMembers, NULL, Grid_draw, Grid_deserialize);

Grid* Grid_create(char* name, u16 columns, u16 rows, UIElement_Ptr* ui_elements){
    Grid* grid=malloc(sizeof(Grid));
    *grid=(Grid){
        .base=_UIElement_initBaseDefault(name, &UITDescriptor_Grid),
        .columns=columns,
        .rows=rows,
        .ui_elements=ui_elements
    };
    return grid;
}

