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
#include <stdlib.h>
#include <unistd.h>

FanController::FanController(int pin, int freq) :
    configurator(pin, freq),
    tempReader(),
    log(configurator.isLogEnabled(), configurator.getLogLevel()),
    previousTemp(tempReader.readTemperatureMC()),
    previousActivePWM(0),
    previousCheckPeriod(configurator.getCheckPeriodMinS()),
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
        if (configurator.isLogEnabled()) log.append("CPU temperature: "+std::to_string(tMC/1000)+"C, Change PWM to: "+std::to_string(activepwm*100/pwmperiod)+"%", 0);
        previousActivePWM = activepwm;
    }
    else
    {
        if (configurator.isLogEnabled()) log.append("CPU temperature: "+std::to_string(tMC/1000)+"C, Keep   PWM to: "+std::to_string(activepwm*100/pwmperiod)+"%", 2);
    }
}

void FanController::delayForNextCheck(int tMC)
{
    const int deltaT = abs(tMC - previousTemp);

    int checkperiod;

    if (deltaT == 0)
    {
        checkperiod = configurator.getCheckPeriodMaxS();
    }
    else
    {
        checkperiod = configurator.getCheckMaxDeltaTempMC() * previousCheckPeriod / deltaT;

        if (checkperiod < configurator.getCheckPeriodMinS())
        {
            checkperiod = configurator.getCheckPeriodMinS();
        }
        else if (checkperiod > configurator.getCheckPeriodMaxS())
        {
            checkperiod = configurator.getCheckPeriodMaxS();
        }

        if (previousCheckPeriod != checkperiod)
        {
            if (configurator.isLogEnabled()) log.append("Updtate temperature check period to: "+std::to_string(checkperiod)+"s", 1);
            previousCheckPeriod = checkperiod;
        }
    }

    previousTemp = tMC;

    sleep(previousCheckPeriod);
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
