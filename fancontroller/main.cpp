/**
*   fancontroller is a fan controller to run on Raspberry
*   Copyright (C) 2020 Simone Pernice <pernice@libero.it>
*
*   This file is part of fancontroller.
*
*   fancontroller is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   fancontroller is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*/

#include <wiringPi.h>

#include <iostream>
#include <unistd.h>

#include "TempReader.h"
#include "Configurator.h"
#include "FanController.h"


void tempTest ()
{
    TempReader tr;
    for (int i=0; i<10; ++i)
    {
        std::cout << "The temperatue is " << tr.readTemperatureMC() << "\n";
        sleep(1);
    }
}

void configTest ()
{
    Configurator cr;
    std::cout << "The temperature minimum is " << cr.getTempMinMC() << "mC\n";
    std::cout << "The temperature maximum is " << cr.getTempMaxMC() << "mC\n";
    std::cout << "The min duty cycle is " << cr.getDutyCycleMin() << "%\n";
    std::cout << "The max duty cycle is " << cr.getDutyCycleMax() << "%\n";
    std::cout << "The PWM frequency is " << cr.getPWMFrequencyHz() << "Hz\n";
    std::cout << "The max-power turn-on time is " << cr.getMaxPowTurnOnTimeMS() << "ms\n";
    std::cout << "The pin number is " << cr.getPinNumber() << "\n";
    std::cout << "The max check period time is " << cr.getCheckPeriodMaxS() << "ms\n";
    std::cout << "The min check period time is " << cr.getCheckPeriodMinS() << "ms\n";
    std::cout << "The max delta-temperature per check is " << cr.getCheckMaxDeltaTempMC() << "mC\n";
    std::cout << "The log file is enabled " << cr.isLogEnabled() << "\n";
    std::cout << "The log level is " << cr.getLogLevel() << "\n";
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        int pin = -1;
        int freq = -1;
        int dc = -1;
        int st = 1000;
        int tm = 10;
        int c;
        while ((c = getopt(argc, argv, "p:f:d:t:s:")) != -1)
        {
            switch (c)
            {
                case 'p':
                    pin = atoi(optarg);
                    break;
                case 'f':
                    freq = atoi(optarg);
                    break;
                case 'd':
                    dc = atoi(optarg);
                    break;
                case 't':
                    tm = atoi(optarg);
                    break;
                case 's':
                    st = atoi(optarg);
                    break;
                default:
                    std::cout << "Do not provide any option to run in background or set -f frequency, -d dutycycle, -t how long last the test and -p pin to run a test\n";
                    return 1;
            }
        }
        if (pin < 0 || pin > 64 || freq < 1 || freq > 100 || dc < 1 || dc > 100 || st > 10000 || st < 0)
        {
            std::cout << "Missing or not valid pin, frequency or duty cycle\n";
            return 1;
        }

        FanController fanc(pin, freq);
        std::cout << "Pin " << pin << " set at 100% " << " for " << st << " milliseconds" << std::endl;
        fanc.setPWMfromDC(100);
        delay(st);
        std::cout << "Pin " << pin << " set at frequency " << freq << " and duty cycle " << dc << " for " << tm << " seconds" << std::endl;
        fanc.setPWMfromDC(dc);
        sleep(tm);
        std::cout << "Done.\nNow set to zero. " << std::endl;
        fanc.setPWMfromDC(0);
        sleep(1);
        std::cout << "Bye bye.\n";
        return 0;
    }
    else
    {
        FanController f;
        f.run(); //forever
    }
    return 0;
}


