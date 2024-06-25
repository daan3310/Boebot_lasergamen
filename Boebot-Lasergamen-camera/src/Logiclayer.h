#ifndef LOGICLAYER_H
#define LOGICLAYER_H

#include "main.h"
#include "string.h"
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
extern DMA_ATTR char sendbuf[4];
extern DMA_ATTR char receivebuf[4];
<<<<<<< HEAD
char arg(char serCom[], char ser[4][17]);
=======
//char arg(char serCom[], char ser[4][17]);
>>>>>>> a6b1aecb72dac8391c9fd676f784815e02872f72
void updateFSM(void);
void MessageFSM(int message);

#endif