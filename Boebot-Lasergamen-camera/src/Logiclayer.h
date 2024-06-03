#ifndef LOGICLAYER_H
#define LOGICLAYER_H

#include "main.h"

enum SPICMD
{
  // Definition for SPI Commands from master to slave
  SHUTDOWN,
  PLACEMAKER,
  STARTGAME,
  ACKNOWLEDGE,
  STATUSSLAVE,
  SHOOT,
  ERROR,

  // Definition for SPI Commands from slave to master
  SLAVEACKNOWLEDGE,
  TEAMCOLOUR,
  GAMEMODE,
  GAME_END,
  GOTHIT,
  SLAVE_ERROR,

  AMOUNTOFCMD
};

enum MY_STATES
{
  STATE_0 = 0, 
  STATE_1, 
  STATE_2, 
  STATE_3, 
  STATE_4, 
};

extern uint currentState; // 4 posible states

void updateFSM();
void MessageFSM(int message);

#endif