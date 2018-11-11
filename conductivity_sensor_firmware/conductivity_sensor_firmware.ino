/* Author: Mohsin Khan
Reads raw voltage output from a galvanic sensor. Analog voltage scales directly with conductivity of the media.

*/

const int INPUT_PIN = A1;
const int THRESHOLD_PIN = 8;
const int SENSOR_PWR = 4;
const int WET_CALIBRATION = 355;
const int DRY_CALIBRATION = 1023;
const int READ_DELAY = 500; //tweak for stability
char inByte;
int STB_DLY = 300;
int avg_filter[] = {0,0,0,0,0,0,0,0,0,0};
int avg_size = sizeof(avg_filter) / sizeof(avg_filter[0]);
int s;
//Setup Serial BT Commands
const char CMD_INQUIRE_RAW = 'I';
const char CMD_INQUIRE_DIGITAL = 'D';
const char CMD_PING = 'P';
const char CMD_AVG_10 = 'A';
const char CMD_AVG_10_RAW = 'R';

void setup() {
  Serial.begin(9600);
  pinMode(INPUT_PIN, INPUT);  //Analog Input 
  pinMode(THRESHOLD_PIN, INPUT);  //Digital Input. This goes high/low based on a physical trim pot on the sensor
  pinMode(SENSOR_PWR, OUTPUT);  //Prevent anode decay by only powering the circuit during a reading.
  digitalWrite(SENSOR_PWR, LOW);  //initally power down sensor (see note above)
}

void loop() {
  while (Serial.available()) {
    inByte = Serial.read();
    delay(STB_DLY);

      switch (inByte) {
        case CMD_PING:
          Serial.println(getReading());
          break;
          
        case CMD_INQUIRE_DIGITAL:
          Serial.println(getDigitalReading());
          break;
          
        case CMD_INQUIRE_RAW:
          Serial.println(getRawReading());
          break;
          
        case CMD_AVG_10:
          for (int i = 0; i < avg_size; i++) {
            avg_filter[i] = getReading();
            delay(1000);
          }
          s = 0;
          for (int i=0; i<avg_size; i++){
              s += avg_filter[i];
          }
          
          s = s / avg_size;
          Serial.println(s);
          break;
          
        case CMD_AVG_10_RAW:
          for (int i = 0; i < avg_size; i++) {
            avg_filter[i] = getRawReading();
            delay(1000);
          }
          
          s = 0;
          for (int i=0; i<avg_size; i++){
              s += avg_filter[i];
          }
          
          s = s / avg_size;
          Serial.println(s);
          break;
                              
        default:
          break;                                
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

int getRawReading() {
  digitalWrite(SENSOR_PWR, HIGH);
  delay(READ_DELAY);
  int moisture_lvl = analogRead(INPUT_PIN);
  digitalWrite(SENSOR_PWR, LOW);
  //Serial.println(moisture_lvl);
  //delay(READ_DELAY);
  return moisture_lvl;
}

int getDigitalReading() {
  digitalWrite(SENSOR_PWR, HIGH);
  delay(READ_DELAY);
  int moisture_lvl = digitalRead(THRESHOLD_PIN);
  digitalWrite(SENSOR_PWR, LOW);
  //Serial.println(moisture_lvl);
  //delay(READ_DELAY);
  return moisture_lvl;
}
