/*int val;
int tempPin = 1;



void setup() {
  // put your setup code here, to run once

Serial.begin(9600);
}

void loop() {
  //put your main code here, to run repeatedly:

val = analogRead(tempPin);
float mv = (val/1024.0)*5000;
float cel = mv/10;
Serial.print("Temprature = " );
Serial.print(cel);
Serial.print("*c");
Serial.println();
*/
int p=3;
int  d=2;
void setup()
{
pinMode(p,OUTPUT);
pinMode(d,OUTPUT);

}
void loop()
{

r();


}
void r(){
digitalWrite(p,HIGH);
digitalWrite(d,HIGH);
delay(1);
}