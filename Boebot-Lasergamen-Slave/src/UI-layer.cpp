#include "UI-layer.h"

byte UI_layer_error_handling(byte error)
{   
    Serial.println();

    switch (error)
    {
        case CONTROLLERNOTDETECTED:
        {
            
            Serial.println("Error: Controller not detected in the correct time frame, try charging the controller");
            return 0;
        }
        case ESPSLAVENOTDETECTED:
        {
            Serial.println("Error: ESPCAM not detected, look if its connected to power else try resetting the ESP");
            return 0;
        }
        case NOCMD:
        {
            Serial.println("Error: Internal error in esp32 cmd not recognised");
            return 0;
        }
        case CONTROLLERLOWPOWER:
        {
            Serial.println("Warning: Controller low battery, put the controller on the charger");
            return 0;
        }
        case HOSTNOTDETECTED:
        {
            Serial.println("Error: No Raspberry pi found on local network");
            return 0;
        }
        case NOINTERNET:
        {
            Serial.println("Error: No connection with the internet made");
            return 0;
        }
        default:
        {
            Serial.println("Error: Error not recognised");
            return 0;
        }
    }

    return 0;
}