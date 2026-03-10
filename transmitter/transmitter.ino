#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <BH1750.h>
#include <DHT.h>
#include <MHZ19.h>

#define DHTPIN 4
#define DHTTYPE DHT22

#define SDA_PIN 8
#define SCL_PIN 9

#define SOIL_PIN 1
#define PH_PIN 2
#define EC_PIN 7
#define WATER_PIN 5
#define FLOW_PIN 6

#define RX_PIN 16
#define TX_PIN 17

// LoRa pins
#define LORA_SS 10
#define LORA_RST 14
#define LORA_DIO0 15

DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;

MHZ19 myMHZ19;
HardwareSerial mySerial(1);

volatile int flowPulse = 0;
float flowRate = 0;

void IRAM_ATTR flowCounter()
{
  flowPulse++;
}

void setup()
{
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  dht.begin();
  lightMeter.begin();

  pinMode(WATER_PIN, INPUT);
  pinMode(FLOW_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), flowCounter, FALLING);

  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  myMHZ19.begin(mySerial);

  // LoRa start
  SPI.begin(12,13,11,10);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6))
  {
    Serial.println("LoRa Failed");
    while (1);
  }

  Serial.println("Greenhouse Sensors Started");
}

void loop()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float lux = lightMeter.readLightLevel();
  int co2 = myMHZ19.getCO2();

  int soilValue = analogRead(SOIL_PIN);
  int phValue = analogRead(PH_PIN);
  int ecValue = analogRead(EC_PIN);
  int waterLevel = digitalRead(WATER_PIN);

  flowPulse = 0;
  delay(1000);
  flowRate = flowPulse / 7.5;

  String report = "";

  report += "\n------ AIR PARAMETERS ------\n\n";
  report += "Temperature: " + String(temperature) + " C\n";
  report += "Humidity: " + String(humidity) + " %\n\n";
  report += "CO2: " + String(co2) + " ppm\n\n";
  report += "Light Intensity: " + String(lux) + " Lux\n\n";
  report += "-----------------------------\n\n";

  report += "------ SOIL PARAMETERS ------\n\n";
  report += "Soil Moisture Raw: " + String(soilValue) + "\n\n";
  report += "-----------------------------\n\n";

  report += "------ WATER PARAMETERS ------\n\n";
  report += "Water pH Raw: " + String(phValue) + "\n\n";
  report += "Water EC Raw: " + String(ecValue) + "\n\n";

  if (waterLevel == HIGH)
    report += "Water Level: Water Detected\n";
  else
    report += "Water Level: No Water\n";

  report += "\nFlow Rate: " + String(flowRate) + " L/min\n\n";
  report += "-----------------------------\n";

  Serial.println(report);

  LoRa.beginPacket();
  LoRa.print(report);
  LoRa.endPacket();

  Serial.println("LoRa Transmission Done");

  delay(3000);
}
