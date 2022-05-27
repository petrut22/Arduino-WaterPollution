#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 17, 2);

SoftwareSerial bluetooth(2, 3);

#define ledPin 9
#define photoPin A0

#define sensorPower 7
#define sensorPin A1

#define sensorTurPin A2

float volt;
//float ntu;
int sensorValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bluetooth.begin(9600);
  
  lcd.init();
  lcd.backlight();

  //set up input and output for photoresistor
  pinMode(ledPin, OUTPUT);
  pinMode(photoPin, INPUT);

  //set up input and output for water sensor
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);

}

int sensorWaterRead() {
  digitalWrite(sensorPower, HIGH);
  delay(10);
  int val = analogRead(sensorPin);
  digitalWrite(sensorPower, LOW);
  return val;
}

float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}



void loop() {
  int photoValue = analogRead(photoPin);
  lcd.setCursor(0, 0);
  
  int level = sensorWaterRead();
  lcd.print("Level:        ");
  lcd.setCursor(7, 0);
  if(level > 420 && level <= 520) {
      lcd.print("MEDIUM");
  } else if(level > 0 && level < 420) {
      lcd.print("LOW");
  } else if(level == 0){
      lcd.print("EMPTY");
  } else {
      lcd.print("HIGH");
  }

  volt = 0;
  for(int i=0; i<800; i++) {
    volt += ((float)analogRead(sensorTurPin)/1023)*5;
  }
  volt = volt/800;
  volt = round_to_dp(volt,1);
  
//  if (volt < 2.5){
//    ntu = 3000;
//  } else{
//    ntu = -1120.4*square(volt)+5742.3*volt-4353.8; 
//  }

  //if(bluetooth.available()) {
    Serial.print(volt);
    Serial.print(" , ");
    Serial.println(level);
  //}

  lcd.setCursor(0, 1);
  lcd.print("Pollution:        ");
  lcd.setCursor(10, 1);
  if(volt >= 4 && volt < 4.3) {
    lcd.print("CLEAN");
  } else if(volt >= 3.2 && volt < 4) {
    lcd.print("LOW");
  } else if(volt >= 2.5 && volt < 3.2) {
    lcd.print("MEDIUM");
  } else if(volt < 2.5) {
    lcd.print("HIGH");
  }

  if(photoValue <= 50 || volt < 2.5) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  
  delay(1000);
}
