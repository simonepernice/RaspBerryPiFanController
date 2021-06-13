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

#include <iostream>
#include <unistd.h>

#include "TempReader.h"
#include "Configurator.h"


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
    std::cout << "The max check period time is " << cr.getCheckPeriodMaxMS() << "ms\n";
    std::cout << "The min check period time is " << cr.getCheckPeriodMinMS() << "ms\n";
    std::cout << "The max delta-temperature per check is " << cr.getCheckMaxDeltaTempMC() << "mC\n";
    std::cout << "The log file is enabled " << cr.isLogEnabled() << "\n";
    std::cout << "The log level is " << cr.getLogLever() << "\n";
}

int main()
{
    configTest();
    return 0;
}


