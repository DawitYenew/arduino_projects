#include <MQ135.h>

#include <SoftwareSerial.h>
#include<DHT.h>       //Software Serial library
SoftwareSerial espSerial(13, 12);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
#define red  7
#define green 6
#define buzzer 5
#define DHTTYPE DHT22
String mySSID = "Codak";       // WiFi SSID
String myPWD = "lemlem0646"; // WiFi Password
String myAPI = "FZQHE2NYUZZBJ5CO";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";


int countTrueCommand;
int countTimeCommand; 
const int analog_gas = A1;
const int sensorvalue2 = 10; //address
const int tempsensor = 2;//address
int watersensor = 0;//value
int vibration_state = 0;//value
const int vibration_analog  = A2;
MQ135 gas_state = MQ135(analog_gas);
const int analogPin = A0;//address
boolean found = false; 

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
  
  // pinMode(sensorvalue2 , INPUT);
  dht.begin(); //initiating the sensor
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"",20,"OK");
  
  // {          
  //     //Wait for connection
  // }*/
pinMode(tempsensor , INPUT);
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


 
  
  


//code to be augmented
 String getData = "GET /update?api_key="  + myAPI +  "&field1=" +get_temperature()+  "&field2=" +get_humidity()+  "&field3="   +get_vibration()+  "&field4=" +get_ppm() +"&field5= "  +get_water_c();
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData),4,">");
 espSerial.println(getData.length()+4);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
    
  }
         
    /*espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
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
*/


//functions that return value from sensors
 String get_temperature()
 {
   float tempc = dht.readTemperature();
if(isnan(tempc))
      Serial.print("failed to read DHT 22\n");     
     else
     {
    Serial.print("Temperature: ");
    Serial.print(tempc);
    Serial.print("°C ~ \n");
      
      }
     if (tempc >= 33)
     {
      Serial.print("Temprature Level Rising to: ");
       Serial.print(tempc);
        Serial.print("°C\n");

     }
     
      String ret = String(tempc);
      
     return ret;
     }
     

 String get_humidity()
 {
    float humidity = dht.readHumidity();
     Serial.print("humdity:");
    Serial.print(humidity);
    Serial.print("%"); 
    Serial.print("\n");
    String ret = String(humidity);
      return ret;
 }
 String get_vibration()
 {
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

                          String ret = String(analog_value);                                                    
                      return "20";
  }
 String get_ppm()
  {
   float ppm = gas_state.getPPM();
  if(ppm > 800)
  {
   Serial.print("Detecting high level of Smoke in the Room\n");
   digitalWrite(red , HIGH);
   digitalWrite(green , LOW);
   digitalWrite(buzzer , HIGH);
    
  }
    String ret = String(ppm);  
        return "30";  
  }
   String get_water_c()
   {
             watersensor = analogRead(analogPin);
              Serial.print("Water Conductivity:") ;         
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
                                String ret = String(watersensor);
      return "20";
     }

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    espSerial.println(command);//at+cipsend
    if(espSerial.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }