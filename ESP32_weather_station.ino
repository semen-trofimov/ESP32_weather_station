/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (986.5828) //1013.25 (760мм.рт.ст. = 1013.25 гПа (hPa))
#define BLYNK_PRINT Serial

Adafruit_BME280 bme; // I2C


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "65550392bc5a4e85934406a683da4a6c";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "asus";
char pass[] = "t9indigo";

BlynkTimer timer;
unsigned long delayTime = 1000; // Частота вывода в виджет



void sendSensor()
{
  
// Температура
  Blynk.virtualWrite(V0, bme.readTemperature(), " *С");
  
// Давление в милиметрах ртутного столба
  Blynk.virtualWrite(V1, (bme.readPressure() / 133.3F), " мм.рт.ст.");

// Влажность  
  Blynk.virtualWrite(V2, bme.readHumidity(), " %");

// Высота (Расчетная и не очень точная) это не высотомер  
  Blynk.virtualWrite(V3, bme.readAltitude(SEALEVELPRESSURE_HPA), " м");

}//sendSensor



void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  // Запускаем датчик BME 280
    Serial.println(F("BME280 test"));    
    bool status;
    
// Здесь вводим адрес устройства
// По умолчанию адрес i2c равен 0x77. 
// Если вы добавите перемычку из SDO в V3.3, адрес изменится на 0x77.
// Если вы добавите перемычку из SDO в GND, адрес изменится на 0x76.
    status = bme.begin(0x76);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    bme.setSampling();
    Serial.println("-- Default Test --");
    Serial.println();

//*******************************************************
// Запускаем Blynk
  Blynk.begin(auth, ssid, pass);

// Настраиваем таймеры  
  
  timer.setInterval(delayTime, sendSensor);

// Настраиваем виджеты
  Blynk.setProperty(V0, "label", "Температура");
  Blynk.setProperty(V1, "label", "Давление");
  Blynk.setProperty(V2, "label", "Влажность");
  Blynk.setProperty(V3, "label", "Высота");
  
}//setup

void loop()
{
  Blynk.run();
  timer.run();
}
