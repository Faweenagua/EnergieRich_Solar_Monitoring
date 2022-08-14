/*
PIN NUMBERS
LCD {GND: GND, Vcc:Vin, SDA: SDA/21, SCL:SCL/22} 
*/

/*Including libraries*/
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

/*I2C stuff*/
LiquidCrystal_I2C lcd(0x27, 16, 2); //Set LCD
BH1750 lightMeter;
Adafruit_BME680 bme;

/*Wi-Fi credentials*/
const char* ssid = "sccatml";
const char* password =  "Tml.5310M";

/*Variables for sensor data*/
String device_id = "Sunday_Test";
float current = -1;
float voltage = -1;
float temperature = -1;
float humidity = -1;
float dust = -1;
float light_intensity = -1;
float atmospheric_pressure = -1;
float air_pollution = -1;
float altitude = -1;

/*Defining MACROS and pin numbers*/
#define SEALEVELPRESSURE_HPA (1013.25)


void setup(){
  //start serial monitor and LCD
  Serial.begin(115200);
  lcd.begin();                     
  lcd.backlight();
  lcdPrintFirstLine("System Starting");
  lcdPrintSecondLine("  EnergieRich  ");
  //WiFi stuff   
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { //Wi-Fi connection check
    delay(1000);
    Serial.println("Connecting to Wi-Fi..");
    lcdPrintFirstLine("Connecting Wi-Fi");
  }
  Serial.println("Wi-Fi Connected!");
  lcdPrintSecondLine("Wi-Fi Connected!");

  //start light intensity sensor
  if(lightMeter.begin()){
    lcdPrintFirstLine("Light sensor set");
  }
  else{
    lcdPrintFirstLine("Error: Light");
  }

  //start BME sensor
  if (bme.begin()) {
    lcdPrintFirstLine("BME sensor set");
  }
  else{
    lcdPrintFirstLine("Error: BME");
  }
  // Set up oversampling and filter initialization for BME sensor
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void loop(){
  if(WiFi.status()== WL_CONNECTED){ //Wi-Fi connection check
    /*Get all sensor data*/
    //start with BME sensor
    bme.beginReading();
    delay(10000);
    getLightIntensity();
    if(bme.endReading()){
      getTemperature();
      getHumidity();
      getAtmosphericPressure();
      getAirPollution();
      getAltitude();
    }
    // getCurrent();
    // getVoltage();
    // getDust();    

    /*Start post request to send data*/
    HTTPClient http;
    String url = "https://energierichsolar.com/hardware_code/insert.php?device_id="+device_id+"&current="+String(current)+"&voltage="+String(voltage)+"&temperature="+String(temperature)+"&humidity="+String(humidity)+"&dust="+String(dust)+"&light_intensity="+String(light_intensity)+"&atmospheric_pressure="+String(atmospheric_pressure)+"";
    http.begin(url);
    Serial.print("URL: ");
    Serial.println(url);
    lcdPrintFirstLine("Uploading data..");
    http.addHeader("Content-Type", "text/plain");
    int httpResponseCode = http.POST("Inserting data");
    if(httpResponseCode==200){
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
      lcdPrintFirstLine("Data Uploaded");
    }
    else{
      String response = httpResponseCode>0? http.getString(): "No response";
      Serial.print("Error on sending POST: ");
      lcdPrintFirstLine("Data send error");
      Serial.println(httpResponseCode);
      Serial.println(response);
    }
    http.end();  //Free resources
  }
  else{
    Serial.println("Error in WiFi connection");
    lcdPrintFirstLine("Wi-Fi Error");
  }
  lcdPrintFirstLine("Powered by");
  lcdPrintSecondLine("     EnergieRich");
  delay(30000);  //300000 - Send data every 5 minutes
}


/*reads current*/
void getCurrent(){
  current = random(20);
  //print to LCD
  lcdPrintFirstLine("Current:");
  lcdPrintSecondLine(String(current));
}

/*reads voltage*/
void getVoltage(){
  voltage = random(36);
  //print to LCD
  lcdPrintFirstLine("Voltage:");
  lcdPrintSecondLine(String(voltage));
}

/*reads temperature*/
void getTemperature(){
  temperature = bme.temperature;
  //print to LCD
  lcdPrintFirstLine("Temperature:");
  lcdPrintSecondLine(String(temperature)+" *C");
}

/*reads humidity*/
void getHumidity(){
  humidity = bme.humidity;
  //print to LCD
  lcdPrintFirstLine("Humidity:");
  lcdPrintSecondLine(String(humidity)+" %");
}

/*reads dust*/
void getDust(){
  dust = random(100);
  //print to LCD
  lcdPrintFirstLine("Dust:");
  lcdPrintSecondLine(String(dust));
}

/*reads light intensity*/
void getLightIntensity(){
  //light_intensity = random(100);

  light_intensity = lightMeter.readLightLevel();  
  //print to LCD
  lcdPrintFirstLine("Light Intensity:");
  lcdPrintSecondLine(String(light_intensity)+" lx");
}

/*reads atmospheric pressure*/
void getAtmosphericPressure(){
  atmospheric_pressure = bme.pressure / 100.0;
  //print to LCD
  lcdPrintFirstLine("Atmos Pressure:");
  lcdPrintSecondLine(String(atmospheric_pressure)+" hPa");
}

/*reads atmospheric pressure*/
void getAirPollution(){
  //high resitance = low pollution
  air_pollution = bme.gas_resistance / 1000.0;
  //print to LCD
  lcdPrintFirstLine("Air Pollution:");
  lcdPrintSecondLine(String(air_pollution)+" KOhms");
}


/*reads atmospheric pressure*/
void getAltitude(){
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  //print to LCD
  lcdPrintFirstLine("Altitude:");
  lcdPrintSecondLine(String(altitude)+" m");
}

void lcdPrintFirstLine(String text){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(text);
  delay(1000);
}
void lcdPrintSecondLine(String text){
  lcd.setCursor(0, 1);
  lcd.print(text);
  delay(1000);
}
