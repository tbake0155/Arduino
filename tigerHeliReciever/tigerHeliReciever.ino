

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int button1=3;
const int button2=4;
const int button3=5;
const int CE_PIN = 9;
const int CSN_PIN = 10;
const uint64_t pipe = 0xE8E8F0F0E1LL;

int tigerHeli[7]={0};



RF24 radio(CE_PIN, CSN_PIN);


//////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
    Serial.begin(9600);
    radio.begin();

    radio.openReadingPipe(1,pipe);
    radio.setDataRate( RF24_2MBPS ) ;
    radio.setPALevel( RF24_PA_MAX ) ;
    radio.powerUp() ;
    radio.startListening();

     delay(500);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
 
        while(radio.available()){
          radio.read( tigerHeli, sizeof(tigerHeli));
         
 
          Serial.print("mR: ");
          Serial.print(tigerHeli[0]);
          Serial.print("\tR: ");
          Serial.print(tigerHeli[1]);
          Serial.print("\tb1: ");
          Serial.print(tigerHeli[2]);
          Serial.print("\tb2: ");
          Serial.print(tigerHeli[3]);
          Serial.print("\tb3: ");
          Serial.println(tigerHeli[4]);
        }
    }

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

