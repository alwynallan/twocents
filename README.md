# twocents
$.02 Hardware RNG for IoT. RNGoT?

Inspired by discussions on http://www.metzdowd.com/mailman/listinfo/cryptography and having a NodeMCU clone handy I put together a capacitor and resistor on two free IO pins.

![Breadboard](/twocents_breadboard.jpg)

The idea is that the capacitor is charged and discharged through the resistor and the voltage is sensed as either a low or high
on a digital input. The circuit has a time constant of 1ms and the chip can measure time with a resolution of 1/80us so the times
to charge and discharge should contain some entropy.

![Schematic](/twocents_schematic.png)

It appears that the circuit charges slowly with a hard-to-predict number of test loops, but discharges quickly in 1 or 2 test loops. That is because the storage of the result happens after the discharge, using some time, and there is some overshoot on the low side. A delay can be introduced after the charging to even things out, but it doesn't increase entropy rate. By checking the time each cycle we gather the entropy.

![Timing](/twocents_times.png)

There's no need to subtract the previous time to get the loop time, because the entopy is in the time measurement regardless. A routine was written that XOR's 32 processor times into a 32-bit register with a different rotation each time. The **res** variable is static so it holds the last value between calls, preserving entropy if we're not adding enough. More than 32 loops would be safer, but testing estimates that there are ~4 bits/loop as it is.

```c
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
```

The routine runs in about 700us 6/7 of the time and twice that 1/7 of the time. On average 40kbps can be expected. A sample of the resulting data looks full-entropy.

![Bits](/twocents.bin.png)

Comments are welcome. Remember this is a $.02 RNG for a $2 computer. The type of data security it's intended for is protecting
over-the-air software updates for a thermostat from nefarious hackers who are looking for an army of bitcoin miners.
