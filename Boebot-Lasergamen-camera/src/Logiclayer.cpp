#include "Logiclayer.h"

uint currentState = STATE_0; // 4 posible states
int hitpoints;
int points;
<<<<<<< HEAD
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
=======
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
>>>>>>> a6b1aecb72dac8391c9fd676f784815e02872f72

void updateFSM(void)
{
    //Serial.println("Logiclayer1");
    // SendMessage("/ESP_DEBUG","updateFSM");
    switch(currentState)
    {
        case STATE_0:
<<<<<<< HEAD

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
=======
            {
                //Gamestate("/gamestate/", MAC_ADDRESS_DEF);
                //Serial.println("State0");
                //char* sendbuf0 = "0.";
                //char receivebuf0[16];
                //char receivearg[4][9];
                // SendMessage("/ESP_DEBUG","State0");
                byte RxBuf = blocking_transmit_slave_serial(acknowledge);  
                //arg(receivebuf0);
                if(startgame == RxBuf)
                {
                    //Gamestate("/gamestate/", MAC_ADDRESS_DEF);
>>>>>>> a6b1aecb72dac8391c9fd676f784815e02872f72
                    currentState = STATE_1;
                }
            }
            break;
        case STATE_1:
            {
                // ACKNOWLEDGE
                //Serial.println("State1");
                //sendbuf[0] = ACKNOWLEDGE;
<<<<<<< HEAD
                char sendbuf1[] = "0,0,0,0.";
                char receivebuf1[18];
                blocking_transmit_slave_serial(sendbuf1, receivebuf1);
                arg(receivebuf1);
                if(strcmp(statusslave, receivearg[0])==0)
                {
                    currentState = STATE_2;
                }
                else if(strcmp(startgame, receivearg[0])==0){
=======
                //char* sendbuf1 = "0.";
                //char receivebuf1[18];
                // SendMessage("/ESP_DEBUG","State1");
                byte RxBuf1 = blocking_transmit_slave_serial(acknowledge);
                //arg(receivebuf1);
                if(statusslave == RxBuf1)
                {
                    //Gamestate("/gamestate/", MAC_ADDRESS_DEF);
                    currentState = STATE_2;
                }
                else if(startgame == RxBuf1){
>>>>>>> a6b1aecb72dac8391c9fd676f784815e02872f72
                    currentState = STATE_1;
                }
            }
            break;
        case STATE_2:
            {
                // ACKNOWLEDGE
                //sendbuf[0] = STATUSSLAVE;
                //sendbuf[0] = ERROR;
<<<<<<< HEAD
                char sendbuf2[] = "acknowledge,0,0,0.";
                char receivebuf2[17];
                blocking_transmit_slave_serial(sendbuf2, receivebuf2);
                arg(receivebuf2);
                if(strcmp(teamcolour, receivearg[0])==0)
                {
                    currentState = STATE_3;
                }
                else if(strcmp(startgame, receivearg[0])==0){
=======
                //char* sendbuf2 = "acknowledge.";
                //char receivebuf2[17];
                // SendMessage("/ESP_DEBUG","State2");
                byte RxBuf2 = blocking_transmit_slave_serial(acknowledge);
                //arg(receivebuf2);
                if(teamcolour == RxBuf2)
                {
                    //Gamestate("/gamestate/", MAC_ADDRESS_DEF);
                    currentState = STATE_3;
                }
                else if(startgame == RxBuf2){
>>>>>>> a6b1aecb72dac8391c9fd676f784815e02872f72
                    currentState = STATE_1;
                }
            }
            break;
        case STATE_3:
            {
                // TEAMCOLOUR
<<<<<<< HEAD
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
=======
                //char* sendbuf3 = "teamcolour.";
                //char receivebuf3[17];
                // SendMessage("/ESP_DEBUG","State3");
                byte RxBuf3 = blocking_transmit_slave_serial(teamcolour);
                //arg(receivebuf3);
                if(placemaker == RxBuf3)
                {
                    //Gamestate("/gamestate/", MAC_ADDRESS_DEF);
                    //Serial.print("Hello_Placemaker");
                    currentState = STATE_4;
                }
                else if(startgame == RxBuf3){
                    //Serial.print("Hello_Placemaker 2");
>>>>>>> a6b1aecb72dac8391c9fd676f784815e02872f72
                    currentState = STATE_1;
                }
            }
            break;
        case STATE_4:
            {
<<<<<<< HEAD
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
=======
                //char* sendbuf4 = "geraakt.";
                //char receivebuf4[12];
                // SendMessage("/ESP_DEBUG","State4");
                byte RxBuf4 = blocking_transmit_slave_serial(nul);
                //arg(receivebuf4);
                if(shoot == RxBuf4)
                {
                    //Serial.print("Hello_world!");
                    //Serial.write("Helloworldtest");
                    //Gamestate("/gamestate/", MAC_ADDRESS_DEF);
                    sendPhoto();
                    //SendMessage("/esp_debug","shoot");
                }
                else if(startgame == RxBuf4){
>>>>>>> a6b1aecb72dac8391c9fd676f784815e02872f72
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
            #ifdef DEBUG
            Serial.println(".");
            #endif
            break;
        case 1:
            #ifdef DEBUG
            Serial.println("Start game");
            #endif
            break;
        case 2:
            #ifdef DEBUG
            Serial.println("End game");
            #endif
            while(msg != 6)
            {   
                /* Game ended. wacht op reset message. */
                msg = WaitForMessage();
                // LedBlink(); --> laat leds knipperen?
            }
            esp_restart();
            break;
        case 3:
            #ifdef DEBUG
            Serial.println("You've been Hit");
            #endif
            hitpoints--;
            // Led hp bar
            break;
        case 4:
            #ifdef DEBUG
            Serial.println("You hit target");
            #endif
            points++;
            break;
        case 5:
            #ifdef DEBUG
            Serial.println("gamestate changed");
            #endif
            Gamestate("/gamestate/", MAC_ADDRESS_DEF);
            break;
        case 6:
            #ifdef DEBUG
            Serial.println("reset");
            #endif
            esp_restart();
        default:
            #ifdef DEBUG
            Serial.println("error");
            #endif
            break;
    }
}
