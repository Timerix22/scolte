#include "term.h"

kt_define(TermCharInfo, NULL, NULL);

int TCI_fwrite(FILE* file, TermCharInfo tci){
    kprint_setColor(tci.color);
    return termchar_fwrite(file, tci.ch);
}
