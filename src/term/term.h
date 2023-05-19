#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../../kerep/src/base/base.h"
#include "../../kerep/src/kprint/kprint_format.h"
#include "../encoding/encoding.h"

void term_moveCursor(u16 row, u16 column);
void term_clear();

STRUCT(TerminalSize,
    u16 cols;
    u16 rows;
)

///@return TRUE if have got terminal size, otherwise FALSE
bool term_getSize(TerminalSize* out) WARN_UNUSED_REZULT;

/// can be used if term_getSize() fails
extern TerminalSize term_default_size;

typedef kp_fmt termcolor;
typedef utf32char termchar;
#define TERMCHAR(CHAR) U##CHAR
#define TERMSTR(STR) U##STR
#define termchar_fwrite utf32char_fwrite

STRUCT(TermCharInfo,
    termchar ch;
    termcolor color; /* background + foreground */
)

#define TCI(CH,COLOR)(TermCharInfo){.ch=CH, .color=COLOR}
int TCI_fwrite(FILE* file, TermCharInfo tci);
#define TCI_print(tci) TCI_fwrite(stdout, tci);

#if __cplusplus
}
#endif
