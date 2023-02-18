#include <MQ135.h>

#include <SoftwareSerial.h>
#include<DHT.h>       //Software Serial library
SoftwareSerial espSerial(10, 11);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
#define red  7
#define green 6
#define buzzer 5
#define DHTTYPE DHT22
String mySSID = "Codak";       // WiFi SSID
String myPWD = "lemlem064"; // WiFi Password
String myAPI = "WK6HT6APPUC8ZVAS";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1"; 
String myFIELD2 = "field2";
String myFIELD3 = "field3";
String myFIELD4 = "field4";
String myFIELD5 = "field5";
int sendVal;
int sendVal2;
int sendVal3;
int sendVal4;
int sendVal5;

const int analog_gas = A1;
const int sensorvalue2 = 10; //address
const int tempsensor = 2;//address
int watersensor = 0;//value
int vibration_state = 0;//value
const int vibration_analog  = A2;
MQ135 gas_state = MQ135(analog_gas);
const int analogPin = A0;//address

DHT dht(tempsensor , DHTTYPE);

//analoge  value of  MQ 135 sensor are fed in to A0 pin
//analog_gas is directly correlated to Co2 value of the room
//sensorvalue2 is correlated to the vibration level of the bed
//buzzer holds the data of the alarming system 
//red and green are the pin that control the LED outputs
//tempsensor reads the value of the temperature
//watersensor holds information about the level of water content in the bed



void setup()
{
  Serial.begin(9600);
  espSerial.begin(115200);
  pinMode(sensorvalue2 , INPUT);
    dht.begin(); //initiating the sensor
  
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  /*while(!esp.find("OK")) 
  {          
      //Wait for connection
  }*/
pinMode(red , OUTPUT);
pinMode(green , OUTPUT);
pinMode(buzzer , OUTPUT);  
pinMode(vibration_analog , INPUT);
  delay(1000);
  
}

  void loop()
  {
  

 digitalWrite(green , HIGH);
 digitalWrite(red , LOW);
 digitalWrite(buzzer ,LOW);
  float humidity = dht.readHumidity();
  
float tempc = dht.readTemperature();
if(isnan(tempc) ||isnan(humidity))
        Serial.print("failed to read DHT 22\n");

        
else
{
  
    Serial.print("Temperature: ");
    Serial.print(tempc);
    Serial.print("°C ~ \n");
    Serial.print("humdity:");
    Serial.print(humidity);
    Serial.print("%");    
}
if (tempc >= 33)
{
Serial.print("Temprature Level Rising to: ");
Serial.print(tempc);
Serial.print("°C\n");
}
vibration_state = digitalRead(sensorvalue2);
int analog_value = analogRead(vibration_analog);
Serial.print("analoge  value for vibration:");
Serial.print(analog_value);
Serial.print("\n");
if (vibration_state == 1)
{
 digitalWrite(red , HIGH);
 digitalWrite(green , LOW);
 Serial.print("sensing some Vibration\n");
delay(1000);

}
else
{
digitalWrite(red , LOW);
digitalWrite(green ,HIGH);
Serial.print("No Vibration\n");

}
watersensor = analogRead(analogPin);
Serial.print(watersensor);
Serial.print("\n");
int flag1 = 0;
if(watersensor >=400)
{
  flag1 =1;
    digitalWrite(red,HIGH);
    digitalWrite(green , LOW);
    delay(100);

}
 float ppm = gas_state.getPPM();
  if(ppm > 800)
  {
   Serial.print("Detecting high level of Smoke in the Room\n");
   digitalWrite(red , HIGH);
   digitalWrite(green , LOW);
   digitalWrite(buzzer , HIGH);
    
  }





    
    
    sendVal = tempc; 
    sendVal2 = humidity;
    sendVal3 = vibration_analog; //analog vibration value
    sendVal4 = watersensor; //water conductivity
    sendVal5 = ppm;// gas rate in ppm
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(sendVal);
    String sendData2 = "GET /update?api_key="+ myAPI +"&"+ myFIELD2 +"="+String(sendVal2);
    String sendData3 = "GET /update?api_key="+ myAPI +"&"+ myFIELD3 +"="+String(sendVal3);  
    String sendData4 = "GET /update?api_key="+ myAPI +"&"+ myFIELD4 +"="+String(sendVal4);   
    String sendData5 = "GET /update?api_key="+ myAPI +"&"+ myFIELD5 +"="+String(sendVal5);           
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    
        espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
        espData("AT+CIPSEND=0," +String(sendData2.length()+4),1000,DEBUG); 
        espData("AT+CIPSEND=0," +String(sendData3.length()+4),1000,DEBUG); 
        espData("AT+CIPSEND=0," +String(sendData4.length()+4),1000,DEBUG);
        espData("AT+CIPSEND=0," +String(sendData5.length()+4),1000,DEBUG);  
        
        
  
      if (sendVal > 40)
    {
      Serial.println("high temprature");
    }
    espSerial.find(">"); 
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
    Serial.println(sendVal);
     
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(10000);
  }

  String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
    // Serial.print(response);
  }
  return response;
}