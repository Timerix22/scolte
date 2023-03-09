#pragma once

#if __cplusplus
extern "C" {
#endif

// #include <wchar.h>
#include <uchar.h>

typedef char16_t utfchar;
#define UTFCHAR(CHAR) u##CHAR
#define UTFSTR(STR) u##STR

// returns length of written bytes or error code 
static inline size_t utfchar_write(utfchar ch, FILE* file){
    char multibyteUtf8[8]={0};
    mbstate_t mbs={0};
    size_t length=c16rtomb(multibyteUtf8, ch, &mbs);
    if(length!=-1)
        for(u8 i=0; i<length; i++){
            int rez=fputc(multibyteUtf8[i], file);
            if(rez<0)
                return rez;
        }
    return length;
}
#define utfchar_print(CH) utfchar_write(CH, stdout)

#if __cplusplus
}
#endif
