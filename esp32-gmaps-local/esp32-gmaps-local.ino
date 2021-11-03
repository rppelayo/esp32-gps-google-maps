/*
ESP32 GPS Local Plotter on Google Maps
by Roland Pelayo
for TeachMeMicro

Rev 1.0 - November 1, 2021

Full tutorial on https://www.teachmemicro.com/esp32-gps-google-maps
*/

#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "mainpage.h"
#include "jscript.h"
#include "style.h"

//provide your own WiFi SSID and password
const char* ssid = "<Your WiFi SSID>";
const char* password = "<Your WiFi Password>";

WebServer server(80);

TinyGPS gps;
SoftwareSerial ss;

//For storing data as string
String text= "";
char buff[10];
//GPS data
float flat, flon;
unsigned long age;

void setup(void) {
  // For debugging
  Serial.begin(115200);
  // Initialize gps
  ss.begin(9600, SWSERIAL_8N1, 13, 12, false);
  if(!ss){
    Serial.println("Invalid SoftwareSerial pin configuration, check config"); 
    while (1) { // Don't continue with invalid configuration
      delay (1000);
    }
  }
 
  //Use ESP32 as WiFi Station
  WiFi.mode(WIFI_STA);
  //Initiate WiFi Connection
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  //Print your WiFi's SSID (might be insecure)
  Serial.println(ssid);
  Serial.print("IP address: ");
  //Print your local IP address (needed for browsing the app)
  Serial.println(WiFi.localIP());
  //Page for reading data. Sensor is read in this part
  server.on("/loc", [](){
    gps.f_get_position(&flat, &flon, &age);
    text = floatToString(flat,TinyGPS::GPS_INVALID_F_ANGLE, 10, 6) + ',';
    text += floatToString(flon,TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
    if(text.indexOf("v") > 0){
      text = "7.207573, 125.395874";
    }
    Serial.println(text);
    server.send(200, "text/plain", text);
    smartdelay(500);
  });
  //Home page. Contents of 'page' is in mainpage.h
  server.on("/", []() {
   server.send(200, "text/html", page);
  });
  //JavaScript! Contents of 'javascript' is in jscript.h
  server.on("/jscript.js", []() {
   server.send(200, "text/javascript", javascript);
  });
  //CSS! Contents of 'style' is in style.h
  server.on("/style.css", []() {
   server.send(200, "text/css", style);
  });
  //start web server
  server.begin();
  //Just stating things
  Serial.println("HTTP server started");
}

void loop(void) {
  //Make the ESP32 always handle web clients
  server.handleClient();
}

// Function for converting gps float values to string
String floatToString(float val, float invalid, int len, int prec) {
  String out = "";
  if (val == invalid) {
    while (len-- > 1){
      return "inv" ;
    }
  }
  else{
    for (int i = 0; i < 10; i++) {
       dtostrf(val, len, prec, buff);  
       out += buff;
       return out;
    }
  }
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
