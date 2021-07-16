# RaspBerryPiFanController

*Fan Controller* is designed to drive a PWM fan on a Raspberry Pi proportionally to the CPU temperature. It is designed to use very low resources and be totally configurable.

The latest Rasberry Pi versions are more powerful but they also need a fan. It is noisy and needs power. The purpose of this application is to run the fan as less as possible using as few Rasberry Pi resources as possible.

Copyright (C) 2020 Simone Pernice <pernice@libero.it>. 

*Fan Controller* is provided under GNU General Public License version 3 as published by the Free Software Foundation.

*Fan Controller* is programmed in C++ for maximum energy efficiency. Other programming languages like Java or Python would have required much more memory and CPU cycles to perform the same tasks. 
*Fan Controller* has plenty of configuration options to adapt at every load condition. 
*Fan Controller* wants to fresh Raspberry while it is executing complex tasks, it does not want to be the reason for heating!!!

*Fan Control* runs as a `systemd` service  in back ground. 

#Introduction

*Fan Controller* drives a fan with a PWM signal on a GPIO pin of Raspberry header. 

*Fan Controller* monitors the CPU temperature and compares it against a temperature interval. 

* If the CPU temperature is below that interval: the PWM is set to 0% -> no fan noise/power drain
* If the CPU temperature drops within that interval: the PWM duty cycle is set proportionally to the temperature -> minimum acceptable noise/power drain
* If the CPU temperature is above that interval: the PWM is set to maximum -> maximum noise and power drain

*Fan Controller* checks the CPU temperature every once in a while depending on its variation rate. The more stable the temperature the less frequent the check in order to reduce the Raspberry resource requirement. 

* If there was a temperature change exceeding the maximum temperature variation expected -> the maximum check rate is used
* If there was a temperature change lower than the maximum temperature variation expected -> the check rate is linearly estimated to reach the maximum temperature variation expected at next read. Howeve if ths time is shorter than the minimum check rate, it is used instead

*Fan Controller* manages a log file `/var/log/fancontroller.log` where it writes all the actions executed to keep the CPU fresh. Its purpose is to validate the settings, logging can be disabled once the configuration works properly to save computational power.

*Fan Controller* can be lunched with arguments. In that mode, it is used to calibrate fan parameters (control pin, frequency, duty cycle, full power turn on timing). 

*Fan Controller* works as service if lunched without arguments. It is automatically lunched at startup by the `systemd`. In that mode it run the PWM for the fan to keep Raspberry fresh. 
