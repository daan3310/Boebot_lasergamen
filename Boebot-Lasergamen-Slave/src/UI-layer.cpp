#include "UI-layer.h"

unsigned long CurrentMillis = 0;

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


// Function for keeping up the millis timer and checking if you're allowed to send a Status again
// 
void UI_layer_Shoot()
{
    CurrentMillis = millis();
    UPDATECURRENTTIME(CurrentMillis);
    unsigned long Millis_Per_Timer = CurrentMillis + TIMERSTATUS;
    unsigned long Millis_Per_Shoot = CurrentMillis + TIMERSHOOT; // reloading time

    byte status =0;
    // eerst een functie om te schieten
    // daarna een functie om de huidige tijd te checken
    // if1 als 100ms nieuwe status van willem
    // if2 als 1s nieuwe status naar willem als de status al terug is gelezen 

    Logiclayer_SPI_CMD_NO_DATA(SHOOT);
    while (1)
    {

    UPDATECURRENTTIME(CurrentMillis);

    if (CurrentMillis >= Millis_Per_Timer)
    {
        Serial.println("Getting current status");
        Millis_Per_Timer = CurrentMillis + TIMERSTATUS;
    }
    
    if (CurrentMillis >= Millis_Per_Shoot && status != ERROR)
    {
        Serial.println("Reloaded");
        Millis_Per_Shoot = CurrentMillis + TIMERSHOOT;
    }
    else if (CurrentMillis >= Millis_Per_Shoot)
    {
        Serial.println("Error: Geen reactie van CAM");
        Millis_Per_Shoot = CurrentMillis + TIMERSHOOT;
    }

    }
}