#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
 
//Submitted by : Sibasis Nayak,190050115

const char* ssid     = "Aniket";
const char* password = "123456712";

int REDLED = 21;
int GREENLED = 32;
long ptime,currtime;
int acases,deaths,pacases,pdeaths;

void setup() 
{

  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  digitalWrite(REDLED, LOW);
  digitalWrite(GREENLED, LOW);
  Serial.begin(9600);

  WiFi.enableSTA(true);
  
  delay(2000);

  connectToWifi();
  currtime = 0;
  ptime = currtime;

  initializePcases();

}

void loop(){
  currtime = millis()/1000;
  if(WiFi.status() == WL_CONNECTED){
    if(currtime - ptime >= 86400){
      ptime = currtime;
      doTheTask();
    }
  }
  else{
     Serial.println("WiFi not connected. Please Turn it on.");
     delay(1000);
     connectToWifi();
  }
}

void connectToWifi()
{

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void initializePcases()
{
  HTTPClient http;
  http.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=0PIN96P7H2G0P48Y");
  int httpCode = http.GET();
  if(httpCode>0){ 
      String activeCases = http.getString();  
      Serial.print("Active cases - ");
      Serial.println(activeCases);
      pacases = activeCases.toInt();
  }

  HTTPClient http1;
  http1.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=1OQ0C6XCRBBU3AS7");
  int http1Code = http1.GET();
  if(http1Code>0){ 
      String cdeaths = http.getString();   
      Serial.print("Deaths - ");
      Serial.println(cdeaths); 
      pdeaths = cdeaths.toInt();
  } 

  // Here you have to initialize the pacases,pdeaths with the current value
  // you can fetch it just like that or can copy paste from the wikipedia as this is for one time only.
}

void doTheTask()
{

  HTTPClient http;
  http.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=0PIN96P7H2G0P48Y");
  int httpCode = http.GET();
  if(httpCode>0){ 
      String activeCases = http.getString();   
      Serial.print("Active cases - ");
      Serial.println(activeCases); 
      acases = activeCases.toInt();
  }

  HTTPClient http1;
  http1.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=1OQ0C6XCRBBU3AS7");
  int http1Code = http1.GET();
  if(http1Code>0){ 
      String cdeaths = http.getString();   
      Serial.print("Deaths - ");
      Serial.println(cdeaths);
      deaths = cdeaths.toInt();
  }

  if(acases > 1.01*pacases || deaths > 1.01*pdeaths){
    digitalWrite(REDLED, HIGH);
    digitalWrite(GREENLED, LOW);
  }
  else{
    digitalWrite(REDLED, LOW);
    digitalWrite(GREENLED, HIGH);
  }
  pacases = acases;
  pdeaths = deaths; 

  
  // Here you have to fetch data every time of Active Cases and Deaths for Corona in India(For whole country and not state-wise)
  // Store the data in the variable defined and check whether this data is above or below 1 percent increase from previous datas
  // If any of the data is above 1 percent increase glow REDLED or else glow GREENLED
  // Also remember to turn off other LED
  // Then change previous data to current so to check again from this next day
}
