#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

const char* ssid = "Pantomath";
const char* password = "azizdddd1999";

// Pins variables
#define DHTPIN 18
#define DHTTYPE DHT22
#define ONE_WIRE_BUS 13

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

AsyncWebServer server(80);

String TA = "25.75";
String TS = "27.2";
String HA = "1";
String HS = "10";


String Read_Amb_Humidity(){
  float h = dht.readHumidity();
  if (isnan(h)){
    return String("--");
  }
  else
  {
    return String(110-h);
  }
}
String Read_Soil_Moisture(){
  int a = analogRead(14);
  return String(a);
}

String Read_Amb_Temperature(){
  float t = dht.readTemperature();
  if (isnan(t)){
    return String("--");
  }
  else{return String(t);
}
}
String Read_Soil_Temperature(){
  sensors.requestTemperatures();
  float Celsius = sensors.getTempCByIndex(0);
return String(Celsius);
}
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 Plant monitoring Server</h2>
  <h3>BOUZIDI Abdelaziz</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Ambiante Temperature</span> 
    <span id="temperature1">%TEMPERATUREAMB%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Soil Temperature</span> 
    <span id="temperature2">%TEMPERATURESOIL%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Ambiante Humidity</span>
    <span id="humidity1">%HUMIDITYAMB%</span>
    <sup class="units">&percnt;</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Soil Moisture</span>
    <span id="humidity2">%HUMIDITYSOIL%</span>
    <sup class="units">&percnt;</sup>
  </p>
</body>
</html>)rawliteral";

String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATUREAMB"){
    return TA;
  }
  else if(var == "TEMPERATURESOIL"){
    return TS;
  }
   else if(var == "HUMIDITYAMB"){
    return HA;
  }
    
   else if(var == "HUMIDITYSOIL"){
    return HS;
  }
  return String();
}
void setup() {
  Serial.begin(9600);
  sensors.begin();
  dht.begin();
  pinMode(14,OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", TA.c_str());
  });
  server.on("/temperature2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", TS.c_str());
  });
  server.on("/humidity1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", HA.c_str());
  });
  server.on("/humidity2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", HS.c_str());
  });

  // Start server
  server.begin();

}

void loop() {
//  TA = Read_Amb_Humidity();
//  TS = Read_Soil_Temperature();
//  HA = Read_Amb_Humidity();
//  HS = Read_Soil_Moisture();
//  delay(2000);
}