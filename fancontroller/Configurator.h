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

        const std::string& getParameterName(Parameters p) const {return PARAMETERNAMES[p];}
        const std::string& getParameterMeasurementUnit(Parameters p) const {return PARAMETERMEASUNIT[p];}
        int getParameterValue(Parameters p) const {return parameterValue[p]/PARAMETERSCONVERSIONFACTORS[p];}
        bool isParameterDefined(Parameters p) const {return parameterDefined[p];}
        bool isParameterForced(Parameters p) const {return parameterForced[p]>=0;}
        bool isThereConfigFile() const {return existsConfigFile;}

    private:
        const std::string PARAMETERNAMES[12] =      {"pinnumber",   "pwmfrequencyhz",   "tempminc",     "tempmaxc", "dutycycleminpr",   "dutycyclemaxpr",   "maxpowturnontimems",  "checkperiodmins",    "checkperiodmaxs",  "checkmaxdeltatempc",   "logenabled",   "loglevel"  };
        const std::string PARAMETERMEASUNIT[12] =   {"#",           "Hz",               "C",            "C",        "%",                "%",                "ms",                  "s",                  "s",                "C",                    "",             ""          };
        const int PARAMETERSCONVERSIONFACTORS[12] = {1,             1,                  1000,           1000,       1,                  1,                  1,                      1,                  1,                  1000,                   1,              1           };
        const int PARAMETERSDEFAULTVALUES[12] =     {14,            10,                 45,             60,         20,                 100,                500,                    1,                  60,                 2,                      1,              0           };
        const int PARAMETERSMINVALUES[12] =         {0,             1,                  0,              0,          0,                  0,                  0,                      1,                  1,                  0,                      0,              0           };
        const int PARAMETERSMAXVALUES[12] =         {64,            200,                100,            100,        100,                100,                10000,                  3600,               3600,               30,                     1,              5           };

        const Parameters ORDEREDCOUPLES[3][2] = {{Configurator::TMIN, Configurator::TMAX}, {Configurator::DCMIN, Configurator::DCMAX}, {Configurator::CPMIN, Configurator::CPMAX}};

        bool existsConfigFileTest() const;
        void checkForExtraSettings(const std::set<std::string>&) const;
        std::array<bool, N_OF_PARAMETERS> readIfParametersDefined () const;
        std::array<int, N_OF_PARAMETERS> readParametersValue () const;

        const std::string CONFIGFILENAME = "/etc/fancontroller.cfg";

        const bool existsConfigFile;
        libconfig::Config config;
        const libconfig::Setting& setting;

        const std::array<bool, N_OF_PARAMETERS> parameterDefined;
        const std::array<int, N_OF_PARAMETERS> parameterForced;
        const std::array<int, N_OF_PARAMETERS> parameterValue;
};

#endif // CONFIGURATOR_H
