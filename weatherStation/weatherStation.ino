
#include <SPI.h>
#include <RF24.h>

#include <Wire.h>
#include <Adafruit_MPL3115A2.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <Message.h>

#define DHTPIN 7
#define DHTTYPE DHT22    
#define NOTIFICATION_LED 13

const uint64_t pipe = 0xE8E8F0F0E2LL;

RF24 radio(9,10);

Message radio_message [6] = {Message(0.0, "Humidity"), 
                             Message(0.0, "Temperature"), 
                             Message(0.0, "Pressure"), 
                             Message(), Message(), Message()};

DHT_Unified dht(DHTPIN, DHTTYPE);

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

void setup() {
    configureRadio();
    configurePins();
    configureDHT();
    configureMPL();
}

void loop() {
    delay(1000);
    loadDHT22Data();
    loadMPLData();
    writeToRadio();
}

// set up MPL
void configureMPL()
{
    if (! baro.begin()) {
      return;
  }
}

// send radio message
void writeToRadio()
{
   radio.write( radio_message, sizeof(radio_message) );
}

// get MPL data
void loadMPLData()
{
  float pascals = baro.getPressure();
  radio_message[2].Value(pascals);

  //float altm = baro.getAltitude();
  // float tempC = baro.getTemperature();
}

// get DHT22 data
void loadDHT22Data()
{
  // Get temperature event and print its value.
  sensors_event_t event;  
  
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    radio_message[1].Value(event.temperature);
  }
  
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    radio_message[0].Value(event.relative_humidity);
  }

}

// setup DHT22 humidity sensor
void configureDHT()
{
   dht.begin();
   sensor_t sensor;
   dht.temperature().getSensor(&sensor);
   dht.humidity().getSensor(&sensor);
}

// setup RF24 radio
void configureRadio()
{
   radio.begin();
   radio.openWritingPipe(pipe);
   radio.setDataRate( RF24_2MBPS ) ;
   radio.setPALevel( RF24_PA_MAX ) ;
   radio.powerUp() ;
}

// setup onboard pins to be use
void configurePins(){
    pinMode(DHTPIN, INPUT);  
    pinMode(NOTIFICATION_LED, OUTPUT);
    digitalWrite(NOTIFICATION_LED, LOW);
}





