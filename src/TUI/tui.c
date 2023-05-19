#include "tui.h"

kt_define(DrawingArea, NULL, NULL);
kt_define(UIBorder, NULL, NULL);

void kt_initScolteTypes(){
    kt_register(DrawingArea);
    kt_register(UIBorder);
    kt_register(UIElement);
    kt_register(Grid);
    kt_register(TextBlock);
}
