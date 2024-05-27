#include "Logiclayer.h"

uint currentState = STATE_0; // 4 posible states

void updateFSM()
{
    //Serial.println("Logiclayer1");
    switch(currentState)
    {
        case STATE_0:
            //Serial.println("State0");
            sendbuf[0] = 0;
            sendbuf[1] = 0;
            sendbuf[2] = 0;
            sendbuf[3] = 0;

            receivebuf[0] = 0;
            receivebuf[1] = 0;
            receivebuf[2] = 0;
            receivebuf[3] = 0;
            blocking_transmit_slave_serial(sendbuf, receivebuf, 8*4);  
            if(STARTGAME == receivebuf[0])
            {
                currentState = STATE_1;
            }
            break;
        case STATE_1:
            // ACKNOWLEDGE
            //Serial.println("State1");
            //sendbuf[0] = ACKNOWLEDGE;
            sendbuf[0] = 0;
            sendbuf[1] = 0;
            sendbuf[2] = 0;
            sendbuf[3] = 0;

            receivebuf[0] = 0;
            receivebuf[1] = 0;
            receivebuf[2] = 0;
            receivebuf[3] = 0;
            blocking_transmit_slave_serial(sendbuf, receivebuf, 8*4);
            if(STATUSSLAVE == receivebuf[0])
            {
                currentState = STATE_2;
            }
            else if(STARTGAME == receivebuf[0])
            {
                currentState = STATE_1;
            }
            break;
        case STATE_2:
            // ACKNOWLEDGE
            //sendbuf[0] = STATUSSLAVE;
            //sendbuf[0] = ERROR;
            sendbuf[0] = ACKNOWLEDGE;
            sendbuf[1] = 0;
            sendbuf[2] = 0;
            sendbuf[3] = 0;

            receivebuf[0] = 0;
            receivebuf[1] = 0;
            receivebuf[2] = 0;
            receivebuf[3] = 0;
            blocking_transmit_slave_serial(sendbuf, receivebuf, 8*4);
            if(TEAMCOLOUR == receivebuf[0])
            {
                currentState = STATE_3;
            }
            else if(STARTGAME == receivebuf[0])
            {
                currentState = STATE_1;
            }
            break;
        case STATE_3:
            // TEAMCOLOUR
            sendbuf[0] = TEAMCOLOUR;
            sendbuf[1] = 1;
            sendbuf[2] = 0;
            sendbuf[3] = 255;

            receivebuf[0] = 0;
            receivebuf[1] = 0;
            receivebuf[2] = 0;
            receivebuf[3] = 0;
            blocking_transmit_slave_serial(sendbuf, receivebuf, 8*4);
            if(PLACEMAKER == receivebuf[0])
            {
                Serial.print("Hello_Placemaker");
                currentState = STATE_4;
            }
            else if(STARTGAME == receivebuf[0])
            {
                Serial.print("Hello_Placemaker 2");
                currentState = STATE_1;
            }
            break;
        case STATE_4:
            sendbuf[0] = 0;
            sendbuf[1] = 0;
            sendbuf[2] = 0;
            sendbuf[3] = 0;

            receivebuf[0] = 0;
            receivebuf[1] = 0;
            receivebuf[2] = 0;
            receivebuf[3] = 0;
            blocking_transmit_slave_serial(sendbuf, receivebuf, 8*4);
            if(SHOOT == receivebuf[0])
            {
                //Serial.print("Hello_world!");
                //Serial.write("Helloworldtest");
                sendPhoto();
            }
            if(STARTGAME == receivebuf[0])
            {
                currentState = STATE_1;
            }
            break;
        
    }
}
