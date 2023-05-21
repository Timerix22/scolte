#pragma once

#include "tui_dtsod.h"


STRUCT(UIDtsodFileModel,
    char* file_name;
    char* dtsod_text;
    Dtsod* dtsod;
)
Autoarr_declare(UIDtsodFileModel)

STRUCT(UIDtsodParser,
    Autoarr(UIDtsodFileModel)* file_models;
    UIContext* context;
    bool returned_context;
)

STRUCT(UIContext,
    Hashtable* ui_elements;
)