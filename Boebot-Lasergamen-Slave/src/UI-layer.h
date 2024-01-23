#ifndef UI_LAYER_H
#define UI_LAYER_H

#include "main.h"

// Enumerator for different 
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

#endif