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
    pwmperiod(10000/configurator.getPWMFrequencyHz()),
    pwmminperiod((pwmperiod*configurator.getDutyCycleMin())/100)
{
    wiringPiSetup();
    softPwmCreate(configurator.getPinNumber(), 0, pwmperiod) ;
}

FanController::FanController(int pin, int freq) :
    configurator(pin, freq),
    tempReader(),
    log(configurator.isLogEnabled()),
    previousTemp(tempReader.readTemperatureMC()),
    previousActivePWM(0),
    previousCheclPeriod(configurator.getCheckPeriodMinS()),
    pwmperiod(10000/configurator.getPWMFrequencyHz()),
    pwmminperiod((pwmperiod*configurator.getDutyCycleMin())/100)
{
    wiringPiSetup();
    softPwmCreate(configurator.getPinNumber(), 0, pwmperiod) ;
}

void FanController::setPWMfromTemp(int tMC)
{
    if (tMC < configurator.getTempMinMC())
    {
        softPwmWrite(configurator.getPinNumber(), 0);
    }
    else if (tMC > configurator.getTempMaxMC())
    {
        softPwmWrite(configurator.getPinNumber(), pwmperiod);
    }
    else
    {
        int activeperiod = (tMC - configurator.getTempMinMC()) *  pwmperiod / (configurator.getTempMaxMC() - configurator.getTempMinMC());
        if (activeperiod < pwmminperiod)
        {
            activeperiod = pwmminperiod;
        }
        softPwmWrite(configurator.getPinNumber(), activeperiod);
    }
}
void FanController::setPWMfromDC(int dc)
{
    if (dc < 0 || dc > 100) return;
    softPwmWrite(configurator.getPinNumber(), dc * pwmperiod / 100);
}

void FanController::run()
{
    while(true)
    {
        tempReader.readTemperatureMC();
        delay(configurator.getCheckMaxDeltaTempMC());
    }
}

FanController::~FanController()
{
    //dtor
}
