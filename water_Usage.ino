#include <ThingerESP8266.h>
#include <ESP8266WiFi.h>
//WiFiClient client;
#define USERNAME "nabilpatel4012"
#define DEVICE_ID "NodeMCU"
#define DEVICE_CREDENTIAL "y@eNBEWUjY1api40"
#define SSID "Connecting"
#define SSID_PASSWORD "12345678"
#define SENSOR  D3
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
//boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}
void setup()
{
  Serial.begin(115200);
  thing.add_wifi(SSID, SSID_PASSWORD);
  pinMode(SENSOR, INPUT_PULLUP);
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
}
void loop()
{
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    pulse1Sec = pulseCount;
    pulseCount = 0;
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space
    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(totalMilliLitres / 1000);
    Serial.println("L");
    thing["data"] >> [](pson& out){
    out["flow_Rate"] = flowRate;
    out["total"]= totalMilliLitres;
     };
    thing.handle();
    thing.stream(thing["data"]);
  }
}
