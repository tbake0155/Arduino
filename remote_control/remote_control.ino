/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int joystickbutton = 0;
const int greenbutton = 1;
const int bluebutton = 9;
const int yellowbutton  = 8;
const int redbutton = 4;
const int throttlePin = 2;
const int tailthrottlePin = 3;
const int CE_PIN = 6;
const int CSN_PIN = 7;
const uint64_t pipe = 0xE8E8F0F0E1LL;

int joystick[7]={0};
RF24 radio(CE_PIN, CSN_PIN);

/////////////////////////////////////////////////////////////////
void setup() 
{
    Serial.begin(9600);
    delay(1000);

    radio.begin();
    delay(1000);
    radio.openWritingPipe(pipe);
}

/////////////////////////////////////////////////////////////////
void loop() 
{
    joystick[0] = analogRead(throttlePin);
    joystick[0] = map(joystick[0], 0, 501, 179, 0);
    joystick[1] = analogRead(tailthrottlePin);
    joystick[1] = map(joystick[1], 0, 1023, 179, -179);
    joystick[2] = analogRead(joystickbutton);
    joystick[2] = map(joystick[2], 0, 1023, 0, 1);
    joystick[3] = analogRead(greenbutton);
    joystick[3] = map(joystick[3], 0, 1023, 0, 1);
    joystick[4] = digitalRead(bluebutton);
    joystick[5] = digitalRead(yellowbutton);
    joystick[6] = digitalRead(redbutton);

    radio.write( joystick, sizeof(joystick) );
    
    Serial.print(joystick[0]);
    Serial.print("\t");
    Serial.print(joystick[1]);
    Serial.print("\t");
    Serial.print(joystick[2]);
    Serial.print("\t");
    Serial.print(joystick[3]);
    Serial.print("\t");
    Serial.print(joystick[4]);
    Serial.print("\t");
    Serial.print(joystick[5]);
    Serial.print("\t");
    Serial.println(joystick[6]);

    delay(10);
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
