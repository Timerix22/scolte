#include "tui_internal.h"

utfchar UIBorder_char_h[4]={
    [UIBorder_Hidden]=UTFCHAR(' '),
    [UIBorder_Thin  ]=UTFCHAR('─'),
    [UIBorder_Thick ]=UTFCHAR('━'),
    [UIBorder_Double]=UTFCHAR('═')
};

utfchar UIBorder_char_v[4]={
    [UIBorder_Hidden]=UTFCHAR(' '),
    [UIBorder_Thin  ]=UTFCHAR('│'),
    [UIBorder_Thick ]=UTFCHAR('┃'),
    [UIBorder_Double]=UTFCHAR('║')
};

/// [left_border][top_border]
utfchar UIBorder_char_lt[4][4]={
    [UIBorder_Hidden][UIBorder_Hidden]=UTFCHAR(' '),
    [UIBorder_Hidden][UIBorder_Thin  ]=UTFCHAR('─'),
    [UIBorder_Hidden][UIBorder_Thick ]=UTFCHAR('━'),
    [UIBorder_Hidden][UIBorder_Double]=UTFCHAR('═'),

    [UIBorder_Thin  ][UIBorder_Hidden]=UTFCHAR('│'),
    [UIBorder_Thin  ][UIBorder_Thin  ]=UTFCHAR('┌'),
    [UIBorder_Thin  ][UIBorder_Thick ]=UTFCHAR('┍'),
    [UIBorder_Thin  ][UIBorder_Double]=UTFCHAR('╒'),

    [UIBorder_Thick ][UIBorder_Hidden]=UTFCHAR('┃'),
    [UIBorder_Thick ][UIBorder_Thin  ]=UTFCHAR('┎'),
    [UIBorder_Thick ][UIBorder_Thick ]=UTFCHAR('┏'),
    [UIBorder_Thick ][UIBorder_Double]=UTFCHAR('╒'),

    [UIBorder_Double][UIBorder_Hidden]=UTFCHAR('║'),
    [UIBorder_Double][UIBorder_Thin  ]=UTFCHAR('╓'),
    [UIBorder_Double][UIBorder_Thick ]=UTFCHAR('╓'),
    [UIBorder_Double][UIBorder_Double]=UTFCHAR('╔')
};


/// [right_border][top_border]
utfchar UIBorder_char_rt[4][4]={
    [UIBorder_Hidden][UIBorder_Hidden]=UTFCHAR(' '),
    [UIBorder_Hidden][UIBorder_Thin  ]=UTFCHAR('─'),
    [UIBorder_Hidden][UIBorder_Thick ]=UTFCHAR('━'),
    [UIBorder_Hidden][UIBorder_Double]=UTFCHAR('═'),

    [UIBorder_Thin  ][UIBorder_Hidden]=UTFCHAR('│'),
    [UIBorder_Thin  ][UIBorder_Thin  ]=UTFCHAR('┐'),
    [UIBorder_Thin  ][UIBorder_Thick ]=UTFCHAR('┑'),
    [UIBorder_Thin  ][UIBorder_Double]=UTFCHAR('╕'),

    [UIBorder_Thick ][UIBorder_Hidden]=UTFCHAR('┃'),
    [UIBorder_Thick ][UIBorder_Thin  ]=UTFCHAR('┒'),
    [UIBorder_Thick ][UIBorder_Thick ]=UTFCHAR('┓'),
    [UIBorder_Thick ][UIBorder_Double]=UTFCHAR('╕'),

    [UIBorder_Double][UIBorder_Hidden]=UTFCHAR('║'),
    [UIBorder_Double][UIBorder_Thin  ]=UTFCHAR('╖'),
    [UIBorder_Double][UIBorder_Thick ]=UTFCHAR('╖'),
    [UIBorder_Double][UIBorder_Double]=UTFCHAR('╗')
};

/// [right_border][bottom_border]
utfchar UIBorder_char_rb[4][4]={
    [UIBorder_Hidden][UIBorder_Hidden]=UTFCHAR(' '),
    [UIBorder_Hidden][UIBorder_Thin  ]=UTFCHAR('─'),
    [UIBorder_Hidden][UIBorder_Thick ]=UTFCHAR('━'),
    [UIBorder_Hidden][UIBorder_Double]=UTFCHAR('═'),

    [UIBorder_Thin  ][UIBorder_Hidden]=UTFCHAR('│'),
    [UIBorder_Thin  ][UIBorder_Thin  ]=UTFCHAR('┘'),
    [UIBorder_Thin  ][UIBorder_Thick ]=UTFCHAR('┙'),
    [UIBorder_Thin  ][UIBorder_Double]=UTFCHAR('╛'),

    [UIBorder_Thick ][UIBorder_Hidden]=UTFCHAR('┃'),
    [UIBorder_Thick ][UIBorder_Thin  ]=UTFCHAR('┚'),
    [UIBorder_Thick ][UIBorder_Thick ]=UTFCHAR('┛'),
    [UIBorder_Thick ][UIBorder_Double]=UTFCHAR('╛'),

    [UIBorder_Double][UIBorder_Hidden]=UTFCHAR('║'),
    [UIBorder_Double][UIBorder_Thin  ]=UTFCHAR('╜'),
    [UIBorder_Double][UIBorder_Thick ]=UTFCHAR('╜'),
    [UIBorder_Double][UIBorder_Double]=UTFCHAR('╝')
};

/// [left_border][bottom_border]
utfchar UIBorder_char_lb[4][4]={
    [UIBorder_Hidden][UIBorder_Hidden]=UTFCHAR(' '),
    [UIBorder_Hidden][UIBorder_Thin  ]=UTFCHAR('─'),
    [UIBorder_Hidden][UIBorder_Thick ]=UTFCHAR('━'),
    [UIBorder_Hidden][UIBorder_Double]=UTFCHAR('═'),

    [UIBorder_Thin  ][UIBorder_Hidden]=UTFCHAR('│'),
    [UIBorder_Thin  ][UIBorder_Thin  ]=UTFCHAR('└'),
    [UIBorder_Thin  ][UIBorder_Thick ]=UTFCHAR('┕'),
    [UIBorder_Thin  ][UIBorder_Double]=UTFCHAR('╘'),

    [UIBorder_Thick ][UIBorder_Hidden]=UTFCHAR('┃'),
    [UIBorder_Thick ][UIBorder_Thin  ]=UTFCHAR('┖'),
    [UIBorder_Thick ][UIBorder_Thick ]=UTFCHAR('┗'),
    [UIBorder_Thick ][UIBorder_Double]=UTFCHAR('╘'),

    [UIBorder_Double][UIBorder_Hidden]=UTFCHAR('║'),
    [UIBorder_Double][UIBorder_Thin  ]=UTFCHAR('╙'),
    [UIBorder_Double][UIBorder_Thick ]=UTFCHAR('╙'),
    [UIBorder_Double][UIBorder_Double]=UTFCHAR('╚')
};
