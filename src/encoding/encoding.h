#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../../kerep/src/base/base.h"

typedef u8 utf8char;
typedef u32 utf32char;

/// Writes utf32 char to file
///@returns >0 length of utf8 multibyte character representation.  
///@returns 0  utf8proc_encode_char returns error  
///@returns <0 error code of fputc
int utf32char_fwrite(FILE* file, utf32char ch);

typedef utf32char termchar;
#define TERMCHAR(CHAR) U##CHAR
#define TERMSTR(STR) U##STR
#define termchar_print(CH) utf32char_fwrite(stdout, CH)

#if __cplusplus
}
#endif
