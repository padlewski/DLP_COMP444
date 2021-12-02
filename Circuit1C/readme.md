Implementation of circuit 1B from the Sparkfun Inventors Kit (SIK). Their original source code can be found at the [SparkFun Git Repo](https://github.com/sparkfun/SIK-Guide-Code).
View circuit diagram and instructions at: https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v41/circuit-1c-photoresistor

The setup of the first circuit:
* LED (colour doesn't matter) connected to D13 (digital pin 13), this is the same as the onboard LED of the RedBoard, so both will flash.
* A 350 ohm resistor from between the LED ground
* Photoresistor is connected to 5v power (either pin)
* A 10k ohm resister is on the other lead of the photoresistor to the ground
* The negative (ground side) is also wired to analog 0 (A0)

![Wiring Photo][1]

[1]: doc/1C_Wiring.jpg "1C Circuit Wiring"

The original circuit was tested using the code from the [SparkFun git repo](https://github.com/sparkfun/SIK-Guide-Code/blob/master/SIK_Circuit_1C-Photoresistor/SIK_Circuit_1C-Photoresistor.ino) moved into the PlatformIO format.

The additional experiment from the guide was run to treat the photoresistor like a no-touch button. When a hand is waved over the photoresistor, the LED will flash a number of times.

The following video shows the results of the experiment.

[![Potoresistor Experiment](doc/C1C_Intro.png)](https://youtu.be/T8ByeexY0LM "1C Experiment Video")