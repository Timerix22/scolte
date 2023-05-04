#include "encoding.h"
#include "../../utf8proc/utf8proc.h"

int utf32char_fwrite(FILE* file, utf32char ch){
    utf8char utf8buf[6];
    int length=utf8proc_encode_char(ch, utf8buf);

    for(u8 i=0; i<length; i++){
        int rez=fputc(utf8buf[i], file);
        if(rez<0)
            return rez;
    }

    return length;
}