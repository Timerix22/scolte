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

UI_Maybe Grid_onBuildUIContext(UIElement* _grid, UIContext* context) {
    Grid* grid=(Grid*)_grid;
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
    free(self->column_widths);
    free(self->row_heights);
    if(self->content_names){
        for(u16 r = 0; r < self->rows; r++){
            for(u16 c = 0; c < self->columns; c++){
                tryLast(Grid_get(self, c, r), m_el, 
                    free(self->content);
                    free(self->content_names));
                UIElement_Ptr el=m_el.value.VoidPtr;
                UIElement_destroy(el);
            }
        }
        free(self->content_names);
    }
    else free(self->content);
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
    u16 columns=0, rows=0;
    Autoarr(i16)* column_widths=Autoarr_create(i16, 16, 64);
    Autoarr(i16)* row_heights=Autoarr_create(i16, 16, 64);
    Autoarr(Pointer)* content_names=Autoarr_create(Pointer, 64, 16);

    UI_try(UIElement_deserializeBase(dtsod, &gr.base), _91875, ;);

    Autoarr(Unitype)* _column_widths;
    Dtsod_tryGet_Autoarr(dtsod, "column_widths", _column_widths, true,
        Autoarr_foreach(_column_widths, _cw, 
            ScalingSize cw;
            UI_try(ScalingSize_fromUnitype(_cw, &cw), _18751, 
                Autoarr_freeWithoutMembers(content_names, true);
                Autoarr_freeWithoutMembers(column_widths, true);
                Autoarr_freeWithoutMembers(row_heights, true);
            );
            Autoarr_add(column_widths, cw);
        );
    );
    columns=Autoarr_length(column_widths);

    Autoarr(Unitype)* _row_heights;
    Dtsod_tryGet_Autoarr(dtsod, "row_heights", _row_heights, true,
        Autoarr_foreach(_row_heights, _rh, 
            ScalingSize rh;
            UI_try(ScalingSize_fromUnitype(_rh, &rh), _18751, 
                Autoarr_freeWithoutMembers(content_names, true);
                Autoarr_freeWithoutMembers(row_heights, true);
                Autoarr_freeWithoutMembers(row_heights, true);
            );
            Autoarr_add(row_heights, rh);
        );
    );
    rows=Autoarr_length(row_heights);

    Autoarr(Unitype)* _content;
    Dtsod_tryGet_Autoarr(dtsod, "content", _content, true);
    if(rows != Autoarr_length(_content))
        UI_safethrow(UIError_InvalidGrid,
            Autoarr_freeWithoutMembers(content_names, true);
            Autoarr_freeWithoutMembers(column_widths, true);
            Autoarr_freeWithoutMembers(row_heights, true);
        );
    
    Autoarr_foreach(_content, _row,
        if(!UniCheckTypePtr(_row, Autoarr(Unitype)))
            UI_safethrow_msg(
                cptr_concat("expected type Autoarr(Unitype), but has got type id '",
                    toString_i64(_row.typeId), "'"),
                    Autoarr_freeWithoutMembers(content_names, true);
                    Autoarr_freeWithoutMembers(column_widths, true);
                    Autoarr_freeWithoutMembers(row_heights, true);
            );
        
        Autoarr(Unitype)* row=_row.VoidPtr;
        if(columns != Autoarr_length(row))
            UI_safethrow(UIError_InvalidGrid, 
                Autoarr_freeWithoutMembers(content_names, true);
                Autoarr_freeWithoutMembers(column_widths, true);
                Autoarr_freeWithoutMembers(row_heights, true);
            );
        
        Autoarr_foreach(row, _elem_d, 
            if(!UniCheckTypePtr(_elem_d, char))
                UI_safethrow_msg(
                    cptr_concat("expected type char*, but has got type id '",
                        toString_i64(_elem_d.typeId),"'"),
                    Autoarr_freeWithoutMembers(content_names, true);
                    Autoarr_freeWithoutMembers(column_widths, true);
                    Autoarr_freeWithoutMembers(row_heights, true);
                );
            
            char* elem_name=_elem_d.VoidPtr;
            Autoarr_add(content_names, elem_name);
        )
    );
    
    gr.content_names=(char**)Autoarr_toArray(content_names);
    Autoarr_freeWithoutMembers(content_names, true);
    gr.columns=columns;
    gr.rows=rows;
    gr.column_widths=Autoarr_toArray(column_widths);
    Autoarr_freeWithoutMembers(column_widths, true);
    gr.row_heights=Autoarr_toArray(row_heights);
    Autoarr_freeWithoutMembers(row_heights, true);
    gr.content=NULL;

    Grid* ptr=malloc(sizeof(*ptr));
    *ptr=gr;
    return SUCCESS(UniHeapPtr(Grid, ptr));
}

char* Grid_toString(void* _gr, kp_fmt fmt){
    Grid* gr=_gr;
    StringBuilder* sb=StringBuilder_create();
    adds("Grid {")
    UIElement_appendToStringBuilder(sb, &gr->base);
    adduf(gr,rows)
    adduf(gr,columns)
    adds(" row_heights: [ ")
    for(u16 r=0; r < gr->rows; r++){
        addi(gr->row_heights[r]) addc(' ')
    }
    adds("]"
         " column_widths: [ ")
    for(u16 c=0; c < gr->rows; c++){
        addi(gr->row_heights[c]) addc(' ')
    }
    u32 count=gr->rows*gr->columns;
    adds("]"
        " content: [")
    if(gr->content)
        for(u32 i=0; i<count; i++){
            UIElement_Ptr child=gr->content[i];
            addc(' ')adds(child->name)addc(':')adds(child->type->kt->name)
        }
    else if(gr->content_names)
        for(u32 i=0; i<count; i++){
            char* child_name=gr->content_names[i];
            addc(' ')adds(child_name)
        }
    adds(" ]"
         " }")
    return StringBuilder_build(sb).ptr;
}

uit_define(Grid, Grid_freeMembers, Grid_toString, Grid_draw, Grid_deserialize, Grid_onBuildUIContext);

Grid* Grid_create(char* name,
        u16 columns, u16 rows, 
        ScalingSize* column_widths, ScalingSize* row_heights,
        UIElement_Ptr* content)
{
    Grid* grid=malloc(sizeof(Grid));
    *grid=(Grid){
        .base=_UIElement_initBaseDefault(name, &UITDescriptor_Grid),
        .columns=columns,
        .rows=rows,
        .column_widths=column_widths,
        .row_heights=row_heights,
        .content=content,
        .content_names=NULL
    };
    return grid;
}

