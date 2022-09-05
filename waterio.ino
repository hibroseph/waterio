// variables
bool isPumpActive = false;
bool manualPumpOn = false;
unsigned long pumpUpdateTime = 0;
long numberOfPumpActivations = 0;
long displayUpdateTime = 0;
unsigned long soilSensorReadTime = 0;

// pins
const byte PUMP_TRIGGER_PIN = 2;
const byte SOIL_SENSOR_POWER_PIN = 13;
const int SOIL_SENSOR_READ_PIN = A0;

const byte MANUAL_PUMP_TRIGGER_PIN = 8;

// constants
unsigned const long WAIT_TIME_MS = 14400000;
unsigned const int ON_TIME_MS = 20000;
unsigned const int SOIL_READ_WAIT_TIME_MS = 15000/*1800000*/;

void setup() {
		 pinMode(PUMP_TRIGGER_PIN, OUTPUT);
			    pinMode(MANUAL_PUMP_TRIGGER_PIN, INPUT);

					pinMode(SOIL_SENSOR_POWER_PIN, OUTPUT);  
					  digitalWrite(PUMP_TRIGGER_PIN, LOW);

							    Serial.begin(9600);
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
					Serial.print("[FILE]:");
							Serial.println(message);
}

void LogDebug(String message) {
		Serial.print("[DEBUG]:");
			Serial.println(message);
}


