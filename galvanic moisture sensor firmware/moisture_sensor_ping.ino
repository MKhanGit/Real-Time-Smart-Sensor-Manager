const int INPUT_PIN = A1;
const int THRESHOLD_PIN = 8;
const int SENSOR_PWR = 4;
const int WET_CALIBRATION = 355;
const int DRY_CALIBRATION = 1023;
const int RNG = DRY_CALIBRATION - WET_CALIBRATION;
const int READ_DELAY = 500; //tweak for stability
char inByte;

void setup() {
  Serial.begin(9600);
  pinMode(INPUT_PIN, INPUT);
  pinMode(THRESHOLD_PIN, INPUT);
  pinMode(SENSOR_PWR, OUTPUT);
  digitalWrite(SENSOR_PWR, LOW);
}

void loop() {
  while (Serial.available()) {
    inByte = Serial.read();
    delay(300);
    if (inByte == 'P') {
      Serial.println(getReading());
    }
    else if (inByte == 'I') {
      //Serial.println("INQ bit");
      Serial.println(digitalRead(THRESHOLD_PIN));
    }
    else if (inByte == 'X') {
      for (int i = 0; i < 10; i++) {
        Serial.println(getReading());
        delay(1000);
      }
    }
    delay(100);
  }

}

int getReading() {
  digitalWrite(SENSOR_PWR, HIGH);
  delay(READ_DELAY);
  int moisture_lvl = map(analogRead(INPUT_PIN), DRY_CALIBRATION, WET_CALIBRATION, 0, 100);
  digitalWrite(SENSOR_PWR, LOW);
  //Serial.println(moisture_lvl);
  //delay(READ_DELAY);
  return moisture_lvl;
}
