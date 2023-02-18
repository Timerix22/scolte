#include "terminal.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

void terminal_moveCursor(u16 row, u16 column){
    printf("\e[%u;%uf",row,column);
}

void terminal_clear() {
    printf("\e[2j");
}

TerminalSize terminal_getSize(){
    struct winsize w={0,0,0,0};
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)!=0)
        kprintf("\e[93mterminal_getSize() error\n");
    TerminalSize tsize={
        .cols=w.ws_col,
        .rows=w.ws_row
    };
    return tsize;
}
