/* Code Written by Rishi Tiwari
 *  Website:- https://tricksumo.com
*/

#include <WiFiClient.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();


SoftwareSerial nodemcu(D6, D5);



#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


WiFiClient wifiClient;
// Update HOST URL here

#define HOST "ascraplive.000webhostapp.com"          // Enter HOST URL without "http:// "  and "/" at the end of URL

#define WIFI_SSID "Yo"            // WIFI SSID here                                   
#define WIFI_PASSWORD "inam1234"        // WIFI password here

// Declare global variables which will be uploaded to server



String heart ;
String time_1 ;
String temp;
String oxy ;

String  postData;


void setup() {
  
     
  Serial.begin(9600); 
 
  nodemcu.begin(9600);
  mlx.begin();  
  Wire.begin(D4,D3);

  lcd.begin();
  
  lcd.home();
  
  lcd.print("Health Monitor");
  while (!Serial) continue;
  Serial.println("Communication Started \n\n");  
  delay(1000);
    
  
  pinMode(LED_BUILTIN, OUTPUT);     // initialize built in led on the board
 


WiFi.mode(WIFI_STA);           
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
Serial.print("Connecting to ");
lcd.clear();
lcd.print("Connecting wifi");
Serial.print(WIFI_SSID);
while (WiFi.status() != WL_CONNECTED) 
{ Serial.print(".");
    delay(500); }

Serial.println();
Serial.print("Connected to ");
lcd.clear();
lcd.print("Connected");
Serial.println(WIFI_SSID);
Serial.print("IP Address is : ");
Serial.println(WiFi.localIP());    //print local IP address

delay(30);
}



void loop() { 
  
  float celsius =mlx.readObjectTempC();
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
  lcd.clear();
  Serial.println("Object Recieved");
  lcd.print("Object Recieved");

  Serial.print("spo2e:  ");
  float spo2 = data["SPO2"];
  Serial.println(spo2);
  float heart = data["heartbeat"];
  lcd.clear();
  lcd.print("T ");
  lcd.print(celsius);
  lcd.print("C");

  lcd.setCursor(7,0);
  lcd.print("BPM ");
  lcd.print(heart);

  lcd.setCursor(0,1);
  lcd.print("SPO2 ");
  lcd.print(spo2);
  
  Serial.println(heart);
  Serial.println("-----------------------------------------");

  postData = "oxy=" + String(spo2) +  "&temp=" + String(celsius) + "&heart=" + String(heart);
  
 
  
  // Update Host URL here:-  
  
  http.begin(wifiClient,"http://ascraplive.000webhostapp.com/code.php");          
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");           
  
    
   
  int httpCode = http.POST(postData);  
  Serial.println("Values are, heart = " + String(heart) + " and oxy = "+String(oxy) +"and temp = "+String(temp));
  
  
  // if connection eatablished then do this
  if (httpCode == 200) { 
    lcd.clear();
    lcd.print("Uploaded");
    Serial.println("Values uploaded successfully."); 
    Serial.println(httpCode); 
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
  lcd.clear();




}
