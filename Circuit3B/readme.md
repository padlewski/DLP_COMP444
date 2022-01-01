Implementation of circuit 2A from the Sparkfun Inventors Kit (SIK). Their original source code can be found at the [SparkFun Git Repo](https://github.com/sparkfun/SIK-Guide-Code/tree/master/SIK_Circuit_3B-DistanceSensor).
View circuit diagram and instructions at: https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v41/circuit-3b-distance-sensor

The setup for the experiments:
* Distance Sensor positive to 5v
* Distance Sensor negative to ground 
* Distance Sensor trigger analog 11 (A11)
* Distance Sensor echo to analog 12 (A12)
* RGB LED red to 350 ohm resistor to analog 3 (A3)
* RGB LED green to 350 ohm resistor to analog 5 (A5)
* RGB LED blue to 350 ohm resistor to analog 6 (A6)
* RFB LED negative to ground

![Wiring Photo][1]

[1]: doc/3B_Wiring.jpg "3B Circuit Wiring"

### Default
The sparkfun code was downloaded and tested. No significant modifications were required for PlatformIO.

### Experiment 1
Changes to the distances which update the light colours.

### Experiment 2
The units are updated from inches to centimeters. The conversion constant was obtained from the [data sheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) for the distance sensor.

### Experiment 3
Added additional colours to the distance sensor and left the sensor in cm.

There is no video for this circuit.