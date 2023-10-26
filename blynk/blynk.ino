
#define BLYNK_TEMPLATE_ID           "TMPL3D8s8DtfF"
#define BLYNK_TEMPLATE_NAME         "Node "
#define BLYNK_AUTH_TOKEN            ""

#define BLYNK_PRINT Serial


#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
//pins:
const int HX711_dout = 25; //mcu > HX711 dout pin
const int HX711_sck = 26; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;


char ssid[] = "";
char pass[] = "";

BlynkTimer timer;


BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  // Debug console
  
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  
  float calibrationValue; 
  calibrationValue = 99.81; 
  
#if defined(ESP8266)|| defined(ESP32)
  
#endif

  unsigned long stabilizingtime = 2000; 
  boolean _tare = true; 
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); 
    Serial.println("Startup is complete");
  }
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
  
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0;  
   
  if (LoadCell.update()) newDataReady = true;
  float i;
   
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
       i = abs(LoadCell.getData());
      Serial.print("Load_cell output val: ");
      Blynk.virtualWrite(V4, i);
      Serial.println(i);
      newDataReady = 0;
      t = millis();
    }
  }
 
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

   
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
   
  
}
