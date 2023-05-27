#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../../kerep/src/base/base.h"

PACKED_ENUM(UIError, 
    UIError_Success=ERR_IO_EOF+1,
    UIError_NotImplemented,
    UIError_NullPtr,
    UIError_InvalidHeight, UIError_InvalidWidth,
    UIError_InvalidX, UIError_InvalidY,
    UIError_PrintError,
    UIError_InvalidVersion,
    UIError_InvalidGrid,
    UIError_InvalidFormat
)

// this macros can be changed later to improve debugging
#define UI_safethrow(ERRCODE, FREEMEM) safethrow(nameof(ERRCODE), FREEMEM)
#define UI_safethrow_msg(ERRMSG, FREEMEM) safethrow(ERRMSG, FREEMEM)
#define UI_try(CALL,RZLT,FREEMEM) try(CALL,RZLT,FREEMEM)
#define UI_Maybe Maybe
#define UI_THROWING_FUNC_DECL(NAME_AND_ARGS) UI_Maybe NAME_AND_ARGS WARN_UNUSED_REZULT

#if __cplusplus
}
#endif
