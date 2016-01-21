#include <ESP8266WiFi.h>
#include <pins_arduino.h>

#define MAX_TIMES 32
uint32_t times[MAX_TIMES];

uint32_t rotateRight(uint32_t value, int count){
  return (value >> count) | (value << (32 - count));
}

uint32_t funsa(void){
  uint32_t res=0L;
  
  for(int i=0; i<32; i++){
    digitalWrite(D7, HIGH);
    while(digitalRead(D8) != HIGH);
    digitalWrite(D7, LOW);
    while(digitalRead(D8) != LOW);
    res ^= rotateRight(ESP.getCycleCount(),i);
  }
  return res;
}

void setup() {
  Serial.begin(115200);
  Serial.println(); // get a new line
  //WiFi.begin(SSID, PW);
  pinMode(D8, INPUT);
  pinMode(D7, OUTPUT);

  for(int i=0; i<MAX_TIMES; i++){
    digitalWrite(D7, HIGH);
    while(digitalRead(D8) != HIGH);
    digitalWrite(D7, LOW);
    while(digitalRead(D8) != LOW);
    times[i] = ESP.getCycleCount();
  }

  for(int i=0; i<MAX_TIMES; i++){
    Serial.println(times[i]);
  }
}

void loop() {
  uint32_t t, r;
  String rs;

  for(int col=0; col<8; col++){
    t=ESP.getCycleCount();
    r=funsa();
    t=(ESP.getCycleCount()-t)/80; // microseconds
    rs = String(r,HEX);
    while(rs.length()<8) rs = '0' + rs;
    Serial.print(rs);
    //Serial.print(' ');
    //Serial.print(t);
  }
  Serial.println();
}
