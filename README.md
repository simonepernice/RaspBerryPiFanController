# Raspberry Pi Fan Controller

*Fan Controller* is designed to drive a PWM fan on a Raspberry Pi at a speed proportional to the CPU temperature. It is designed to use very low resources and be totally configurable.

The latest Rasberry Pi versions are more powerful but they also need a fan to get maximum performance. It is noisy and needs power. The purpose of this application is to run the fan as less as possible using as few Rasberry Pi resources as possible.

Copyright (C) 2020 Simone Pernice <pernice@libero.it>. 

*Fan Controller* is provided under GNU General Public License version 3 as published by the Free Software Foundation.

*Fan Controller* is programmed in C++ for maximum energy efficiency. Other programming languages like Java or Python would have required much more memory and CPU cycles to perform the same task. 
*Fan Controller* has plenty of configuration options to adapt at every condition. 
*Fan Controller* wants to fresh Raspberry while it is executing complex tasks, it does not want to be the reason for heating!!!

*Fan Control* runs as a `systemd` service  in back ground. 

#Introduction

*Fan Controller* drives a fan with a PWM signal on a GPIO pin of Raspberry header. 

*Fan Controller* compares the CPU temperature against a temperature interval. 

* If the CPU temperature is below that interval: the PWM is set to 0% -> no fan noise/power drain
* If the CPU temperature drops within that interval: the PWM duty cycle is set proportionally to the temperature -> minimum acceptable noise/power drain
* If the CPU temperature is above that interval: the PWM is set to maximum -> maximum noise and power drain

*Fan Controller* checks the CPU temperature every once in a while depending on its variation rate. The more stable the temperature the less frequent the check in order to reduce the Raspberry resource requirement. 

* If, since the last check, there was a temperature change below the maximum temperature variation expected -> the longest check time is used
* If, since the last check, there was a temperature change above the maximum temperature variation expected -> the next check time is linearly estimated to reach the maximum temperature variation expected at next read. However, if ths time is shorter than the minimum check rate, it is used instead

*Fan Controller* manages a log file `/var/log/fancontroller.log` where it writes all the actions executed to keep the CPU fresh. Its purpose is to validate the settings, logging can be disabled once the configuration works properly to save computational power.

*Fan Controller* can be lunched with arguments. In that mode, it is used to calibrate fan parameters (control pin, frequency, duty cycle, full power turn on timing). 

*Fan Controller* works as service if lunched without arguments. It is automatically lunched at startup by the `systemd`. In that mode it run the PWM for the fan to keep Raspberry fresh. 

#Install

The quickest way to install *Fan Controller*, is to launch the install.sh script with sudo: sudo ./install.sh

It executes the following tasks:

* copys fancontroller, the binary file, below /usr/bin
* copys fancontroller.cfg, the basic configuration file, below /etc
* copys fancontroller.service, the service configuration file, below /etc/systemd/system
* enables and starts the fancontroller service
	
A better way would be to find the best settings for the system running *Fan Controller* from the command line, providing the arguments to test the fan.
Once the optimal parameters are found, they can be written in ./config/fancontroller.cfg
Eventually the install script can be lunched: sudo ./install.sh
