#include <LiquidCrystal.h>
// variables
String lastLogMessage;
bool isPumpActive = false;
bool manualPumpOn = false;
unsigned long pumpUpdateTime = 0;
long numberOfPumpActivations = 0;
long displayUpdateTime = 0;
unsigned long soilSensorReadTime = 0;

// pins
const byte PUMP_TRIGGER_PIN = 2;
const byte TIME_CLOCK_PIN = 1;
const byte TIME_SERIAL_PIN = 3;
const int ledNumber = 4;
const byte SOIL_SENSOR_POWER_PIN = 13;
const int SOIL_SENSOR_READ_PIN = A0;

const byte MANUAL_PUMP_TRIGGER_PIN = 8;

// constants
unsigned const long WAIT_TIME_MS = 14400000;
unsigned const int ON_TIME_MS = 20000;
unsigned const int SOIL_READ_WAIT_TIME_MS = 1800000;

LiquidCrystal lcd(12, 11, 4, 5, 6, 7);

void setup() {
	lastLogMessage = "";
  pinMode(PUMP_TRIGGER_PIN, OUTPUT);
  pinMode(TIME_CLOCK_PIN, OUTPUT);
  pinMode(TIME_SERIAL_PIN, OUTPUT);
  pinMode(MANUAL_PUMP_TRIGGER_PIN, INPUT);

	pinMode(SOIL_SENSOR_POWER_PIN, OUTPUT);  
  digitalWrite(PUMP_TRIGGER_PIN, LOW);
  digitalWrite(TIME_CLOCK_PIN, LOW);
  digitalWrite(TIME_SERIAL_PIN, LOW);

  lcd.begin(16, 2);

  // Clears the LCD screen
  lcd.clear();
  
  Serial.begin(9200);
}


void loop() 	{
	HandlePump();
	HandleMoistureSensor();
	HandleManualTrigger();
}

void HandleMoistureSensor() {
	if (millis() - soilSensorReadTime > SOIL_READ_WAIT_TIME_MS) {
		ReadSoil();
		soilSensorReadTime = millis();
	}
}

void ReadSoil() {
	LogDebug("reading soil");
	digitalWrite(SOIL_SENSOR_POWER_PIN, HIGH);
	delay(100);
	int moistureValue = analogRead(SOIL_SENSOR_READ_PIN);
	digitalWrite(SOIL_SENSOR_POWER_PIN, LOW);

	LogToFile(String("SoilMoisture,") += String(moistureValue));
}

void HandleManualTrigger() 
{
  if (digitalRead(MANUAL_PUMP_TRIGGER_PIN) == HIGH) {
	LogToFile("ManualPumpOn");
	  manualPumpOn = true;
	digitalWrite(PUMP_TRIGGER_PIN, HIGH);
  } else if (!isPumpActive && manualPumpOn) {
    LogToFile("ManualPumpOff");
	LogDebug("turning off the pump");  
	digitalWrite(PUMP_TRIGGER_PIN, LOW);
	manualPumpOn = false;
  }
}

void HandlePump(){
  // every second
  if (ShouldTurnOnPump() && !isPumpActive) {
    Serial.println("[PumpOn]");
    pumpUpdateTime = millis();
    isPumpActive = true;
    numberOfPumpActivations++;
    digitalWrite(PUMP_TRIGGER_PIN, HIGH);
  }

  if (ShouldTurnOffPump()) {
    Serial.println("[PumpOff]");
    isPumpActive = false;
    pumpUpdateTime = millis();
    digitalWrite(PUMP_TRIGGER_PIN, LOW);
  }

  DisplayPumpInformation();

}

void DisplayPumpInformation() {


  if (millis() - displayUpdateTime > 1000) {
    displayUpdateTime = millis();
  lcd.clear();
      lcd.setCursor(0, 0);
  lcd.print("Count");
  lcd.setCursor(6, 0);
  lcd.print(numberOfPumpActivations);
  lcd.setCursor(0, 1);
  if (isPumpActive) {
    lcd.print("Off in ");
    lcd.setCursor(7, 1);
    lcd.print(getOnTime());
  } else {
    lcd.print("On in ");
    lcd.setCursor(6, 1);
    lcd.print(getWaitTime());
  }
  }
}

long getOnTime() {
	return (ON_TIME_MS - (millis() - pumpUpdateTime)) / 1000;
}

long getWaitTime() {
	return (WAIT_TIME_MS - (millis() - pumpUpdateTime)) / 1000;
}

bool ShouldTurnOnPump() {
  if (millis() - pumpUpdateTime > WAIT_TIME_MS) {
    
    return true;
  }

  return false;
}

bool ShouldTurnOffPump() {
  if (isPumpActive && (millis() - pumpUpdateTime) > (ON_TIME_MS)) {
    return true;
  }
  return false;
}

void LogToFile(String message) {
	if (lastLogMessage != message) {
		Serial.print("[FILE]:");
		Serial.println(message);
		lastLogMessage = message;
	}
}

void LogDebug(String message) {
	Serial.print("[DEBUG]:");
	Serial.println(message);
}


