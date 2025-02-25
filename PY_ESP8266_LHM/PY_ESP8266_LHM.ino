#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "place_your_SSID_here";
const char* password = "place_your_SSID_password";
const char* serverUrl = "http://place_your_local_IP:5000/data.json"; // sample >>> http://192.168.x.xx:5000/data.json

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 10800, 60000); // UTC+3 (Istanbul)

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite clock_sprite = TFT_eSprite(&tft);
TFT_eSprite cput_sprite = TFT_eSprite(&tft);
TFT_eSprite cpul_sprite = TFT_eSprite(&tft);
TFT_eSprite gput_sprite = TFT_eSprite(&tft);
TFT_eSprite gpul_sprite = TFT_eSprite(&tft);

#include "Robot_Kicks10pt7b.h"
#include "background.h"
#include "main_img.h"
#define black TFT_BLACK
#define white TFT_WHITE
#define transparent TFT_TRANSPARENT
#define red TFT_RED

String previousDate = "";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  tft.begin();
  tft.setRotation(1); // Adjust rotation if needed

  tft.setSwapBytes(true);
  tft.pushImage(0, 0, 320, 240, background);
  delay (3000);

  tft.setTextWrap(false);

  clock_sprite.setColorDepth(8);
  clock_sprite.createSprite(75,20);
  clock_sprite.fillSprite(black);

  cput_sprite.setColorDepth(8);
  cput_sprite.createSprite(40, 20);
  cput_sprite.fillSprite(black);

  cpul_sprite.setColorDepth(8);
  cpul_sprite.createSprite(40, 20);
  cpul_sprite.fillSprite(black);

  gput_sprite.setColorDepth(8);
  gput_sprite.createSprite(40, 20);
  gput_sprite.fillSprite(black);

  gpul_sprite.setColorDepth(8);
  gpul_sprite.createSprite(40, 20);
  gpul_sprite.fillSprite(black);

  

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  timeClient.begin();
  timeClient.setTimeOffset(10800); // UTC+3 (Istanbul)
  tft.pushImage(0, 0, 320, 240, main_img);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) 
  {
    WiFiClient client;
    HTTPClient http;
    

    // Use the new API: begin(WiFiClient, url)
    http.begin(client, serverUrl);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Received JSON data:");
      Serial.println(payload);

      // Parse JSON data
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      // Extract CPU and GPU data
      JsonObject cpu = doc["CPU"];
      JsonObject gpu = doc["GPU"];

      Serial.println("CPU:");
      Serial.print("Name: ");
      Serial.println(cpu["Name"].as<String>());
      Serial.print("Temperature: ");
      Serial.println(cpu["Temperature"].as<String>());
      Serial.print("Load: ");
      Serial.println(cpu["Load"].as<String>());

      Serial.println("GPU:");
      Serial.print("Name: ");
      Serial.println(gpu["Name"].as<String>());
      Serial.print("Temperature: ");
      Serial.println(gpu["Temperature"].as<String>());
      Serial.print("Load: ");
      Serial.println(gpu["Load"].as<String>());

      String cpu_name = cpu["Name"].as<String>();
      String gpu_name = gpu["Name"].as<String>();

      String cpu_temp = cpu["Temperature"].as<String>();
      String cpu_load = cpu["Load"].as<String>();

      String gpu_temp = gpu["Temperature"].as<String>();
      String gpu_load = gpu["Load"].as<String>();

      dateandtime();
      showgpu_cpu(cpu_name, gpu_name);
      showcpu_loadtemp(cpu_temp, cpu_load, gpu_temp, gpu_load);


    } else {
      Serial.print("Failed to fetch data. HTTP Code: ");
      Serial.println(httpCode);
    }
    http.end();
  }
  delay(1000); // Fetch data every 5 seconds
}

