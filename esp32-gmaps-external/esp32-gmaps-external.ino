/*
ESP32 GPS External Plotter on Google Maps
by Roland Pelayo
for TeachMeMicro

Rev 1.0 - November 1, 2021

Full tutorial on https://www.teachmemicro.com/esp32-gps-google-maps
*/

#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <WiFi.h>
#include <WiFiClient.h>
//#include <WebServer.h>
//#include "mainpage.h"
//#include "jscript.h"
//#include "style.h"
#include <HTTPClient.h>

//provide your own WiFi SSID and password
const char* ssid = "<Your WiFi SSID>";
const char* password = "<Your WiFi Password>";

//external web server address
const char* server = "http://menrva.online/apps/gps.php";

//WebServer server(80);

TinyGPS gps;
SoftwareSerial ss;

//For storing data as string
String gpsData = "";
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
}

void loop(void) {
  // Send GPS data every second
  gps.f_get_position(&flat, &flon, &age);
  gpsData = "lat="+floatToString(flat,TinyGPS::GPS_INVALID_F_ANGLE, 10, 6) + "&lng=";
  gpsData += floatToString(flon,TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  if(gpsData.indexOf("v") > 0){
      gpsData = "lat=7.207573&lng=125.395874";
  }
  Serial.println(gpsData);
  smartdelay(1000);
  
  HTTPClient client;
  
  client.begin(server);
  client.addHeader("Connection","keep-alive");
  client.addHeader("Content-Length",String(gpsData.length()));
  client.addHeader("Content-Type", "text/plain");
  int httpResponseCode = client.POST(gpsData);

  Serial.println(client.getString());
  if(httpResponseCode > 0){
    Serial.println("Data successfully sent to server!");      
  }else{
    Serial.print("Server upload failed. ");
    Serial.println(client.errorToString(httpResponseCode).c_str());
  }
  // Free resources
  client.end();
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
