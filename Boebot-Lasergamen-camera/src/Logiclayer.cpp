#include "Logiclayer.h"

uint currentState = STATE_0; // 4 posible states
int hitpoints;
int points;
char receivearg[4][17];
byte placemaker = 2;
byte startgame = 3;
byte statusslave = 4;
byte shoot =  5;
byte errorc = 6;
byte teamcolour = 7;
byte acknowledge = 8;
byte error1 = 9;
byte nul = 0;
// char arg(char serCom[]){
// 	char *token;
// 	token = strtok(serCom, ",");
// 	int length = strlen(serCom);
// 	int i;
// 	int p = 0;
// 	while(token != NULL){
// 		for(i=0; i<strlen(token); i++){
// 			receivearg[p][i] = token[i];
// 		}
// 		p++;
// 		token = strtok(NULL, ",");
// 	}
// 	p = 0;
// }

void updateFSM(void)
{
    //Serial.println("Logiclayer1");
    switch(currentState)
    {
        case STATE_0:
            {
                //Gamestate("/gamestate/", MAC_ADDRESS_DEF);
                //Serial.println("State0");
                //char* sendbuf0 = "0.";
                //char receivebuf0[16];
                //char receivearg[4][9];
                blocking_transmit_slave_serial(nul);  
                //arg(receivebuf0);
                if(startgame == RxB)
                {
                    //Gamestate("/gamestate/", MAC_ADDRESS_DEF);
                    currentState = STATE_1;
                }
            }
            break;
        case STATE_1:
            {
                // ACKNOWLEDGE
                //Serial.println("State1");
                //sendbuf[0] = ACKNOWLEDGE;
                //char* sendbuf1 = "0.";
                //char receivebuf1[18];
                blocking_transmit_slave_serial(acknowledge);
                //arg(receivebuf1);
                if(statusslave == RxB)
                {
                    //Gamestate("/gamestate/", MAC_ADDRESS_DEF);
                    currentState = STATE_2;
                }
                else if(startgame == RxB){
                    currentState = STATE_1;
                }
            }
            break;
        case STATE_2:
            {
                // ACKNOWLEDGE
                //sendbuf[0] = STATUSSLAVE;
                //sendbuf[0] = ERROR;
                //char* sendbuf2 = "acknowledge.";
                //char receivebuf2[17];
                blocking_transmit_slave_serial(acknowledge);
                //arg(receivebuf2);
                if(teamcolour == RxB)
                {
                    //Gamestate("/gamestate/", MAC_ADDRESS_DEF);
                    currentState = STATE_3;
                }
                else if(startgame == RxB){
                    currentState = STATE_1;
                }
            }
            break;
        case STATE_3:
            {
                // TEAMCOLOUR
                //char* sendbuf3 = "teamcolour.";
                //char receivebuf3[17];
                blocking_transmit_slave_serial(teamcolour);
                //arg(receivebuf3);
                if(placemaker == RxB)
                {
                    //Gamestate("/gamestate/", MAC_ADDRESS_DEF);
                    Serial.print("Hello_Placemaker");
                    currentState = STATE_4;
                }
                else if(startgame == RxB){
                    Serial.print("Hello_Placemaker 2");
                    currentState = STATE_1;
                }
            }
            break;
        case STATE_4:
            {
                //char* sendbuf4 = "geraakt.";
                //char receivebuf4[12];
                blocking_transmit_slave_serial(nul);
                //arg(receivebuf4);
                if(shoot == RxB)
                {
                    //Serial.print("Hello_world!");
                    //Serial.write("Helloworldtest");
                    //Gamestate("/gamestate/", MAC_ADDRESS_DEF);
                    //sendPhoto();

                }
                else if(startgame == RxB){
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
