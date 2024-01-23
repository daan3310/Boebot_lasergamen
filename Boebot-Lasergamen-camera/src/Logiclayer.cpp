#include "Logiclayer.h"

uint currentState = STATE_0; // 4 posible states

void updateFSM()
{
    switch(currentState)
    {
        case STATE_0:
            sendbuf[0] = 0;
            sendbuf[1] = 0;
            sendbuf[2] = 0;
            sendbuf[3] = 0;

            receivebuf[0] = 0;
            receivebuf[1] = 0;
            receivebuf[2] = 0;
            receivebuf[3] = 0;
            blocking_transmit_slave_spi(sendbuf, receivebuf, 8*5);
            currentState = STATE_1;
            break;
        case STATE_1:
            // ACKNOWLEDGE
            sendbuf[0] = ACKNOWLEDGE;
            sendbuf[1] = 0;
            sendbuf[2] = 0;
            sendbuf[3] = 0;

            receivebuf[0] = 0;
            receivebuf[1] = 0;
            receivebuf[2] = 0;
            receivebuf[3] = 0;
            blocking_transmit_slave_spi(sendbuf, receivebuf, 8*5);
            currentState = STATE_2;
            break;
        case STATE_2:
            // ACKNOWLEDGE
            sendbuf[0] = STATUSSLAVE;
            sendbuf[1] = 0;
            sendbuf[2] = 0;
            sendbuf[3] = 0;

            receivebuf[0] = 0;
            receivebuf[1] = 0;
            receivebuf[2] = 0;
            receivebuf[3] = 0;
            blocking_transmit_slave_spi(sendbuf, receivebuf, 8*5);
            currentState = STATE_3;
            break;
        case STATE_3:
            // TEAMCOLOUR
            sendbuf[0] = TEAMCOLOUR;
            sendbuf[1] = 0;
            sendbuf[2] = 0;
            sendbuf[3] = 0;

            receivebuf[0] = 0;
            receivebuf[1] = 0;
            receivebuf[2] = 0;
            receivebuf[3] = 0;
            blocking_transmit_slave_spi(sendbuf, receivebuf, 8*5);
            currentState = STATE_4;
            break;
        case STATE_4:
            sendbuf[0] = 0xFF;
            sendbuf[1] = 0;
            sendbuf[2] = 0;
            sendbuf[3] = 0;

            receivebuf[0] = 0;
            receivebuf[1] = 0;
            receivebuf[2] = 0;
            receivebuf[3] = 0;
            blocking_transmit_slave_spi(sendbuf, receivebuf, 8*5);
            break;
        
    }
}
