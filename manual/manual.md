# Raspberry Pi Fan Controller

*Fan Controller* is designed to drive a fan on a Raspberry Pi.
The latest rasberry needs a fan and it is very noisy. The purpose of this application is to run it as less as possible.



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

*Fan Control* runs as a `systemd` service  in back ground. At startup it reads an optional configuration file present in /etc/fancontroller.cfg. That configuration file describes all the settings, it is optional, all the settings not found are used the default values.

