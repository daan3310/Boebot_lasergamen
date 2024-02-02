#ifndef UI_LAYER_H
#define UI_LAYER_H

#include "main.h"

#define UPDATECURRENTTIME(a) (a = millis())

// Enumerator for different error types
enum ERROR
{
    CONTROLLERNOTDETECTED,
    ESPSLAVENOTDETECTED,
    NOCMD,
    CONTROLLERLOWPOWER,
    

    //Slave errors:
    HOSTNOTDETECTED,
    NOINTERNET,
    
};

byte UI_layer_error_handling(byte);

void UI_layer_Shoot();

#endif