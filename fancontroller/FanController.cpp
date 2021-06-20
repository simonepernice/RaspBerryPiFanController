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
    log(configurator.isLogEnabled(), configurator.getLogLevel()),
    previousTemp(tempReader.readTemperatureMC()),
    previousActivePWM(0),
    previousCheckPeriod(configurator.getCheckPeriodMinMS()),
    pwmperiod(10000 / configurator.getPWMFrequencyHz()),
    pwmminperiod(pwmperiod * configurator.getDutyCycleMin() / 100)
{
    wiringPiSetup();
    softPwmCreate(configurator.getPinNumber(), 0, pwmperiod) ;
}

FanController::FanController(int pin, int freq) :
    configurator(pin, freq),
    tempReader(),
    log(configurator.isLogEnabled(), configurator.getLogLevel()),
    previousTemp(tempReader.readTemperatureMC()),
    previousActivePWM(0),
    previousCheckPeriod(configurator.getCheckPeriodMinMS()),
    pwmperiod(10000 / configurator.getPWMFrequencyHz()),
    pwmminperiod(pwmperiod * configurator.getDutyCycleMin() / 100)
{
    wiringPiSetup();
    softPwmCreate(configurator.getPinNumber(), 0, pwmperiod) ;
}

void FanController::setPWMfromTemp(int tMC)
{
    int activepwm;
    if (tMC <= configurator.getTempMinMC())
    {
        activepwm = 0;
    }
    else if (tMC >= configurator.getTempMaxMC())
    {
        activepwm = pwmperiod;
    }
    else
    {
        activepwm = (tMC - configurator.getTempMinMC()) *  pwmperiod / (configurator.getTempMaxMC() - configurator.getTempMinMC());
        if (activepwm < pwmminperiod)
        {
            activepwm = pwmminperiod;
        }

        if (previousActivePWM == 0 && configurator.getMaxPowTurnOnTimeMS() > 0)
        {
            softPwmWrite(configurator.getPinNumber(), pwmperiod);
            delay(configurator.getMaxPowTurnOnTimeMS());
        }
    }

    softPwmWrite(configurator.getPinNumber(), activepwm);

    if (previousActivePWM != activepwm)
    {
        log.append("CPU temperature: "+std::to_string(tMC/1000)+"C, Changed PWM to: "+std::to_string(activepwm*100/pwmminperiod)+"%", 0);
        previousActivePWM = activepwm;
    }
    else
    {
        log.append("CPU temperature: "+std::to_string(tMC/1000)+"C, Changed PWM to: "+std::to_string(activepwm*100/pwmminperiod)+"%", 0);
    }
}

void FanController::delayForNextCheck(int tMC)
{
    const int deltaT = tMC - previousTemp;
    if (deltaT != configurator.getCheckMaxDeltaTempMC())
    {
        int checkperiod = configurator.getCheckMaxDeltaTempMC() * previousCheckPeriod / deltaT;

        if (checkperiod < configurator.getCheckPeriodMinMS())
        {
            checkperiod = configurator.getCheckPeriodMinMS();
        }
        else if (checkperiod > configurator.getCheckPeriodMaxMS())
        {
            checkperiod = configurator.getCheckPeriodMaxMS();
        }

        if (previousCheckPeriod != checkperiod)
        {
            log.append("Updtate temperature check period to: "+std::to_string(checkperiod)+"ms", 1);
            previousCheckPeriod = checkperiod;
        }

    }

    previousTemp = tMC;

    delay(previousCheckPeriod);
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
        int t = tempReader.readTemperatureMC();
        setPWMfromTemp(t);
        delayForNextCheck(t);
    }
}

FanController::~FanController()
{
    //dtor
}
