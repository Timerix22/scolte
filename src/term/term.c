#include "term.h"
#include <stdio.h>
#include <unistd.h>
#include IFWIN("windows.h", "sys/ioctl.h")

void term_moveCursor(u16 row, u16 column){
    printf("\e[%u;%uf",row,column);
    
}

void term_clear() {
    printf("\e[2j");
}

bool term_getSize(TerminalSize* out) {
#if defined(_WIN64) || defined(_WIN32)
    // helps when STD_OUT is redirected to a file
    HANDLE hConsoleErr = GetStdHandle(STD_ERROR_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    if(!GetConsoleScreenBufferInfo(hConsoleErr, &consoleInfo))
        return false;
    *out=(TerminalSize){
        .cols=consoleInfo.srWindow.Right-consoleInfo.srWindow.Left+1,
        .rows=consoleInfo.srWindow.Bottom-consoleInfo.srWindow.Top+1
    };
#else
    struct winsize w={0,0,0,0};
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)!=0)
        return false;
    *out=(TerminalSize){
        .cols=w.ws_col,
        .rows=w.ws_row
    };
#endif
    return true;
}

TerminalSize term_default_size={.cols=80, .rows=16};
