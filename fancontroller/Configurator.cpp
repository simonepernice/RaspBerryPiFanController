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


#include "Configurator.h"
#include <fstream>
#include <stdexcept>

bool Configurator::existsConfigFileTest() const
{
    std::ifstream infile(CONFIGFILENAME);
    return infile.good();
}

void Configurator::checkForExtraSettings(const std::set<std::string>& keywords) const
{
    if (existsConfigFile)
        for (const auto& subSet : setting)
            if (keywords.find(std::string(subSet.getName())) == keywords.end())
                throw std::runtime_error(std::string("\nThe following setting was not expected: ").append(subSet.getName()));

}

Configurator::Configurator(int pn, int fr) :
    existsConfigFile(existsConfigFileTest()),
    config(),
    setting(existsConfigFile ? (config.readFile(CONFIGFILENAME.c_str()), config.getRoot()) : config.getRoot()),
    pinNumber(pn > 0 ? pn : (existsConfigFile ? (setting.exists("pinnumber") ? (int) setting["pinnumber"] : 14 ) : 14)),
    PWMFrequencyHz(fr > 0 ? fr : (existsConfigFile ? (setting.exists("pwmfrequencyhz") ? (int) setting["pwmfrequencyhz"] : 10 ) : 10)),
    tempMinMC(existsConfigFile ? (setting.exists("tempminc") ? 1000 * (int) setting["tempminc"] : 45000 ) : 45000),
    tempMaxMC(existsConfigFile ? (setting.exists("tempmaxc") ? 1000 * (int) setting["tempmaxc"] : 60000 ) : 60000),
    dutyCycleMin(existsConfigFile ? (setting.exists("dutycycleminpr") ? (int) setting["dutycycleminpr"] : 20 ) : 20),
    dutyCycleMax(existsConfigFile ? (setting.exists("dutycyclemaxpr") ? (int) setting["dutycyclemaxpr"] : 100 ) : 100),
    maxPowTurnOnTimeMS(existsConfigFile ? (setting.exists("maxpowturnontimems") ? (int) setting["maxpowturnontimems"] : 0 ) : 0),
    checkPeriodMaxS(existsConfigFile ? (setting.exists("checkperiodmaxs") ? (int) setting["checkperiodmaxs"] : 60) : 60),
    checkPeriodMinS(existsConfigFile ? (setting.exists("checkperiodmins") ? (int) setting["checkperiodmins"] : 1) : 1),
    checkMaxDeltaTempMC(existsConfigFile ? (setting.exists("checkmaxdeltatempc") ? 1000 * (int) setting["checkmaxdeltatempc"] : 2000 ) : 2000),
    logEnabled(existsConfigFile ? (setting.exists("logenabled") ? (bool) setting["logenabled"] : false ) : false),
    logLevel(existsConfigFile ? (setting.exists("loglevel") ? (int) setting["loglevel"] : 1 ) : 1)
{
    static const std::set<std::string> keywords{"tempminc", "tempmaxc", "dutycycleminpr", "dutycyclemaxpr", "pwmfrequencyhz", "maxpowturnontimems", "pinnumber", "checkperiodmaxs", "checkperiodmins", "checkmaxdeltatempc", "logenabled", "loglevel"};
    checkForExtraSettings(keywords);

    if (tempMinMC > 100000 || tempMinMC < 0) throw std::runtime_error("Temperature minumum is out of range: allowed from 0 to 100C");
    if (tempMaxMC > 100000 || tempMaxMC < 0) throw std::runtime_error("Temperature maximum is out of range: allowed from 0 to 100C");
    if (tempMinMC > tempMaxMC) throw std::runtime_error("Temperature minimum is higher than maximum");
    if (dutyCycleMin > 100 || dutyCycleMin < 0) throw std::runtime_error("Minumum duty cycle out of range: allowed from 0 to 100%");
    if (dutyCycleMax > 100 || dutyCycleMax < 0) throw std::runtime_error("Maximum duty cycle out of range: allowed from 0 to 100%");
    if (dutyCycleMax < dutyCycleMin) throw std::runtime_error("Maximum duty cycle lower than minimum");
    if (PWMFrequencyHz < 0 || PWMFrequencyHz > 100) throw std::runtime_error("PwM frequency out of range: allowed from 0 to 100Hz");
    if (maxPowTurnOnTimeMS < 0 || maxPowTurnOnTimeMS > 10000) throw std::runtime_error("Max-power turn-on time out of range: allowed from 0 to 10000ms");
    if (pinNumber < 0 || pinNumber > 64) throw std::runtime_error("Pin number out of range: allowed from 0 to 64");
    if (checkPeriodMaxS > 3600 || checkPeriodMaxS < 0) throw std::runtime_error("Maximum check period out of range: allowed from 0 to 3600s ");
    if (checkPeriodMinS > 3600 || checkPeriodMinS < 0) throw std::runtime_error("Minimum check period out of range: allowed from 0 to 3600s");
    if (checkPeriodMaxS < checkPeriodMinS) throw std::runtime_error("Maximum check period lower than minimum");
    if (checkMaxDeltaTempMC > 50000 || checkMaxDeltaTempMC < 0) throw std::runtime_error("Max delta temperature check out of range: allowed from 0 to 50C");
    if (logLevel < 1 || logLevel > 5) throw std::runtime_error("The log detail level is out of range: allowed from 1 to 5");
}

Configurator::~Configurator()
{
    //dtor
}
