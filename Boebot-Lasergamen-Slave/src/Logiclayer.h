#ifndef LOGICLAYER_H
#define LOGICLAYER_H

#include "main.h"
#include "IO-layer.h"


struct PS3 Logiclayer_Besturing_Data(struct PS3 PS3Inputs);

byte Logiclayer_Serial_CMD(byte cmd, byte data[3]);
byte Logiclayer_Serial_CMD_NO_DATA(byte CMD);

void Logiclayer_set_colour(byte Color);

byte Logiclayer_Startup_Serial(byte state);

// Definition for SPI Commands from master to slave
enum SPICMD
{
    SHUTDOWN,       // Shutdown om de espcam te resetten
    PLACEMAKER,     // Plaatsmaker betekent niks voor de espcam maar dan kan je de data in de registers uitlezen van espcam
    STARTGAME,      // Vraag aan de cam om klaar te maken
    ACKNOWLEDGE,    // Acknowledge voor de start
    STATUSSLAVE,    // Vraag voor de status en kijk voor eventuele errors
    SHOOT,          // Schiet. ESPcam maakt een foto en stuurt dat door
    ERROR,          // Error command wordt vervolgt met welke error 
   
    // Definition for SPI Commands from slave to master
    SLAVEACKNOWLEDGE, //
    TEAMCOLOUR,
    GAMEMODE,
    GAME_END,
    GOTHIT,
    SLAVE_ERROR,

    AMOUNTOFCMD
};


#endif