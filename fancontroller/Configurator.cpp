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

Configurator::Configurator() :
    existsConfigFile(existsConfigFileTest()),
    config(),
    setting(existsConfigFile ? (config.readFile(CONFIGFILENAME.c_str()), config.getRoot()) : config.getRoot()),
    tempThresholdMC(existsConfigFile ? (setting.exists("tempthresholdc") ? 1000 * (int) setting["tempthresholdc"] : 60000 ) : 60000),
    tempHysteresisMC(existsConfigFile ? (setting.exists("temphysteresisc") ? 1000 * (int) setting["temphysteresisc"] : 10000 ) : 10000),
    dutyCycleMin(existsConfigFile ? (setting.exists("dutycycleminpr") ? (int) setting["dutycycleminpr"] : 20 ) : 20),
    dutyCycleMax(existsConfigFile ? (setting.exists("dutycyclemaxpr") ? (int) setting["dutycyclemaxpr"] : 100 ) : 100),
    PWMFrequencyHz(existsConfigFile ? (setting.exists("pwmfrequencyhz") ? (int) setting["pwmfrequencyhz"] : 10 ) : 10),
    maxPowTurnOnTimeS(existsConfigFile ? (setting.exists("maxpowturnontimes") ? (int) setting["maxpowturnontimes"] : 5 ) : 5),
    pinNumber(existsConfigFile ? (setting.exists("pinnumber") ? (int) setting["pinnumber"] : 14 ) : 14),
    checkPeriodMaxS(existsConfigFile ? (setting.exists("checkperiodmaxs") ? (int) setting["checkperiodmaxs"] : 60 ) : 60),
    checkPeriodMinS(existsConfigFile ? (setting.exists("checkperiodmins") ? (int) setting["checkperiodmins"] : 1 ) : 1),
    checkMaxDeltaTempMC(existsConfigFile ? (setting.exists("checkmaxdeltatempc") ? 1000 * (int) setting["checkmaxdeltatempc"] : 2000 ) : 2000),
    logEnabled(existsConfigFile ? (setting.exists("logenabled") ? 1000 * (bool) setting["logenabled"] : false ) : false)
{
    static const std::set<std::string> keywords{"tempthresholdc", "temphysteresisc", "dutycycleminpr", "dutycyclemaxpr", "pwmfrequencyhz", "maxpowturnontimes", "pinnumber", "checkperiodmaxs", "checkperiodmins", "checkmaxdeltatempc", "logenabled"};
    checkForExtraSettings(keywords);

    if (tempThresholdMC > 100000 || tempThresholdMC < 0) throw std::runtime_error("Temperature out of range: allowed from 0 to 100C");
    if (tempHysteresisMC > 50000 || tempHysteresisMC < 0) throw std::runtime_error("Hysteresis out of range: allowed from 0 to 50C");
    if (dutyCycleMin > 100 || dutyCycleMin < 0) throw std::runtime_error("Minumum duty cycle out of range: allowed from 0 to 100%");
    if (dutyCycleMax > 100 || dutyCycleMax < 0) throw std::runtime_error("Maximum duty cycle out of range: allowed from 0 to 100%");
    if (dutyCycleMax < dutyCycleMin) throw std::runtime_error("Maximum duty cycle lower than minimum");
    if (PWMFrequencyHz < 0 || PWMFrequencyHz > 100) throw std::runtime_error("PwM frequency out of range: allowed from 0 to 100Hz");
    if (maxPowTurnOnTimeS < 0 || maxPowTurnOnTimeS > 600) throw std::runtime_error("Max-power turn-on time out of range: allowed from 0 to 600s");
    if (pinNumber < 0 || pinNumber > 64) throw std::runtime_error("Pin number out of range: allowed from 0 to 64");
    if (checkPeriodMaxS > 3600 || checkPeriodMaxS < 0) throw std::runtime_error("Maximum check period out of range: allowed from 0 to 3600s");
    if (checkPeriodMinS > 3600 || checkPeriodMinS < 0) throw std::runtime_error("Minimum check period out of range: allowed from 0 to 3600s");
    if (checkPeriodMaxS < checkPeriodMinS) throw std::runtime_error("Maximum check period lower than minimum");
    if (checkMaxDeltaTempMC > 50000 || checkMaxDeltaTempMC < 0) throw std::runtime_error("Max delta temperature check out of range: allowed from 0 to 50C");
}

Configurator::~Configurator()
{
    //dtor
}
