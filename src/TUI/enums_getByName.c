#include "tui_internal.h"

#define enum_pair(name, value) { cptr_copy(name), UniUInt64(value) }

Hashtable* _colors_table=NULL;
void termcolor_table_init(){
    _colors_table=Hashtable_create();
    KVPair pairs[]={
        enum_pair("black", kp_bgBlack|kp_fgBlack),
        enum_pair("white", kp_bgWhite|kp_fgWhite),
        enum_pair("gray", kp_bgGray|kp_fgGray),
        enum_pair("red", kp_bgRed|kp_fgRed),
        enum_pair("green", kp_bgGreen|kp_fgGreen),
        enum_pair("yellow", kp_bgYellow|kp_fgYellow),
        enum_pair("blue", kp_bgBlue|kp_fgBlue),
        enum_pair("magenta", kp_bgMagenta|kp_fgMagenta),
        enum_pair("cyan", kp_bgCyan|kp_fgCyan),

        enum_pair("gray_dark", kp_bgGrayD|kp_fgGrayD),
        enum_pair("red_dark", kp_bgRedD|kp_fgRedD),
        enum_pair("green_dark", kp_bgGreenD|kp_fgGreenD),
        enum_pair("yellow_dark", kp_bgYellowD|kp_fgYellowD),
        enum_pair("blue_dark", kp_bgBlueD|kp_fgBlueD),
        enum_pair("magenta_dark", kp_bgMagentaD|kp_fgMagentaD),
        enum_pair("cyan_dark", kp_bgCyanD|kp_fgCyanD),

        enum_pair("dark_gray", kp_bgGrayD|kp_fgGrayD),
        enum_pair("dark_red", kp_bgRedD|kp_fgRedD),
        enum_pair("dark_green", kp_bgGreenD|kp_fgGreenD),
        enum_pair("dark_yellow", kp_bgYellowD|kp_fgYellowD),
        enum_pair("dark_blue", kp_bgBlueD|kp_fgBlueD),
        enum_pair("dark_magenta", kp_bgMagentaD|kp_fgMagentaD),
        enum_pair("dark_cyan", kp_bgCyanD|kp_fgCyanD),

        enum_pair("gray_d", kp_bgGrayD|kp_fgGrayD),
        enum_pair("red_d", kp_bgRedD|kp_fgRedD),
        enum_pair("green_d", kp_bgGreenD|kp_fgGreenD),
        enum_pair("yellow_d", kp_bgYellowD|kp_fgYellowD),
        enum_pair("blue_d", kp_bgBlueD|kp_fgBlueD),
        enum_pair("magenta_d", kp_bgMagentaD|kp_fgMagentaD),
        enum_pair("cyan_d", kp_bgCyanD|kp_fgCyanD),

        enum_pair("d_gray", kp_bgGrayD|kp_fgGrayD),
        enum_pair("d_red", kp_bgRedD|kp_fgRedD),
        enum_pair("d_green", kp_bgGreenD|kp_fgGreenD),
        enum_pair("d_yellow", kp_bgYellowD|kp_fgYellowD),
        enum_pair("d_blue", kp_bgBlueD|kp_fgBlueD),
        enum_pair("d_magenta", kp_bgMagentaD|kp_fgMagentaD),
        enum_pair("d_cyan", kp_bgCyanD|kp_fgCyanD),
        
        enum_pair("gray_d", kp_bgGrayD|kp_fgGrayD),
        enum_pair("red_d", kp_bgRedD|kp_fgRedD),
        enum_pair("green_d", kp_bgGreenD|kp_fgGreenD),
        enum_pair("yellow_d", kp_bgYellowD|kp_fgYellowD),
        enum_pair("blue_d", kp_bgBlueD|kp_fgBlueD),
        enum_pair("magenta_d", kp_bgMagentaD|kp_fgMagentaD),
        enum_pair("cyan_d", kp_bgCyanD|kp_fgCyanD),

        enum_pair("dgray", kp_bgGrayD|kp_fgGrayD),
        enum_pair("dred", kp_bgRedD|kp_fgRedD),
        enum_pair("dgreen", kp_bgGreenD|kp_fgGreenD),
        enum_pair("dyellow", kp_bgYellowD|kp_fgYellowD),
        enum_pair("dblue", kp_bgBlueD|kp_fgBlueD),
        enum_pair("dmagenta", kp_bgMagentaD|kp_fgMagentaD),
        enum_pair("dcyan", kp_bgCyanD|kp_fgCyanD),
    };
    Hashtable_addMany(_colors_table, pairs, sizeof(pairs)/sizeof(KVPair));
}

Hashtable* _border_t_table=NULL;

void UIBorderThickness_table_init(){
    _border_t_table=Hashtable_create();
    KVPair pairs[]={
        enum_pair("hidden", UIBorder_Hidden),
        enum_pair("0", UIBorder_Hidden),
        enum_pair("thin", UIBorder_Thin),
        enum_pair("0.5", UIBorder_Thin),
        enum_pair("thick", UIBorder_Thick),
        enum_pair("1", UIBorder_Thick),
        enum_pair("double", UIBorder_Double),
        enum_pair("2", UIBorder_Double),
        enum_pair("noborder", UiBorder_NoBorder),
        enum_pair("no", UiBorder_NoBorder)
    };
    Hashtable_addMany(_border_t_table, pairs, sizeof(pairs)/sizeof(KVPair));
}

void UI_enum_tables_init(){
    termcolor_table_init();
    UIBorderThickness_table_init();
}

void UI_enum_tables_free(){
    Hashtable_free(_colors_table);
    Hashtable_free(_border_t_table);
}

int enum_getByName(Hashtable* table, char* name){
    char* name_lower=cptr_toLower(name);
    Unitype uni;
    if(!Hashtable_tryGet(table, name_lower, &uni))
        return -1;
    free(name_lower);
    return uni.UInt64;
}

int termcolor_getByName(char* name){ return enum_getByName(_colors_table, name); }
int UIBorderThickness_getByName(char* name){ return enum_getByName(_border_t_table, name); }
