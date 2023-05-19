#include "term.h"
#include <stdio.h>
#include <unistd.h>
#include IFWIN("windows.h", "sys/ioctl.h")

char* TerminalSize_toString(TerminalSize t){
    char buf[64];
    sprintf_s(buf, sizeof(buf), "(%ux%u)", t.cols, t.rows);
    return cptr_copy(buf);
}

char* __TerminalSize_toString(void* _t, u32 fmt){ return TerminalSize_toString(*(TerminalSize*)_t); }

kt_define(TerminalSize, NULL, __TerminalSize_toString);

TerminalSize term_default_size={.cols=80, .rows=16};

int getenv_int(const char* var_name){
    char* str=getenv(var_name);
    if(str==NULL)
        return -1;
    return strtol(str, NULL, 0);
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
    struct winsize ws={0,0,0,0};
    // tries to get terminal size from stdin, stdout, stderr
    if (ioctl(STDIN_FILENO,  TIOCGWINSZ, &ws)==0 ||
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws)==0 ||
        ioctl(STDERR_FILENO, TIOCGWINSZ, &ws)==0 ){
        out->cols=ws.ws_col;
        out->rows=ws.ws_row;
    }
    // tries to get size from environtent variables
    else{
        out->cols=getenv_int("COLUMNS");
        out->rows=getenv_int("LINES");
    }
#endif
    return out->cols > 0 && out->cols < 720 && out->rows > 0 && out->rows < 480;
}

/*
Most of escape sequences can be found at: 
https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
*/

void term_resetCursor() { printf("\e[H"); }
void term_resetColors() { printf("\e[0m"); }
void term_clear()       { printf("\e[0m\e[H\e[2J"); }
void term_cursorMove(u16 row, u16 column) { printf("\e[%u;%uH",row,column); }
void term_cursorHide(bool hide) { printf( hide ? "\e[?25l" : "\e[?25h"); }