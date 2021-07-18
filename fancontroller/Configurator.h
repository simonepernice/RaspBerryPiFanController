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
#include <array>

#include "libconfig.h++"

class Configurator
{
    public:
        enum Parameters {PIN, FREQ, TMIN, TMAX, DCMIN, DCMAX, TOTME, CPMIN, CPMAX, DTMAX, LOGEN, LOGLEV, N_OF_PARAMETERS};

        Configurator(int=-1, int=-1);
        virtual ~Configurator();

        int getTempMinMC() const { return parameterValue[TMIN]; }
        int getTempMaxMC() const { return parameterValue[TMAX]; }
        int getDutyCycleMin() const  { return parameterValue[DCMIN]; }
        int getDutyCycleMax() const { return parameterValue[DCMAX]; }
        int getPWMFrequencyHz() const { return parameterValue[FREQ]; }
        int getMaxPowTurnOnTimeMS() const { return parameterValue[TOTME]; }
        int getPinNumber() const { return parameterValue[PIN]; }
        int getCheckPeriodMaxS() const { return parameterValue[CPMAX]; }
        int getCheckPeriodMinS() const { return parameterValue[CPMIN]; }
        int getCheckMaxDeltaTempMC() const { return parameterValue[DTMAX]; }
        bool isLogEnabled() const { return parameterValue[LOGEN]; }
        int getLogLevel() const { return parameterValue[LOGLEV]; }

    private:
        static const std::string PARAMETERNAMES[];
        static const int PARAMETERSCONVERSIONFACTORS[];
        static const int PARAMETERSDEFAULTVALUES[];
        static const int PARAMETERSMINVALUES[];
        static const int PARAMETERSMAXVALUES[];
        static const Parameters ORDEREDCOUPLES[3][2];

        bool existsConfigFileTest() const;
        void checkForExtraSettings(const std::set<std::string>&) const;
        std::array<bool, N_OF_PARAMETERS> readIfParametersDefined () const;
        std::array<int, N_OF_PARAMETERS> readParametersValue (const std::array<int, N_OF_PARAMETERS> &) const;

        const std::string CONFIGFILENAME = "/etc/fancontroller.cfg";

        const bool existsConfigFile;
        libconfig::Config config;
        const libconfig::Setting& setting;

        const std::array<bool, N_OF_PARAMETERS> parameterDefined;
        const std::array<int, N_OF_PARAMETERS> parameterValue;
};

#endif // CONFIGURATOR_H
