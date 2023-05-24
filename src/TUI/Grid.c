#include "tui_internal.h"

#define grid_validate() \
    if(row >= grid->rows) UI_safethrow(UIError_InvalidY,;); \
    if(column >= grid->columns) UI_safethrow(UIError_InvalidX,;);

UI_Maybe Grid_set(Grid* grid, u16 column, u16 row, UIElement_Ptr value){
    grid_validate();
    UIElement_Ptr* ptr_ptr = grid->content + grid->columns*row + column;
    *ptr_ptr=value;
    return MaybeNull;
}

UI_Maybe Grid_get(Grid* grid, u16 column, u16 row){
    grid_validate();
    UIElement_Ptr* ptr_ptr = grid->content + grid->columns*row + column;
    return SUCCESS(UniHeapPtr(UIElement, *ptr_ptr));
}

UI_Maybe _Grid_getName(Grid* grid, u16 column, u16 row) {
    grid_validate();
    char** str_ptr = grid->content_names + grid->columns*row + column;
    return SUCCESS(UniHeapPtr(char, *str_ptr));
}

UI_Maybe _Grid_bindContent(Grid* grid, UIContext* context) {
    Autoarr(UIElement_Ptr)* content=Autoarr_create(UIElement_Ptr, 64, 16);
    Grid_foreachName(grid, name,
        UI_try(UIContext_getAny(context, name), m_uie, Autoarr_freeWithoutMembers(content,true));
        UIElement_Ptr uie=m_uie.value.VoidPtr;
        Autoarr_add(content, uie);
    );
    free(grid->content_names);
    grid->content_names=NULL;
    grid->content=Autoarr_toArray(content);
    Autoarr_freeWithoutMembers(content, true);
    return MaybeNull;
}

void Grid_freeMembers(void* _self){
    Grid* self=(Grid*)_self;
    if(self->is_bound){
        for(u16 r = 0; r < self->rows; r++){
            for(u16 c = 0; c < self->columns; c++){
                tryLast(Grid_get(self, c, r), m_el, 
                    free(self->content);
                    free(self->content_names));
                UIElement_Ptr el=m_el.value.VoidPtr;
                UIElement_destroy(el);
            } 
        }
    }
    free(self->content);
    if(self->content_names)
        free(self->content_names);
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
    Autoarr(Pointer)* content_names=Autoarr_create(Pointer, 64, 16);
    u16 columns=0, rows=0;

    UI_try(UIElement_deserializeBase(dtsod, &gr.base), _91875, ;);
    Autoarr(Unitype)* _content;
    Dtsod_tryGet_Autoarr(dtsod, "content", _content, true);
    rows=Autoarr_length(_content);
    Autoarr_foreach(_content, _row,
        if(!UniCheckTypePtr(_row, Autoarr(Unitype))){
            UI_safethrow_msg(
                cptr_concat("expected Autoarr<Unitype>, but has got type id '",
                    toString_i64(_row.typeId), "'"),
                Autoarr_freeWithoutMembers(content_names, true));
        }
        Autoarr(Unitype)* row=_row.VoidPtr;
        u16 row_len=Autoarr_length(row);
        if(columns==0)
            columns=row_len;
        else if(row_len != columns)
            UI_safethrow_msg("wrong grid row size",;);
        Autoarr_foreach(row, _elem_d, 
            if(!UniCheckTypePtr(_elem_d, char)){
                UI_safethrow_msg(
                    cptr_concat("expected char*, but has got type id '",
                        toString_i64(_elem_d.typeId),"'"),
                    Autoarr_freeWithoutMembers(content_names, true));
            }
            char* elem_name=_elem_d.VoidPtr;
            Autoarr_add(content_names, elem_name);
        )
    );
    
    gr.content_names=(char**)Autoarr_toArray(content_names);
    Autoarr_freeWithoutMembers(content_names, true);
    gr.columns=columns;
    gr.rows=rows;
    gr.is_bound=false;
    gr.content=NULL;

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
        .content=content,
        .content_names=NULL,
        .is_bound=true
    };
    return grid;
}

