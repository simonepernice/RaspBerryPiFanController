# Raspberry Pi Fan Controller

*Fan Controller* is designed to drive a fan on a Raspberry Pi.
The latest Rasberry Pi versions are more powerful but they also need a noisy fa. The purpose of this application is to run it as less as possible using as few Rasberry Pi resources as possible.

Copyright (C) 2020 Simone Pernice <pernice@libero.it>. 
*Fan Controller* is provided under GNU General Public License version 3 as published by the Free Software Foundation.

*Fan Controller* is programmed in C++ for maximum energy efficiency. Other programming languages like Java, Python would have required much more memory and CPU cycles to perform the same tasks. *Fan Controller* wants to fresh Raspberry while it is executing complex tasks, it does not want to be the reason for heating!!!

It requires the following libraries:

* [GNU C++ standard library](https://gcc.gnu.org/onlinedocs/libstdc++/)
* [libconfig](https://hyperrealm.github.io/libconfig/) Copyright (C) 2005-2018  Mark A Lindner
* [WiringPi](http://wiringpi.com/) Copyright (C) Gordon Henderson

*Fan Controller* is built mainly with the following tools:

* [Code::Blocks 17.12](www.codeblocks.org) 
* [GNU C++ compiler  6.3.0](https://gcc.gnu.org/) Copyright (C) 2016 Free Software Foundation, Inc.

*Fan Control* runs as a `systemd` service  in back ground. 

#Introduction

At startup, *Fan Controller* reads an optional configuration file present in `/etc/fancontroller.cfg`. 
That configuration file describes all the settings, it is optional, for all the settings not found are used the default values.

*Fan Controller* drives a fan with a PWM signal on a GPIO pin of Raspberry header. 

*Fan Controller* monitors the CPU temperature and compares it against a temperature interval. 
* If the CPU temperature is below that interval, the PWM is set to 0% -> no fan noise
* If the CPU temperature drops within that interval, the PWM is set proportionally to the temperature -> minimum acceptable noise
* If the CPU temperature is above that interval, the PWM is set to 100% -> maximum temperature dissipation 

*Fan Controller* checks the CPU temperature every once in a while depending on its variation rate. There is a maximum temperature variation expected.
* If there was a temperature change exceeding the maximum temperature variation expected -> the maximum check rate is used
* If there was a temperature change lower than the maximum temperature variation expecte -> the check rate is linearly estimated to reach the maximum temperature variation expected. Howeve if ths calculus exceed the minimum check rate, it is used instead

*Fan Controller* manages a log file `/var/log/fancontroller.log` where it writes all the actions executed to keep the CPU fresh. 
Its purpose is to validate the settings, it can be disabled once the configuration is validated to save computational power.

Usually *Fac Controller* is automatically lunched at startup by the `systemd` daemon without any option. However it is possible to lunch with basic option (fan pin, frequency, duty cycle, test time) to begin the fan calibration

#Fan

Usually the fan has 2 wires: positive and negative. It needs a little modification to be controllable by a GPIO. A little NMOS, like IRF7484, should be used to open the ground connection.

* The negative wire, exiting the fan, should be cut and linked to NMOS drain. 
* The NMOS source should go to the black wire going to the Raspberry.
* The NMOS gate should have a big resistor (100k or more) linking to source and another wire going to raspberry GPIO
* A shottky diode with anode on fan negative out and catode on fan positive in, should be added to avoid overvoltages on the NMOS

#Start up option

Once the fan is linked to Raspberry, it is possible to use fan controll to test it.
