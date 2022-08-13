#include <WiFi.h>
#include <HTTPClient.h>
/*Wi-Fi credentials*/
const char* ssid = "sccatml";
const char* password =  "Tml.5310M";

/*Variables for sensor data*/
String device_id = "TTU";
float current = -1;
float voltage = -1;
float temperature = -1;
float humidity = -1;
float dust = -1;
float light_intensity = -1;
float atmospheric_pressure = -1;

/*Defining pin numbers*/


void setup(){
  Serial.begin(115200);
  delay(4000);   
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { //Wi-Fi connection check
    delay(1000);
    Serial.println("Connecting to Wi-Fi..");
  }
  Serial.println("Wi-Fi Connected!");
}

void loop(){
  if(WiFi.status()== WL_CONNECTED){ //Wi-Fi connection check
    /*Get all sensor data*/
    getCurrent();
    getVoltage();
    getTemperature();
    getHumidity();
    getDust();
    getLightIntensity();
    getAtmosphericPressure();

    /*Start post request to send data*/
    HTTPClient http;
    String url = "https://energierichsolar.com/hardware_code/insert.php?device_id="+device_id+"&current="+current+"&voltage="+voltage+"&temperature="+temperature+"&humidity="+humidity+"&dust="+dust+"&light_intensity="+light_intensity+"&atmospheric_pressure="+atmospheric_pressure+"";
    http.begin(url);
    http.addHeader("Content-Type", "text/plain");
    int httpResponseCode = http.POST("Inserting data");
    if(httpResponseCode>0){
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    }
    else{
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();  //Free resources
  }
  else{
    Serial.println("Error in WiFi connection");
  }
  delay(60000);  //Send data every minute
}


/*reads current*/
void getCurrent(){
  current = random(20);
}

/*reads voltage*/
void getVoltage(){
  voltage = random(36);
}

/*reads temperature*/
void getTemperature(){
  temperature = random(30);
}

/*reads humidity*/
void getHumidity(){
  humidity = random(100);
}

/*reads dust*/
void getDust(){
  dust = random(100);
}

/*reads light intensity*/
void getLightIntensity(){
  light_intensity = random(100);
}

/*reads atmospheric pressure*/
void getAtmosphericPressure(){
  atmospheric_pressure = random(100);
}
