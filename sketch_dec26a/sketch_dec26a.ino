/*#include<LiquidCrystal.h>
LiquidCrystal lcd(7 ,6 , 5 ,4 ,3 ,2);

int redled = 10;
int greenled = 12;
int buzzer = 8;
int smoke = A0;
int sensor_thershold = 100;





void setup() 
{
  // put your setup code here, to run once:
pinMode(redled , OUTPUT);
pinMode(greenled , OUTPUT);
pinMode(buzzer , OUTPUT);
pinMode(smoke , INPUT);
Serial.begin(9600);
lcd.begin(16,2);

}

void loop() 
{
  // put your main code here, to run repeatedly:
int analogsensor = analogRead(smoke);
Serial.print("pin A0:");
Serial.println(analogsensor);
lcd.print("smokelevel:");
int h = analogsensor - 50;
lcd.print(h);
if (h > sensor_thershold)
{
digitalWrite(redled , HIGH);
lcd.setCursor(0 , 2);
lcd.print("ALERT ......");
digitalWrite(greenled , LOW);
tone(buzzer , 1000 , 200);
}
else
{
digitalWrite(redled , LOW);
digitalWrite(greenled , HIGH);
lcd.setCursor(0 , 2);
lcd.print("....NORMAL CONDITION......");
noTone(buzzer);
}
}
*/

#define red 22
#define green 31
#define buzzer 37

int sensorvalue;
int sensorvalue2;
int tempsensor;
int watersensor;


//analoge  value of  MQ 135 sensor are fed in to A0 pin
//sensorvalue is directly correlated to Co2 value of the room
//sensorvalue2 is correlated to the vibration level of the bed
//buzzer holds the data of the alarming system 
//red and green are the pin that control the LED outputs
//tempsensor reads the value of the temperature
//watersensor holds information about the level of water content in the bed
void setup()
{
Serial.begin(9600);
pinMode(red , OUTPUT);
pinMode(green , OUTPUT);
pinMode(buzzer , OUTPUT);
}

void loop()
{
sensorvalue = analogRead(A0);
sensorvalue2 = analogRead(A1);
tempsensor = analogRead(A2);
watersensor = analogRead(A3);
int mapper3 = map(tempsensor , 0 , 1023 , 0 , 255);
int mapper2 = map(sensorvalue2 , 0 ,1023 , 0 ,255);
int mapper = map(sensorvalue, 0 ,1023 , 0 ,255);
  if (mapper >= 100 || mapper2 > 50 || mapper3 > 30)
  {
     if (mapper >= 100)
      {
              digitalWrite(red , HIGH);
                digitalWrite(green , LOW);
                 digitalWrite(buzzer , HIGH);
                  Serial.print("Co2 level rising to :");
                   Serial.print(mapper ,DEC);
                    Serial.print("PPM");
                     Serial.println("\n");
                      delay(1000);
      }
      if (mapper2 > 50)
       {
         digitalWrite(red , HIGH);
          digitalWrite(green , LOW);
           digitalWrite(buzzer , HIGH);
            Serial.print("Patient needs urgent checkup!!!");
             Serial.println("");
              delay(1000);
        }
         if (mapper3 >30)
          {
            digitalWrite(red ,HIGH);
             digitalWrite(green , LOW);
              digitalWrite(buzzer , HIGH);
               Serial.print("temperature rising to :");
                Serial.print(tempsensor , DEC);
                 Serial.println("");
                  delay(1000);



          }         
   
   
   }
        else
        {
          digitalWrite(red , LOW);
            digitalWrite(green , HIGH);
             digitalWrite(buzzer , LOW);
              Serial.print("...\n");
               Serial.print(mapper ,DEC);
                Serial.print("PPM");
         }
}

