#include "Logiclayer.h"

uint currentState = STATE_0; // 4 posible states
int hitpoints;
int points;
// char sendbuf[4] = {0};
// char receivebuf[4] = {0};
char receivearg[4][17];
char startgame[] = "startgame";
char statusslave[] = "statusslave";
char teamcolour[] = "teamcolour";
char placemaker[] = "placemaker";
char shoot[] = "shoot";
char arg(char serCom[]){
	char *token;
	token = strtok(serCom, ",");
	int length = strlen(serCom);
	int i;
	int p = 0;
	while(token != NULL){
		for(i=0; i<strlen(token); i++){
			receivearg[p][i] = token[i];
		}
		p++;
		token = strtok(NULL, ",");
	}
	p = 0;
}

void updateFSM(void)
{
    //Serial.println("Logiclayer1");
    switch(currentState)
    {
        case STATE_0:

            // Gamestate("/gamestate/", MAC_ADDRESS_DEF);
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
                //Gamestate("/gamestate/", MAC_ADDRESS_DEF);
                //Serial.println("State0");
                char sendbuf0[] = "0,0,0,0.";
                char receivebuf0[16];
                char receivearg[4][9];
                blocking_transmit_slave_serial(sendbuf0, receivebuf0);  
                arg(receivebuf0);
                if(strcmp(startgame, receivearg[0])==0)
                {
                    currentState = STATE_1;
                }
            }
            break;
        case STATE_1:
            {
                // ACKNOWLEDGE
                //Serial.println("State1");
                //sendbuf[0] = ACKNOWLEDGE;
                char sendbuf1[] = "0,0,0,0.";
                char receivebuf1[18];
                blocking_transmit_slave_serial(sendbuf1, receivebuf1);
                arg(receivebuf1);
                if(strcmp(statusslave, receivearg[0])==0)
                {
                    currentState = STATE_2;
                }
                else if(strcmp(startgame, receivearg[0])==0){
                    currentState = STATE_1;
                }
            }
            break;
        case STATE_2:
            {
                // ACKNOWLEDGE
                //sendbuf[0] = STATUSSLAVE;
                //sendbuf[0] = ERROR;
                char sendbuf2[] = "acknowledge,0,0,0.";
                char receivebuf2[17];
                blocking_transmit_slave_serial(sendbuf2, receivebuf2);
                arg(receivebuf2);
                if(strcmp(teamcolour, receivearg[0])==0)
                {
                    currentState = STATE_3;
                }
                else if(strcmp(startgame, receivearg[0])==0){
                    currentState = STATE_1;
                }
            }
            break;
        case STATE_3:
            {
                // TEAMCOLOUR
                char sendbuf3[] = "teamcolour,255,0,255.";
                char receivebuf3[17];
                blocking_transmit_slave_serial(sendbuf3, receivebuf3);
                arg(receivebuf3);
                if(strcmp(placemaker, receivearg[0])==0)
                {
                    Serial.print("Hello_Placemaker");
                    currentState = STATE_4;
                }
                else if(strcmp(startgame, receivearg[0])==0){
                    Serial.print("Hello_Placemaker 2");
                    currentState = STATE_1;
                }
            }
            break;
        case STATE_4:
            {
                char sendbuf4[] = "0,0,0,0.";
                char receivebuf4[12];
                blocking_transmit_slave_serial(sendbuf4, receivebuf4);
                arg(receivebuf4);
                if(strcmp(shoot, receivearg[0])==0)
                {
                    //Serial.print("Hello_world!");
                    //Serial.write("Helloworldtest");
                    sendPhoto();
                }
                else if(strcmp(startgame, receivearg[0])==0){
                    currentState = STATE_1;
                }
            }
            break;   
    }
}

void MessageFSM(int message) 
{
    int msg = 0;

    switch(message) {
        case 0:
            // no message.
            Serial.println(".");
            break;
        case 1:
            Serial.println("Start game");
            break;
        case 2:
            Serial.println("End game");
            while(msg != 6)
            {   
                /* Game ended. wacht op reset message. */
                msg = WaitForMessage();
                // LedBlink(); --> laat leds knipperen?
            }
            esp_restart();
            break;
        case 3:
            Serial.println("You've been Hit");
            hitpoints--;
            // Led hp bar
            break;
        case 4:
            Serial.println("You hit target");
            points++;
            break;
        case 5:
            Serial.println("gamestate changed");
            Gamestate("/gamestate/", MAC_ADDRESS_DEF);
            break;
        case 6:
            Serial.println("reset");
            esp_restart();
        default:
            Serial.println("error");
            break;
    }
}
