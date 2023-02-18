#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../kerep/src/base/base.h"

void terminal_moveCursor(u16 row, u16 column);
void terminal_clear();

STRUCT(TerminalSize,
    u16 rows;
    u16 cols;
)

TerminalSize terminal_getSize();

#if __cplusplus
}
#endif
