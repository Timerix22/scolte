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

#define Grid_foreach(GRID_PTR, ELEM_VAR_NAME, CODE...){ \
    for(u16 _g_r = 0; _g_r < GRID_PTR->rows; _g_r++){ \
        for(u16 _g_c = 0; _g_c < GRID_PTR->columns; _g_c++){ \
            UIElement_Ptr ELEM_VAR_NAME = Grid_get(GRID_PTR, _g_c, _g_r); \
            { CODE } \
        } \
    } \
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

kt_define(Grid, Grid_freeMembers, NULL);

Grid* Grid_create(u16 columns, u16 rows, UIElement_Ptr* ui_elements){
    Grid* grid=malloc(sizeof(Grid));
    *grid=(Grid){
        .base=__UIElement_createDefault(ktid_name(Grid), Grid_draw),
        .columns=columns,
        .rows=rows,
        .ui_elements=ui_elements
    };
    return grid;
}

