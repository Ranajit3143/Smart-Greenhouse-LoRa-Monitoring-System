#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define LORA_SS 10
#define LORA_RST 14
#define LORA_DIO0 15

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

String apiKey = "YOUR_THINGSPEAK_WRITE_API_KEY";

void setup()
{
  Serial.begin(115200);

  // WiFi start
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  // LoRa start
  SPI.begin(12,13,11,10);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6))
  {
    Serial.println("LoRa Failed");
    while (1);
  }

  Serial.println("LoRa Receiver Started");
}

void loop()
{
  int packetSize = LoRa.parsePacket();

  if (packetSize)
  {
    String received = "";

    while (LoRa.available())
    {
      received += (char)LoRa.read();
    }

    Serial.println("Received Data:");
    Serial.println(received);

    // Send data to cloud
    if (WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http;

      String url = "http://api.thingspeak.com/update?api_key=" + apiKey + "&field1=" + received;

      http.begin(url);
      int httpCode = http.GET();

      if (httpCode > 0)
      {
        Serial.println("Data sent to ThingSpeak");
      }
      else
      {
        Serial.println("Cloud Upload Failed");
      }

      http.end();
    }
  }
}
