#include <Wire.h>
#include <LiquidCrystal_I2C.h>
int voltPin = A0;
int groundRef = A1;
int curPin = A2;
const float ANALOG_RANGE = 1023.0;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// LCD ADRESS 0x27
const int analog_max = 4095;
const int maxVoltReading = 25;
void setup() {
  Wire.begin();
  pinMode(voltPin, INPUT);
  pinMode(groundRef, INPUT);
  pinMode(curPin, INPUT);
  Serial.begin(115200);
  Serial.println("Setup Complete!");
  for(int address = 1; address < 127; address++ ){
    Wire.beginTransmission(address);
    int error = Wire.endTransmission();
    if (error == 0){
      Serial.print("I2C device found at address 0x");
      if (address<16)
      Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
    }
  }
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
}

void loop() {
  //Serial.println("Voltage in V: "+ String(getVoltage()));
  //Serial.println("Current in mA: " + String(getCurrent()));
  //lcd.setCursor(0,0);
  //lcd.print("Volt: " + String(getVoltage()));
  //lcd.setCursor(0,1);
  //lcd.print("milliA: " + String(getCurrent()));
  //Serial.println();
  getVoltage();
  delay(2000);
  getCurrent();
  //Serial.println( "Cur: "+String(analogRead(curPin)));
  //Serial.println( "Ground: "+String(analogRead(groundRef)));
  delay(2000);
  /*double sum = 0.0;
  for(int i = 0; i < 1000; i++){
    sum += getVoltage();
    delay(5);
  }
  Serial.println(sum / 1000.0);*/

}
float getVoltage(){
  float val = analogRead(voltPin);
  val = val / ANALOG_RANGE;
  lcd.setCursor(0,0);
  lcd.print("Volt: "+String(val * 5 * 5));
  return val * 5 * 5; // . multiply by ayatwm voltage then 5x multiplier
   
}
// constant 9.85 kOhms
double getCurrent(){
  //double val = 5000 * (analogRead(curPin) / ANALOG_RANGE);
  //return  (val - 2500) / 66;
  //return (val - 2500) / 66; // constants found online
  double resistor = 9850.0; // constant measured
  double voltDrop = 5000 * (analogRead(curPin) / ANALOG_RANGE);
  lcd.setCursor(0,0);
  lcd.print("Volt: "+String(voltDrop / resistor));
  return voltDrop / resistor;
}
//3.304 v = 631.6
