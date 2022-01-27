/* Code Written by Rishi Tiwari
 *  Website:- https://tricksumo.com
*/
int outputpin= A0;
#include <WiFiClient.h>

#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);



#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


WiFiClient wifiClient;
// Update HOST URL here

#define HOST "ascraplive.000webhostapp.com"          // Enter HOST URL without "http:// "  and "/" at the end of URL

#define WIFI_SSID "Yo"            // WIFI SSID here                                   
#define WIFI_PASSWORD "inam1234"        // WIFI password here

// Declare global variables which will be uploaded to server



String heart = "6";
String time_1 = "5";
String temp= "4";
String oxy = "2";

String sendval, sendval2, postData;


void setup() {

     
Serial.begin(9600); 

nodemcu.begin(9600);
while (!Serial) continue;
Serial.println("Communication Started \n\n");  
delay(1000);
  

pinMode(LED_BUILTIN, OUTPUT);     // initialize built in led on the board
 


WiFi.mode(WIFI_STA);           
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
Serial.print("Connecting to ");
Serial.print(WIFI_SSID);
while (WiFi.status() != WL_CONNECTED) 
{ Serial.print(".");
    delay(500); }

Serial.println();
Serial.print("Connected to ");
Serial.println(WIFI_SSID);
Serial.print("IP Address is : ");
Serial.println(WiFi.localIP());    //print local IP address

delay(30);
}



void loop() { 
  int analogValue = analogRead(outputpin);
  float millivolts = (analogValue/1024.0) * 3300; //3300 is the voltage provided by NodeMCU
  float celsius = millivolts/10;
  Serial.print("in DegreeC=   ");
  Serial.println(celsius);
  HTTPClient http;    // http object of clas HTTPClient
  
  
  StaticJsonBuffer<100> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);

  if (data == JsonObject::invalid()) {
    Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;
  }

  Serial.println("JSON Object Recieved");

  Serial.print("spo2e:  ");
  float spo2 = data["SPO2"];
  Serial.println(spo2);
  float heart = data["heartbeat"];
  Serial.println(heart);
  Serial.println("-----------------------------------------");

  postData = "oxy=" + String(spo2) +  "&temp=" + String(celsius) + "&heart=" + String(heart);
  
  // We can post values to PHP files as  example.com/dbwrite.php?name1=val1&name2=val2&name3=val3
  // Hence created variable postDAta and stored our variables in it in desired format
  // For more detials, refer:- https://www.tutorialspoint.com/php/php_get_post.htm
  
  // Update Host URL here:-  
  
  http.begin(wifiClient,"http://ascraplive.000webhostapp.com/code.php");              // Connect to host where MySQL databse is hosted
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");            //Specify content-type header
  
    
   
  int httpCode = http.POST(postData);   // Send POST request to php file and store server response code in variable named httpCode
  Serial.println("Values are, heart = " + String(heart) + " and oxy = "+String(oxy) +"and temp = "+String(temp));
  
  
  // if connection eatablished then do this
  if (httpCode == 200) { Serial.println("Values uploaded successfully."); Serial.println(httpCode); 
  String webpage = http.getString();    // Get html webpage output and store it in a string
  Serial.println(webpage + "\n"); 
  }
  
  // if failed to connect then return and restart
  
  else { 
    Serial.println(httpCode); 
    Serial.println("Failed to upload values. \n"); 
    http.end(); 
    return; }
  
  
  delay(3000); 
  digitalWrite(LED_BUILTIN, LOW);
  delay(3000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10000);
   // Incrementing value of variables 



}
