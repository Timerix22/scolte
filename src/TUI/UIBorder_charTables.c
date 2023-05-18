#include "tui_internal.h"

termchar UIBorder_char_h[4]={
    [UIBorder_Hidden]=TERMCHAR(' '),
    [UIBorder_Thin  ]=TERMCHAR('─'),
    [UIBorder_Thick ]=TERMCHAR('━'),
    [UIBorder_Double]=TERMCHAR('═')
};

termchar UIBorder_char_v[4]={
    [UIBorder_Hidden]=TERMCHAR(' '),
    [UIBorder_Thin  ]=TERMCHAR('│'),
    [UIBorder_Thick ]=TERMCHAR('┃'),
    [UIBorder_Double]=TERMCHAR('║')
};

/// [left_border][top_border]
termchar UIBorder_char_lt[4][4]={
    [UIBorder_Hidden][UIBorder_Hidden]=TERMCHAR(' '),
    [UIBorder_Hidden][UIBorder_Thin  ]=TERMCHAR('─'),
    [UIBorder_Hidden][UIBorder_Thick ]=TERMCHAR('━'),
    [UIBorder_Hidden][UIBorder_Double]=TERMCHAR('═'),

    [UIBorder_Thin  ][UIBorder_Hidden]=TERMCHAR('│'),
    [UIBorder_Thin  ][UIBorder_Thin  ]=TERMCHAR('┌'),
    [UIBorder_Thin  ][UIBorder_Thick ]=TERMCHAR('┍'),
    [UIBorder_Thin  ][UIBorder_Double]=TERMCHAR('╒'),

    [UIBorder_Thick ][UIBorder_Hidden]=TERMCHAR('┃'),
    [UIBorder_Thick ][UIBorder_Thin  ]=TERMCHAR('┎'),
    [UIBorder_Thick ][UIBorder_Thick ]=TERMCHAR('┏'),
    [UIBorder_Thick ][UIBorder_Double]=TERMCHAR('╒'),

    [UIBorder_Double][UIBorder_Hidden]=TERMCHAR('║'),
    [UIBorder_Double][UIBorder_Thin  ]=TERMCHAR('╓'),
    [UIBorder_Double][UIBorder_Thick ]=TERMCHAR('╓'),
    [UIBorder_Double][UIBorder_Double]=TERMCHAR('╔')
};


/// [right_border][top_border]
termchar UIBorder_char_rt[4][4]={
    [UIBorder_Hidden][UIBorder_Hidden]=TERMCHAR(' '),
    [UIBorder_Hidden][UIBorder_Thin  ]=TERMCHAR('─'),
    [UIBorder_Hidden][UIBorder_Thick ]=TERMCHAR('━'),
    [UIBorder_Hidden][UIBorder_Double]=TERMCHAR('═'),

    [UIBorder_Thin  ][UIBorder_Hidden]=TERMCHAR('│'),
    [UIBorder_Thin  ][UIBorder_Thin  ]=TERMCHAR('┐'),
    [UIBorder_Thin  ][UIBorder_Thick ]=TERMCHAR('┑'),
    [UIBorder_Thin  ][UIBorder_Double]=TERMCHAR('╕'),

    [UIBorder_Thick ][UIBorder_Hidden]=TERMCHAR('┃'),
    [UIBorder_Thick ][UIBorder_Thin  ]=TERMCHAR('┒'),
    [UIBorder_Thick ][UIBorder_Thick ]=TERMCHAR('┓'),
    [UIBorder_Thick ][UIBorder_Double]=TERMCHAR('╕'),

    [UIBorder_Double][UIBorder_Hidden]=TERMCHAR('║'),
    [UIBorder_Double][UIBorder_Thin  ]=TERMCHAR('╖'),
    [UIBorder_Double][UIBorder_Thick ]=TERMCHAR('╖'),
    [UIBorder_Double][UIBorder_Double]=TERMCHAR('╗')
};

/// [right_border][bottom_border]
termchar UIBorder_char_rb[4][4]={
    [UIBorder_Hidden][UIBorder_Hidden]=TERMCHAR(' '),
    [UIBorder_Hidden][UIBorder_Thin  ]=TERMCHAR('─'),
    [UIBorder_Hidden][UIBorder_Thick ]=TERMCHAR('━'),
    [UIBorder_Hidden][UIBorder_Double]=TERMCHAR('═'),

    [UIBorder_Thin  ][UIBorder_Hidden]=TERMCHAR('│'),
    [UIBorder_Thin  ][UIBorder_Thin  ]=TERMCHAR('┘'),
    [UIBorder_Thin  ][UIBorder_Thick ]=TERMCHAR('┙'),
    [UIBorder_Thin  ][UIBorder_Double]=TERMCHAR('╛'),

    [UIBorder_Thick ][UIBorder_Hidden]=TERMCHAR('┃'),
    [UIBorder_Thick ][UIBorder_Thin  ]=TERMCHAR('┚'),
    [UIBorder_Thick ][UIBorder_Thick ]=TERMCHAR('┛'),
    [UIBorder_Thick ][UIBorder_Double]=TERMCHAR('╛'),

    [UIBorder_Double][UIBorder_Hidden]=TERMCHAR('║'),
    [UIBorder_Double][UIBorder_Thin  ]=TERMCHAR('╜'),
    [UIBorder_Double][UIBorder_Thick ]=TERMCHAR('╜'),
    [UIBorder_Double][UIBorder_Double]=TERMCHAR('╝')
};

/// [left_border][bottom_border]
termchar UIBorder_char_lb[4][4]={
    [UIBorder_Hidden][UIBorder_Hidden]=TERMCHAR(' '),
    [UIBorder_Hidden][UIBorder_Thin  ]=TERMCHAR('─'),
    [UIBorder_Hidden][UIBorder_Thick ]=TERMCHAR('━'),
    [UIBorder_Hidden][UIBorder_Double]=TERMCHAR('═'),

    [UIBorder_Thin  ][UIBorder_Hidden]=TERMCHAR('│'),
    [UIBorder_Thin  ][UIBorder_Thin  ]=TERMCHAR('└'),
    [UIBorder_Thin  ][UIBorder_Thick ]=TERMCHAR('┕'),
    [UIBorder_Thin  ][UIBorder_Double]=TERMCHAR('╘'),

    [UIBorder_Thick ][UIBorder_Hidden]=TERMCHAR('┃'),
    [UIBorder_Thick ][UIBorder_Thin  ]=TERMCHAR('┖'),
    [UIBorder_Thick ][UIBorder_Thick ]=TERMCHAR('┗'),
    [UIBorder_Thick ][UIBorder_Double]=TERMCHAR('╘'),

    [UIBorder_Double][UIBorder_Hidden]=TERMCHAR('║'),
    [UIBorder_Double][UIBorder_Thin  ]=TERMCHAR('╙'),
    [UIBorder_Double][UIBorder_Thick ]=TERMCHAR('╙'),
    [UIBorder_Double][UIBorder_Double]=TERMCHAR('╚')
};
