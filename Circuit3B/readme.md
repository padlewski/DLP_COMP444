Implementation of circuit 2A from the Sparkfun Inventors Kit (SIK). Their original source code can be found at the [SparkFun Git Repo](https://github.com/sparkfun/SIK-Guide-Code/tree/master/SIK_Circuit_3B-DistanceSensor).
View circuit diagram and instructions at: https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v41/circuit-3b-distance-sensor

The setup for the experiments:
* Servo positive to 5v
* Servo negative to ground 
* Servo signal to PWM 10
* Potentiometer connected to A0

![Wiring Photo][1]

[1]: doc/3B_Wiring.jpg "3B Circuit Wiring"

### Default
The sparkfun code was downloaded and tested. No significant modifications were required for PlatformIO.

### Experiment 1
Changes to the distances which update the light colours.

### Experiment 2
The units are updated from inches to centimeters. The conversion constant was obtained from the [data sheet] (https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) for the distance sensor.

### Experiment 3
Added additional colours to the distance sensor and left the sensor in cm.

There is no video for this circuit.