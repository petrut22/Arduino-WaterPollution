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
int sensorValue = 0;

void setup() {
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
//the power of the sensor is not constant
//to avoid an harmful environemnt for sensor
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
  //reading values of water sensor
  int level = sensorWaterRead();
  //set up of lcd for a good display of collected data
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

  //i decided to take 800 samples and then average those samples:
  volt = 0;
  for(int i=0; i<800; i++) {
    volt += ((float)analogRead(sensorTurPin)/1023)*5;
  }
  volt = volt/800;
  volt = round_to_dp(volt,1);
  
//trying to send data using bluetooth
  if(bluetooth.available()) {
    bluetooth.print(volt);
    bluetooth.print(" , ");
    bluetooth.println(level);
  }

  //reposition of the cursor in second row
  lcd.setCursor(0, 1);
  lcd.print("Pollution:        ");
  lcd.setCursor(10, 1);
  //trying to display the state of the water from glass
  if(volt >= 4 && volt < 4.3) {
    lcd.print("CLEAN");
  } else if(volt >= 3.2 && volt < 4) {
    lcd.print("LOW");
  } else if(volt >= 2.5 && volt < 3.2) {
    lcd.print("MEDIUM");
  } else if(volt < 2.5) {
    lcd.print("HIGH");
  }
  //if the light is not good for measurement or the water is polluted
  //then the led will light on
  if(photoValue <= 50 || volt < 2.5) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  
  delay(1000);
}
