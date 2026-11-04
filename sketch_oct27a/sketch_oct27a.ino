
#define BLYNK_TEMPLATE_ID "ID"
#define BLYNK_TEMPLATE_NAME "Smart Plant Monitoring"
#define BLYNK_AUTH_TOKEN "Token"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "Wifi Name";
char pass[] = "Wifi Password";


// ---------------- SENSOR PINS ----------------

#define SOIL_PIN 34
#define DHT_PIN 4
#define WATER_DEPTH_PIN 35

#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

BlynkTimer timer;

// ---------------- RELAY PINS ----------------

#define RELAY_IN 26
#define RELAY_OUT 27

// Active LOW relay module
#define RELAY_ON LOW
#define RELAY_OFF HIGH

// ---------------- SOIL CALIBRATION ----------------

// Adjust using actual dry and wet sensor readings
const int DRY_RAW = 3500;
const int WET_RAW = 1500;

// Soil automation thresholds
const int SOIL_START = 50;
const int SOIL_STOP = 80;

// ---------------- WATER DEPTH CALIBRATION ----------------

// Example values only.
// Replace these with your actual sensor readings
// at empty and full water levels.

const int WATER_EMPTY_RAW = 0;
const int WATER_FULL_RAW = 1900;

// Set true if raw reading increases as water rises.
// Set false if raw reading decreases as water rises.
const bool WATER_RAW_INCREASES = true;

// Water OUT pump thresholds
const int WATER_OUT_START = 80;
const int WATER_OUT_STOP = 60;

// ---------------- TEMPERATURE SETTINGS ----------------

const float LOW_TEMP = 20.0;
const float HIGH_TEMP = 35.0;

const unsigned long ALERT_COOLDOWN = 600000UL;

unsigned long lastLowAlert = 0;
bool lowAlertSent = false;

// ---------------- PUMP STATES ----------------

bool pumpIn = false;
bool pumpOut = false;

bool autoWaterIn = false;
bool autoWaterOut = false;

bool manualWaterIn = false;

// ---------------- SENSOR VALUES ----------------

int soilRaw = 0;
int soilPercent = 0;

int waterRaw = 0;
int waterPercent = 0;

float temperature = 0;
float humidity = 0;

// ---------------- AVERAGE SENSOR READING ----------------

int readAverage(int pin) {

  long total = 0;

  for (int i = 0; i < 10; i++) {
    total += analogRead(pin);
    delay(5);
  }

  return total / 10;
}

// ---------------- UPDATE RELAYS ----------------

void updateRelays() {

  // Never run both pumps simultaneously
  if (pumpOut) {
    pumpIn = false;
  }

  digitalWrite(
    RELAY_IN,
    pumpIn ? RELAY_ON : RELAY_OFF
  );

  digitalWrite(
    RELAY_OUT,
    pumpOut ? RELAY_ON : RELAY_OFF
  );
}

// ---------------- SOIL MOISTURE ----------------

void readSoilMoisture() {

  soilRaw = readAverage(SOIL_PIN);

  soilPercent = map(
    soilRaw,
    DRY_RAW,
    WET_RAW,
    0,
    100
  );

  soilPercent = constrain(soilPercent, 0, 100);
}

// ---------------- WATER DEPTH ----------------

void readWaterDepth() {

  waterRaw = readAverage(WATER_DEPTH_PIN);

  int emptyValue;
  int fullValue;

  if (WATER_RAW_INCREASES) {
    emptyValue = WATER_EMPTY_RAW;
    fullValue = WATER_FULL_RAW;
  }
  else {
    emptyValue = WATER_FULL_RAW;
    fullValue = WATER_EMPTY_RAW;
  }

  if (emptyValue != fullValue) {

    waterPercent = map(
      waterRaw,
      emptyValue,
      fullValue,
      0,
      100
    );

    waterPercent = constrain(
      waterPercent,
      0,
      100
    );
  }
}

// ---------------- AUTOMATIC PUMP CONTROL ----------------

