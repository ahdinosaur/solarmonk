# solarmonk

energy powered by the sun

safely store energy in a battery

safely use excess load energy to power 5v devices

have fun!

## TODO

- [x] get solar panel
- [x] get battery
- [ ] 12v -> 5v converter
- [ ] read battery charge
- [ ] display battery charge
- [ ] control discharge

## components

### solar panel (with built-in charge controller)

[120w maxray folding solar panel](http://maxray.com.au/folding-solar-panel/)

```
Max Power: 	120W
Maximum Power Tolerance: 	±3%
Open-Circuit Voltage/Voc（V）: 	17.5V
Short-Circuit Current/lsc（A）: 	9.42
Max Power Voltage/Vmp（V）: 	14.0
Max Power Current/lmp（A）: 	8.57
Power Spectications at STC: 	1000w/㎡，AM1.5,CELL25℃
Max System Voltage（V）: 	1000
Max Over Current Protecting Rating（A）: 	15
Weight: 	12.6kg
```

### battery

[12v - 85Ah deep cycle flooded lead acid battery](https://www.trademe.co.nz/Browse/Listing.aspx?id=1506593644)

> _voltage must not drop below 11v_

[guide to lead-acid batteries](http://www.itacanet.org/eng/elec/battery/battery.pdf)

> fully charged is 12.6v to 12.8v

State of Charge (approx.) | 12 Volt Battery | Volts per Cell
100% | 12.70 | 2.12
90% | 12.50 | 2.08
80% | 12.42 | 2.07
70% | 12.32 | 2.05
60% | 12.20 | 2.03
50% | 12.06 | 2.01
40% | 11.90 | 1.98
30% | 11.75 | 1.96
20% | 11.58 | 1.93
10% | 11.31 | 1.89
0 | 10.50 | 1.75

### load (discharge) controller

- BTE14-04 betamcu.cn Arduino Uno R3 Compatible
- "DF Robot" LCD Keypad Shield
- R1 resistor = 10k ohms
- R2 resistor = 820 ohms
- ceramic capacitor = 100nF
- schottkey diode

```

12v   5v      5v
 |     |       |
R1   diode  |-------|
 |     |    |       |
 +-----+----|Ain    |
 |     |    |       |
R2    cap   |arduino|
 |     |    |-------|
GND   GND

```

TODO

### 12v to 5v step-down (buck) converter

[step down dc-dc 9A converter (constant volts or constant amps), 5~40v input & 1.5~35v out](https://www.trademe.co.nz/Browse/Listing.aspx?id=1521227419)

## calculations

### [resistive divider](https://en.wikipedia.org/wiki/Voltage_divider#Resistive_divider)

to measure the battery voltage on the microcontroller, we divide the voltage using resistors:

given

- adc input impedence =  10k ohms
  - https://electronics.stackexchange.com/a/67172
- adc reference voltage = 1.1V
  - https://www.arduino.cc/reference/en/language/functions/analog-io/analogreference/
- max battery voltage = 14V + wiggle room = ~15V

so R1 = adc input impedence = 10k ohms

(want to be as high as possible to reduce current)

calculate

```
R2 = R1 * (1 / ((Vin / Vout) - 1))
   = Rinput_impedence * (1 / ((Vmax_bat / Vref) - 1))
   = 10e3 * (1 / ((15 / 1.1) - 1))
   = 791.37
  ~= 820 ohms
```

### smoothing capacitor

TODO

to stabilize the measurements, we use a capacitor to smooth the values:

```
(1 / (PI * R1 * C)) = ~100 hz or ~10hz (?)
```

C = 100nF
