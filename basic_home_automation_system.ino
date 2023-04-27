#include <Servo.h>
#include <Adafruit_LiquidCrystal.h>
int output1Value = 0;
int sonicVal=101;
int pirVal = 0;
int const gas_sensor = A0;
int const LDR = A1;
int limit = 25;// threshold % for gas sensing.
Adafruit_LiquidCrystal lcd(0);
  

Servo servo_7;

void display_motion()
{  lcd.clear();
  lcd.setCursor(1,0);
lcd.print("MOTION") ;
lcd.setCursor(0,1);
lcd.print("DETECTED |FAN ON!");
 delay(1500);
 return;
}
void display_door()
{ lcd.clear();
  lcd.setCursor(1,0);
lcd.print("DOOR OPEN!") ;
  delay(1500);
 return;
}
void display_gas()
{ lcd.clear();
  lcd.setCursor(1,0);
lcd.print("GAS") ;
lcd.setCursor(1,1);
lcd.print("LEAKING!");
 delay(1500);
 return;
}
void display_light()
{ lcd.clear();
  lcd.setCursor(1,0);
lcd.print("LIGHTS ON!"); 
  delay(1500);
 return;
}
long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Initializing trigger pin oh ultrasonic sensor
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
// Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  //Initializing  Echo pin of ultrasoinc sensor
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}
 void bulb()
 {int val1 = analogRead(LDR);
  if (val1 > 500) 
    {
      digitalWrite(13, HIGH);
    Serial.println();
    Serial.print("Bulb ON = ");
   Serial.print(val1);
    display_light();
    } 
  else 
    { lcd.clear();
      digitalWrite(13, LOW);
       Serial.println();
     Serial.print("Bulb OFF = ");
    Serial.print(val1);
    }
       return;
 }
void fan_motion()
  {
  pirVal = digitalRead(9);
  if (pirVal == 0) 
    {
      digitalWrite(10, LOW); //npn as switch OFF
      digitalWrite(4, HIGH); // Red LED ON,indicating no motion
      digitalWrite(3, LOW); //Green LED OFF, since no Motion detected
      Serial.println();
    Serial.print("NO Motion Detected" );
   lcd.clear();
   lcd.setCursor(2,0);
    lcd.print("FAN OFF");

    }
 
  if (pirVal == 1) 
    {
      digitalWrite(10, HIGH);//npn as switch ON
 //   delay(3000);
      digitalWrite(4, LOW); // RED LED OFF 
      digitalWrite(3, HIGH);//GREEN LED ON , indicating motion detected
      Serial.println();
    Serial.print("Motion Detected!" );
    display_motion();
    }
  delay(300);
  return;
  }
void gas_sense()
{
int val = analogRead(gas_sensor);   
  int gas_level = map(val,0,1023, 0, 100);
   Serial.println();
  Serial.print("Gas% = ");
  Serial.print(gas_level);  
  Serial.print("%");  
 
  if (gas_level > limit)
    {
      tone(8, 750);
    display_gas();
    }
  delay(300);
  noTone(8);
  return;
}
void doorControl()
 {
  sonicVal = 0.01723 * readUltrasonicDistance(6, 6);

  if (sonicVal < 100) 
    {
      servo_7.write(90);
     Serial.println();
    Serial.print("Door Open. Person Distance = ");
    Serial.print(sonicVal);
   Serial.print("\n");
    display_door();
 
    } 
  else 
    { 
      servo_7.write(0);
    lcd.clear();
     lcd.setCursor(1,0);
lcd.print("DOOR CLOSED") ;
  delay(1500);
    Serial.println();
    Serial.print("Door Closed. Person Distance =  ");
    Serial.print(sonicVal);
    Serial.print("\n");
  }
   delay(10); // Delay a little bit to improve simulation performance
  return;
}


void setup()
{
   Serial.begin(115200); // Setting baud rate to 115200
  pinMode(A1, INPUT);   //For LDR 
  pinMode(A0,INPUT);        //For Gas Sensor
  pinMode(13, OUTPUT);    //ForSignal to Relay
  servo_7.attach(7, 500, 2500); //For Servo Motor

  pinMode(8,OUTPUT);      //For Piezo buzzer
  pinMode(9, INPUT);    //For PIR Sensor  
  pinMode(10, OUTPUT);    //For NPN switch
  pinMode(4, OUTPUT);   //For Red LED
  pinMode(3, OUTPUT);   //For Green LED
  lcd.begin(16,2);
lcd.setBacklight(1);
lcd.setCursor(2,0);
lcd.print("DOMOTICS") ;
lcd.setCursor(1,1);
lcd.print("SYSTEM READY");
delay(1000);
 
}

void loop()
{
  doorControl();
  bulb();
  fan_motion();
  gas_sense();
 }
