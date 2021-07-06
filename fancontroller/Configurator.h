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


#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <set>

#include "libconfig.h++"

class Configurator
{
    public:
        Configurator(int=-1, int=-1);
        virtual ~Configurator();

        int getTempMinMC() const { return tempMinMC; }
        int getTempMaxMC() const { return tempMaxMC; }
        int getDutyCycleMin() const  { return dutyCycleMin; }
        int getDutyCycleMax() const { return dutyCycleMax; }
        int getPWMFrequencyHz() const { return PWMFrequencyHz; }
        int getMaxPowTurnOnTimeMS() const { return maxPowTurnOnTimeMS; }
        int getPinNumber() const { return pinNumber; }
        int getCheckPeriodMaxS() const { return checkPeriodMaxS; }
        int getCheckPeriodMinS() const { return checkPeriodMinS; }
        int getCheckMaxDeltaTempMC() const { return checkMaxDeltaTempMC; }
        bool isLogEnabled() const { return logEnabled; }
        int getLogLevel() const { return logLevel; }

    private:
        const std::string CONFIGFILENAME = "/etc/fancontroller.cfg";

        const bool existsConfigFile;
        libconfig::Config config;
        const libconfig::Setting& setting;


        const int pinNumber;
        const int PWMFrequencyHz;

        const int tempMinMC;
        const int tempMaxMC;
        const int dutyCycleMin;
        const int dutyCycleMax;
        const int maxPowTurnOnTimeMS;
        const int checkPeriodMaxS;
        const int checkPeriodMinS;
        const int checkMaxDeltaTempMC;
        const bool logEnabled;
        const int logLevel;

        bool existsConfigFileTest() const;
        void checkForExtraSettings(const std::set<std::string>&) const;
};

#endif // CONFIGURATOR_H
