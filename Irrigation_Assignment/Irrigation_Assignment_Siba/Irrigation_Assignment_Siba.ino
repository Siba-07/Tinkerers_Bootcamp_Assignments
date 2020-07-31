//#####               Submitted by: Sibasis Nayak                 ######//

/////////////// TWO VIRTUAL PINS USED V1 AND V2 //////////////////////
/////////////// V1 for the timer self irrigation//////////////////////
/////////////// V2 for the manual input via blynk app ////////////////

#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHTesp.h>

/** Initialize DHT sensor */
DHTesp dht;
/** Task handle for the light value read task */
TaskHandle_t tempTaskHandle = NULL;
/** Pin number for DHT11 data pin */
int dhtPin = 17;


char auth[] = "YourAuthToken";


char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";


BlynkTimer timer;
TempAndHumidity Data;

int Led = 21;
int LEDVALUE = 0;

void irrigate()
{ 
  Data = dht.getTempAndHumidity();
  Blynk.virtualWrite(V1,Data.humidity);     // writing the humidity into the blynk app
  
  if(Data.humidity < 40 ){
    digitalWrite(Led,HIGH);  // if humidity is less then 40 then it will self irrigate
  }
  else{
    if(LEDVALUE != 1){
      digitalWrite(Led,LOW);    //check if the person has not manually configured it to be on
    }
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(Led,OUTPUT);
  dht.setup(dhtPin, DHTesp::DHT11);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000l,irrigate);     // setting up the timer so that the humidity data is received every second
  
}

void loop()
{ 
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V2)     
{
  LEDVALUE = param.asInt();
  digitalWrite(Led,LEDVALUE);     // what the preson gives that is stored globally in LEDVALUE
}
