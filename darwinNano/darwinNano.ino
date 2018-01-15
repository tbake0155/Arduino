                                               
#include <SPI.h>
#include "RF24.h"
#include <Message.h>

#define LAMP_TRIGGER_BUTTON 4
#define NOTIFICATION_LED 13
#define SERIAL_UPDATE_RATE 1000 
#define DARWIN_lamp 232

int serial_update_counter = 0;
int darwin_command = 0;
RF24 radio(9,10);
const uint64_t pipes [2] = {0xE8E8F0F0E1LL,0xE8E8F0F0E2LL};
int controller_message [7] = {0};
Message weather_message [6] = {Message()};
uint8_t pipe_num = 9;
bool lamp_relay_triggered = false;

void setup() 
{
    Serial.begin(115200);
    configureRadio();
    configurePins();
}

void loop() 
{
    checkButtonInput();
    checkSerialInput();
    updateSerialState();
    updateRelaysState();
    checkRadioInput();
 
    delay(1);
}

// setup RF24 radio
void configureRadio()
{
   radio.begin();
   radio.openReadingPipe(0,pipes[0]);
   radio.openReadingPipe(1,pipes[1]);
   radio.setDataRate( RF24_2MBPS ) ;
   radio.setPALevel( RF24_PA_MAX ) ;
   radio.powerUp() ;
   radio.startListening();
}

// setup onboard pins to be use
void configurePins()
{
    pinMode(LAMP_TRIGGER_BUTTON, INPUT_PULLUP);
    pinMode(NOTIFICATION_LED, OUTPUT);
    digitalWrite(NOTIFICATION_LED, LOW);
}

void processDarwinCommand(int darwins_intentions)
{
  if(darwins_intentions != 0 && darwins_intentions != -1 )
  {
     if(darwins_intentions == DARWIN_lamp)
     {
         lamp_relay_triggered = true; 
         flashNotificationLED(100);
     }
  }
}

void processRadioInput(uint8_t pipe)
{
    radio.stopListening();
    
    if(pipe == 0)
    {
       if(controller_message[3] == 1)
      {
         lamp_relay_triggered = true; 
      }
    }
    
    if(pipe == 1)
    {
  
    }   
    
    radio.startListening();
}

// check for messages from radio signals
void checkRadioInput()
{

   while(radio.available(&pipe_num))
   {
      if(pipe_num == 0)
      {
        radio.read(controller_message, sizeof(controller_message));
      }
      else if(pipe_num == 1)
      {
         radio.read(weather_message, sizeof(weather_message));
      }
   }
   
   if(pipe_num !=9)
   {
       flashNotificationLED(100);
       processRadioInput(pipe_num);
   } 
  
   pipe_num = 9;
}

// check for messages from Darwin
void checkSerialInput()
{
    if(Serial.available() > 0)
    {
        darwin_command = Serial.read();
        processDarwinCommand(darwin_command);
        darwin_command = 0;
    }
}

// check for change of state for relays.  Relay_1 controls a light switch (under desk)
void updateRelaysState()
{
      if(lamp_relay_triggered)
      {
         Serial.println("Lamp Relay Triggered");
         lamp_relay_triggered = false;
      }
}

void checkButtonInput()
{
    if(digitalRead(LAMP_TRIGGER_BUTTON) == LOW)
    {
       lamp_relay_triggered = true; 
       flashNotificationLED(100);
    }
}

void flashNotificationLED(int Delay)
{
     digitalWrite(NOTIFICATION_LED, HIGH);
     delay(Delay);
     digitalWrite(NOTIFICATION_LED, LOW);
}

// output to the USB serial line certain state paramaters
void updateSerialState()
{
  if(serial_update_counter > SERIAL_UPDATE_RATE){
    serial_update_counter = 0;
}
  
  serial_update_counter++;
}
