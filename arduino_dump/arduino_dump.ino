
#include <SoftwareSerial.h>
#include <ArduinoJson.h>


SoftwareSerial nodemcu(5, 6);
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

MAX30105 particleSensor;

#define MAX_BRIGHTNESS 255



//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[10]; //infrared LED sensor data
uint16_t redBuffer[10];  //red LED sensor data


int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

byte pulseLED = 11; //Must be on PWM pin
byte readLED = 13; //Blinks with each data read
int8_t count;
void setup()
{
  Serial.begin(9600); // initialize serial communication at 115200 bits per second:
  nodemcu.begin(9600);
  delay(1000);

  Serial.println("Program started");
  pinMode(pulseLED, OUTPUT);
  pinMode(readLED, OUTPUT);

  // Initialize sensor
        if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }

  Serial.println(F("V2"));
 
  particleSensor.setup(); //Configure sensor with these settings

}

void loop()
{
  bufferLength = 25; //buffer length of 100 stores 4 seconds of samples running at 25sps

  //read the first 100 samples, and determine the signal range
  
  //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
  while (1)
  {
    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (byte i = 0; i < 10; i++)
    {
      redBuffer[i] = redBuffer[i];
      irBuffer[i] = irBuffer[i];
    }
    count=0;
    //take 25 sets of samples before calculating the heart rate.
    for (byte i = 0; i < 10; i++)
    {
      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data

      digitalWrite(readLED, !digitalRead(readLED)); //Blink onboard LED with every data read

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //We're finished with this sample so move to next sample

      //send samples and calculation result to terminal program through UART
      

      Serial.print(F(", HR="));
      Serial.print(heartRate, DEC);

      Serial.print(F(", HRvalid="));
      Serial.print(validHeartRate, DEC);

      Serial.print(F(", SPO2="));
      Serial.print(spo2, DEC);

      Serial.print(F(", SPO2Valid="));
      Serial.println(validSPO2, DEC);
      if (validSPO2==1)
      {
          
          
          if (redBuffer[i]>5000)
          {
            count=count+1;
            Serial.print("TRUE");  
          }
          
          if (count==10)
          {
           
         
            delay(4000);
            StaticJsonBuffer<100> jsonBuffer;
            JsonObject& data = jsonBuffer.createObject();
          
            //Obtain Temp and Hum data
    
          
          
            //Assign collected data to JSON Object
 
            data["heartbeat"] = heartRate;
            data["SPO2"]=spo2;      
            //Send data to NodeMCU
            data.printTo(nodemcu);
            jsonBuffer.clear();  
            
            Serial.println("send"); 
            delay(2000);
          }
       
        }
    }

    //After gathering 25 new samples recalculate HR and SP02
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  }
}
