#include "tui.h"

kt_define(DrawingArea, NULL, NULL);
kt_define(UIBorderParams, NULL, NULL);

void kt_initScolteTypes(){
    kt_register(DrawingArea);
    kt_register(UIBorderParams);
    kt_register(UIElement);
    kt_register(Canvas);
    kt_register(TextBlock);
}
