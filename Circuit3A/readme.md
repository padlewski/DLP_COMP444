Implementation of circuit 2A from the Sparkfun Inventors Kit (SIK). Their original source code can be found at the [SparkFun Git Repo](https://github.com/sparkfun/SIK-Guide-Code/tree/master/SIK_Circuit_3A-Servo).
View circuit diagram and instructions at: https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v41/circuit-3a-servo-motors

The setup for the experiments:
* Servo positive to 5v
* Servo negative to ground 
* Servo signal to PWM 10
* Potentiometer connected to A0

For experiment 3:
* Photoresistor ground to A2
* Photoresistor ground to 10k ohm resistor

![Wiring Photo][1]

[1]: doc/3A_Wiring.jpg "3A Circuit Wiring"

The photo includes the wiring for experiment 3

### Default
The sparkfun code was downloaded and tested. No significant modifications were required for PlatformIO.

### Experiment 1
The last two parameters on the `map` function were reversed to reverse the direction fo the servo motion when the pot was adjusted.

### Experiment 2
The last two parameters of the `map` function were changed for fine control on the servo

### Experiment 3
The photoresistor values was used in place of the pot value for moving the servo

There is no video for this circuit.