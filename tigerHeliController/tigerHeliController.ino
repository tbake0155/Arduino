/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
//----tigerHeli--tigerHeli--tigerHeli--tigerHeli--tigerHeli----//
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

const int button1 = 2;
const int button2 = 3;
const int button3  = 4;
const int mainRotor = 7;
const int tailRotor = 6;
const int CE_PIN = 6;
const int CSN_PIN = 7;
const uint64_t pipe = 0xE8E8F0F0E1LL;

int tigerHeli[7]={0};
int mainRotorBuffer=0;
int mainRotorHoldYourHorses=0;
int tailRotorBuffer=0;
RF24 radio(CE_PIN, CSN_PIN);

/////////////////////////////////////////////////////////////////
void setup() 
{
    radio.begin();
    delay(1000);

    radio.openWritingPipe(pipe);
    radio.setDataRate( RF24_2MBPS ) ;
    radio.setPALevel( RF24_PA_MAX ) ;
    radio.powerUp() ;
       
}

/////////////////////////////////////////////////////////////////
void loop() 
{
    mainRotorBuffer = analogRead(mainRotor);
    mainRotorBuffer = map(mainRotorBuffer, 0, 1023, 0, 179);
    
    if (mainRotorBuffer<mainRotorHoldYourHorses+50){   
       tigerHeli[0] = mainRotorBuffer;
       mainRotorHoldYourHorses=mainRotorBuffer;}
   
   
    tigerHeli[1] = analogRead(tailRotor);
    tigerHeli[1] = map(tigerHeli[1], 0, 1023, -179, 179);
    tigerHeli[2] = !digitalRead(button1);
    tigerHeli[3] = !digitalRead(button2);
    tigerHeli[4] = !digitalRead(button3);
    

    if(tigerHeli[2] == 1 || tigerHeli[3] == 1 || tigerHeli[4] == 1 )
    {
      radio.write( tigerHeli, sizeof(tigerHeli) );
    }
    
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
