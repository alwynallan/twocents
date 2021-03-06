#include <ESP8266WiFi.h>
#include <pins_arduino.h>

void setup() {
  Serial.begin(115200);
  Serial.println(); // get a new line
  
  pinMode(D8, INPUT);
  pinMode(D7, OUTPUT);

  //---------------------- test -----------------------------------------------
  #define SAMPLES 10000
  static uint8_t charge_loops[SAMPLES];
  static uint8_t discharge_loops[SAMPLES];

  uint32_t t=ESP.getCycleCount();
  for(register int i=0; i<SAMPLES; i++){
    register uint8_t loops=0;
    digitalWrite(D7, HIGH);
    while(digitalRead(D8) != HIGH) loops++;
    charge_loops[i] = loops;
    //delayMicroseconds(20); // used for testing but not adopted in twocents()
    // 262 ms with delay above, 198,781 bit/s entropy
    // 117 ms without delay,    376,677 bit/s entropy
    loops=0;
    digitalWrite(D7, LOW);
    while(digitalRead(D8) != LOW) loops++;
    discharge_loops[i] = loops;
  }
  t = (ESP.getCycleCount() - t)/80; // us

  for(int i=0; i<SAMPLES; i++){
    Serial.println(String(charge_loops[i]) + '\t' + String(discharge_loops[i]));
  }
  Serial.println(String(SAMPLES) + " samples in " + String(t) + "us");
  //--------------------- end of test -----------------------------------------
}

uint32_t rotateRight(uint32_t value, int count){
  return (value >> count) | (value << (32 - count));
}

uint32_t twocents(void){
  static uint32_t res=0U; // only initializes once
  for(register int i=0; i<32; i++){
    digitalWrite(D7, HIGH);
    while(digitalRead(D8) != HIGH);
    digitalWrite(D7, LOW);
    while(digitalRead(D8) != LOW);
    res ^= rotateRight(ESP.getCycleCount(),i);
  }
  return res;
}

void loop() {
  uint32_t t, r;
  String rs;

  for(int col=0; col<8; col++){
    t=ESP.getCycleCount();
    r=twocents();
    t=(ESP.getCycleCount()-t)/80; // microseconds
    rs = String(r,HEX);
    while(rs.length()<8) rs = '0' + rs;
    Serial.print(rs);
    //Serial.print(' ');
    //Serial.print(t);
  }
  Serial.println();
  while(1) yield();
}
