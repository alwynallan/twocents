# twocents
$.02 Hardware RNG for IoT

Inspired by discussions on http://www.metzdowd.com/mailman/listinfo/cryptography and having a NodeMCU clone handy I threw together a capacitor and resistor on two free pins.

![Breadboard](/twocents_breadboard.jpg)

The idea is that the capacitor is charged and discharged through the resistor and the voltage is sensed as either a zero or one
on a digital input. The circuit has a time constant of 1ms and the chip can measure time with a resolution of 1/80us so the times
to charge and discharge should contain some entropy.

![Schematic](/twocents_schematic.png)

It appears that the circuit charges fast, usually in one test loop, but discharges more slowly with an variable number of loops. By
checking the time each cycle we gather some mushhhhhhhhhhhhh.

![Timing](/twocents_times.png)

A routine was written that XOR's 32 processor times into a 32-bit register with a different rotation each time.

```c
uint32_t twocents(void){
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
```

The routing runs in about 700us 6/7 of the time and twice that 1/7 of the time. On average 40kbps can be expected. A sample of the resulting data looks full-entropy to me.

![Bits](/twocents.bin.png)

Comments welcome. If this looks half-baked it might be because it started at 6am and it's now 11am.
