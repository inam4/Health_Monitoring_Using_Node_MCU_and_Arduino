#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);


void setup() {
  // Initialize Serial port
  
  while (!Serial) continue;
}

void loop() {
  
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
}
