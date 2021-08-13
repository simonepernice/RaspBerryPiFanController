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
                    std::cout << "Do not provide any option to run in background or use the following to test the fan: -f frequency, -d dutycycle, -p pin where the fan is linked, -t seconds for run the test and -s milliseconds to run at 100%.\n";
                    return 1;
            }
        }

        if (pin < 0 || pin > 64 || freq < 1 || freq > 100 || dc < 1 || dc > 100 || st > 10000 || st < 0 || tm < 1 || tm > 100)
        {
            std::cout << "Missing or not valid pin, frequency, duty cycle, start or test time\n";
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
        return 1; //something goes wrong?
    }

    return 0;
}


