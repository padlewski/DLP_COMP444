Implementation of circuit 2A from the Sparkfun Inventors Kit (SIK). Their original source code can be found at the [SparkFun Git Repo](https://github.com/sparkfun/SIK-Guide-Code/tree/master/SIK_Circuit_4B-TemperatureSensor).
View circuit diagram and instructions at: https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v41/circuit-4b-temperature-sensor

The setup for the experiments (including the button for experiment 4):
* LCD pin 1 to ground
* LCD pin 2 to 5V 
* LCD pin 3 to potentiometer middle pin
* LCD pin 4 to digital pin 13 
* LCD pin 5 to ground
* LCD pin 6 to digital pin 12
* LCD pin 7 to 10 are not used
* LCD pin 11 to digital pin 11
* LCD pin 12 to digital pin 10
* LCD pin 13 to digital pin 9
* LCD pin 14 to digital pin 8
* LCD pin 15 to 5v
* LCD pin 16 to ground
* Potentiometer pin 1 to ground
* Potentiometer pin 3 to 5v
* Temperature sensor negative to ground
* Temperature sensor positive to 5v
* Temperature sensor signal to analog 0 (A0)

![Wiring Photo][1]

[1]: doc/4B_Wiring.jpg "4B Circuit Wiring"

### Default
The sparkfun code was downloaded and tested. No significant modifications were required for PlatformIO.

### Experiment 1
Swapped Fahrenheit for Kelvin.

### Experiment 2
Display temperature as bar. The will list 5 degrees of celsius and adjust scale if temp changes outside of current scale.

