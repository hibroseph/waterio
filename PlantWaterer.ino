  #include <LiquidCrystal.h>
// variables
bool isPumpActive = false;
unsigned long pumpUpdateTime = 0;
long numberOfPumpActivations = 0;
long displayUpdateTime = 0;

// pins
const byte PUMP_TRIGGER_PIN = 2;
const byte TIME_CLOCK_PIN = 1;
const byte TIME_SERIAL_PIN = 3;
const int ledNumber = 4;

const byte MANUAL_PUMP_TRIGGER_PIN = 8;

// constants
unsigned const long WAIT_TIME_MS = 14400000;
unsigned const long ON_TIME_MS = 20000;

LiquidCrystal lcd(12, 11, 4, 5, 6, 7);

void setup() {
  pinMode(PUMP_TRIGGER_PIN, OUTPUT);
  pinMode(TIME_CLOCK_PIN, OUTPUT);
  pinMode(TIME_SERIAL_PIN, OUTPUT);
  pinMode(MANUAL_PUMP_TRIGGER_PIN, INPUT);

  
  digitalWrite(PUMP_TRIGGER_PIN, LOW);
  digitalWrite(TIME_CLOCK_PIN, LOW);
  digitalWrite(TIME_SERIAL_PIN, LOW);

  lcd.begin(16, 2);

  // Clears the LCD screen
  lcd.clear();
  
  Serial.begin(9200);
}


void loop() {
  HandlePump();

  HandleManualTrigger();
}

void HandleManualTrigger() 
{
  if (digitalRead(MANUAL_PUMP_TRIGGER_PIN) == HIGH) {
    digitalWrite(PUMP_TRIGGER_PIN, HIGH);
  } else {
    digitalWrite(PUMP_TRIGGER_PIN, LOW);
  }
}

void HandlePump(){
  // every second
  if (ShouldTurnOnPump() && !isPumpActive) {
    Serial.print("turning on pump @ ");
    Serial.println(millis());
    pumpUpdateTime = millis();
    isPumpActive = true;
    numberOfPumpActivations++;
    digitalWrite(PUMP_TRIGGER_PIN, HIGH);
  }

  if (ShouldTurnOffPump()) {
    Serial.print("turning off pump @ ");
    Serial.println(millis());
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
    Serial.print("on ");
    Serial.println(((ON_TIME_MS - (millis() - pumpUpdateTime)) / 1000));
    lcd.print((ON_TIME_MS - (millis() - pumpUpdateTime)) / 1000);
  } else {
    lcd.print("On in ");
    lcd.setCursor(6, 1);
    Serial.print("off ");
    Serial.println(((WAIT_TIME_MS - (millis() - pumpUpdateTime)) / 1000));
    lcd.print((WAIT_TIME_MS - (millis() - pumpUpdateTime)) / 1000);
  }
  }
}

bool ShouldTurnOnPump() {
  if (millis() - pumpUpdateTime > WAIT_TIME_MS) {
    
    return true;
  }

  return false;
}

bool ShouldTurnOffPump() {
  if (isPumpActive && (millis() - pumpUpdateTime) > (ON_TIME_MS)) {
    Serial.println("We should turn off pump");
    return true;
  }
  return false;
}
