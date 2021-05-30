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

#include "FanController.h"

#include <wiringPi.h>
#include <softPwm.h>

FanController::FanController() :
    configurator(),
    tempReader(),
    log(configurator.isLogEnabled()),
    previousTemp(tempReader.readTemperatureMC()),
    previousActivePWM(0),
    previousCheclPeriod(configurator.getCheckPeriodMinS()),
    pwmperiod(10000/configurator.getPWMFrequencyHz())
{
    wiringPiSetup();
    softPwmCreate(configurator.getPinNumber(), 0, pwmperiod) ;
}

void FanController::run()
{
    while(true)
    {
        int t = tempReader.readTemperatureMC();
        delay(configurator.getCheckMaxDeltaTempMC());
    }
}

FanController::~FanController()
{
    //dtor
}
