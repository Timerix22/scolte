#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../../kerep/src/base/base.h"

void term_moveCursor(u16 row, u16 column);
void term_clear();

STRUCT(TerminalSize,
    u16 rows;
    u16 cols;
)

///@return TRUE if have got terminal size, otherwise FALSE
bool term_getSize(TerminalSize* out) WARN_UNUSED_REZULT;

/// can be used if term_getSize() fails
extern TerminalSize term_default_size;

#if __cplusplus
}
#endif
