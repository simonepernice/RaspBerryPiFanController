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


#ifndef FANCONTROLLER_H
#define FANCONTROLLER_H

#include "Configurator.h"
#include "TempReader.h"
#include "Logger.h"

class FanController
{
    public:
        FanController();
        FanController(int, int);
        virtual ~FanController();

        void run();
        void setPWMfromTemp(int);
        void setPWMfromDC(int);


    private:
        const Configurator configurator;
        TempReader tempReader;
        Logger log;
        int previousTemp;
        int previousActivePWM;
        int previousCheclPeriod;
        const int pwmperiod;
        const int pwmminperiod;
};

#endif // FANCONTROLLER_H