void dateandtime() 
{
  // Update the time from the NTP server
  timeClient.update();

  // Get the current epoch time
  unsigned long epochTime = timeClient.getEpochTime();

  // Convert epoch time to a tm structure
  time_t rawTime = (time_t)epochTime; // Cast to time_t
  struct tm *ptm = gmtime(&rawTime); // Use gmtime with time_t

  // Extract date and time
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1; // Month is 0-indexed
  int currentYear = ptm->tm_year + 1900; // Years since 1900

  // Format the date as dd-mm-yyyy
  char dateBuffer[11];
  sprintf(dateBuffer, "%02d-%02d-%04d", currentDay, currentMonth, currentYear);

  // Format the time as hh:mm
  char timeBuffer[6];
  sprintf(timeBuffer, "%02d:%02d", currentHour, currentMinute);

  // Print the time and date
  Serial.print("Time: ");
  Serial.println(timeBuffer);

  Serial.print("Date: ");
  Serial.println(dateBuffer);

  clock_sprite.fillSprite(black);
  clock_sprite.setTextSize(1);
  clock_sprite.setFreeFont(&Robot_Kicks10pt7b);  
  clock_sprite.setTextDatum(4);
  clock_sprite.setTextColor(white, black);
  clock_sprite.drawString(timeBuffer, 35, 5);
  
  clock_sprite.pushSprite(40,6);

  String currentDate = String(dateBuffer);
  if (currentDate != previousDate)
  {
    tft.setTextSize(1);
    tft.setFreeFont(&Robot_Kicks10pt7b);  
    tft.setTextDatum(4);
    tft.setTextColor(white, black);
    tft.drawString(dateBuffer, 215, 11);

    previousDate = currentDate;
  }
  


}

void showgpu_cpu(String name_cpu, String name_gpu)
{
  
  if (name_cpu.indexOf("Intel") != -1)
  {
    tft.setTextSize(1);
    tft.setTextDatum(4);
    tft.setFreeFont();
    tft.setTextColor(white, TFT_TRANSPARENT);
    tft.drawString("Intel", 80, 230);
  }
  else
  {
    tft.setTextSize(1);
    tft.setTextDatum(4);
    tft.setFreeFont();
    tft.setTextColor(white, TFT_TRANSPARENT);
    tft.drawString("AMD", 80, 230);
  }

  if (name_gpu.indexOf("NVIDIA") != -1)
  {
    tft.setTextSize(1);
    tft.setTextDatum(4);
    tft.setFreeFont();
    tft.setTextColor(white, TFT_TRANSPARENT);
    tft.drawString("NVIDIA", 240, 230);
  }
  else
  {
    tft.setTextSize(1);
    tft.setTextDatum(4);
    tft.setFreeFont();
    tft.setTextColor(white, TFT_TRANSPARENT);
    tft.drawString("AMD", 240, 230);
  }


}

void showcpu_loadtemp(String temp_cpu, String load_cpu, String temp_gpu, String load_gpu)
{ 
  int commaIndexCpuTemp = temp_cpu.indexOf(',');
  String integerPartCpuTemp = temp_cpu.substring(0, commaIndexCpuTemp);
  int temperatureValueCpuTemp = integerPartCpuTemp.toInt();

  int commaIndexCpuLoad = load_cpu.indexOf(',');
  String integerPartCpuLoad = load_cpu.substring(0, commaIndexCpuLoad);
  int temperatureValueCpuLoad = integerPartCpuLoad.toInt();
    
  cput_sprite.fillSprite(black);
  cput_sprite.setFreeFont(&Robot_Kicks10pt7b);
  cput_sprite.setTextSize(1);
  cput_sprite.setTextColor(white, black);
  cput_sprite.drawNumber(temperatureValueCpuTemp, 0, 0);  
  cput_sprite.pushSprite(90, 153);

  cpul_sprite.fillSprite(black);
  cpul_sprite.setFreeFont(&Robot_Kicks10pt7b);
  cpul_sprite.setTextSize(1);
  cpul_sprite.setTextColor(white, black);
  cpul_sprite.drawNumber(temperatureValueCpuLoad, 0, 0);
  cpul_sprite.pushSprite(90, 187);
  
  int commaIndexGpuTemp = temp_gpu.indexOf(',');
  String integerPartGpuTemp = temp_gpu.substring(0, commaIndexGpuTemp);
  int temperatureValueGpuTemp = integerPartGpuTemp.toInt();

  int commaIndexGpuLoad = load_gpu.indexOf(',');
  String integerPartGpuLoad = load_gpu.substring(0, commaIndexGpuLoad);
  int temperatureValueGpuLoad = integerPartGpuLoad.toInt();
    
  gput_sprite.fillSprite(black);  
  gput_sprite.setFreeFont(&Robot_Kicks10pt7b);
  gput_sprite.setTextSize(1);
  gput_sprite.setTextColor(white, black);
  gput_sprite.drawNumber(temperatureValueGpuTemp, 0, 0);
  gput_sprite.pushSprite(250, 153);

  gpul_sprite.fillSprite(black);
  gpul_sprite.setFreeFont(&Robot_Kicks10pt7b);
  gpul_sprite.setTextSize(1);
  gpul_sprite.setTextColor(white, black);
  gpul_sprite.drawNumber(temperatureValueGpuLoad, 0, 0);
  gpul_sprite.pushSprite(250, 187);
 



}