void controlPumps() {

  readSoilMoisture();
  readWaterDepth();

  // AUTOMATIC WATER IN
  if (soilPercent < SOIL_START) {
    autoWaterIn = true;
  }

  if (soilPercent >= SOIL_STOP) {
    autoWaterIn = false;
  }

  // AUTOMATIC WATER OUT
  if (waterPercent >= WATER_OUT_START) {
    autoWaterOut = true;
  }

  if (waterPercent <= WATER_OUT_STOP) {
    autoWaterOut = false;
  }

  // Water OUT has priority
  if (autoWaterOut) {

    pumpOut = true;
    pumpIn = false;

  }
  else {

    pumpOut = false;

    // Automatic or manual Water IN
    pumpIn = autoWaterIn || manualWaterIn;

  }

  updateRelays();

  // Serial Monitor
  Serial.println("\n--------------------------");

  Serial.print("Soil Raw: ");
  Serial.println(soilRaw);

  Serial.print("Soil Moisture: ");
  Serial.print(soilPercent);
  Serial.println("%");

  Serial.print("Water Raw: ");
  Serial.println(waterRaw);

  Serial.print("Water Depth: ");
  Serial.print(waterPercent);
  Serial.println("%");

  Serial.print("Water IN Pump: ");
  Serial.println(pumpIn ? "ON" : "OFF");

  Serial.print("Water OUT Pump: ");
  Serial.println(pumpOut ? "ON" : "OFF");

  Serial.println("--------------------------");
}

// ---------------- BLYNK MANUAL BUTTON ----------------

// V4 controls Water IN manually
BLYNK_WRITE(V4) {

  manualWaterIn = param.asInt();

  Serial.println(
    manualWaterIn ?
    "Manual Water IN requested" :
    "Manual Water IN stopped"
  );

  // Update immediately, including current safety state
  controlPumps();
}

// ---------------- BLYNK CONNECTION ----------------

BLYNK_CONNECTED() {

  manualWaterIn = false;

  autoWaterIn = false;
  autoWaterOut = false;

  pumpIn = false;
  pumpOut = false;

  updateRelays();

  Blynk.virtualWrite(V4, 0);

  Serial.println("Blynk Connected");
}

// ---------------- SEND SENSOR DATA ----------------

void sendSensorData() {

  // Send soil and water percentage
  Blynk.virtualWrite(V1, soilPercent);
  Blynk.virtualWrite(V6, waterPercent);

  // Read DHT22
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {

    Serial.println("DHT22 reading failed!");

    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, humidity);

  // TEMPERATURE ALERT SYSTEM

  if (temperature < LOW_TEMP) {

    Blynk.virtualWrite(V8, "LOW TEMPERATURE");

    unsigned long now = millis();

    if (
      !lowAlertSent ||
      (now - lastLowAlert >= ALERT_COOLDOWN)
    ) {

      Blynk.logEvent(
        "low_temperature",
        "Alert! Temperature is below 20 C."
      );

      lastLowAlert = now;
      lowAlertSent = true;
    }

  }

  else if (temperature > HIGH_TEMP) {

    Blynk.virtualWrite(V8, "HIGH TEMPERATURE");

    lowAlertSent = false;

  }

  else {

    Blynk.virtualWrite(V8, "NORMAL");

    lowAlertSent = false;
  }
}

// ---------------- SETUP ----------------

void setup() {

  Serial.begin(115200);

  analogReadResolution(12);

  pinMode(RELAY_IN, OUTPUT);
  pinMode(RELAY_OUT, OUTPUT);

  // Both pumps OFF initially
  digitalWrite(RELAY_IN, RELAY_OFF);
  digitalWrite(RELAY_OUT, RELAY_OFF);

  dht.begin();

  Blynk.begin(
    BLYNK_AUTH_TOKEN,
    ssid,
    pass
  );

  // Sensor and pump control every 2 seconds
  timer.setInterval(2000L, controlPumps);

  // Blynk sensor updates every 15 seconds
  timer.setInterval(15000L, sendSensorData);

  Serial.println("Smart Plant Monitoring Started!");
}

// ---------------- MAIN LOOP ----------------

void loop() {

  Blynk.run();

  timer.run();
}
