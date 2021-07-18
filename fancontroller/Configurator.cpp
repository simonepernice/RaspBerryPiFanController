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

std::string PARAMETERNAMES[] =      {"pinnumber",   "pwmfreqyencyhz",   "tempminc",     "tempmaxc", "dutycycleminpr", "dutycyclemaxpr",     "maxpowturnontimems", "checkperiodmins",    "checkperiodmaxs",  "checkmaxdeltatempc", "logenabled",     "loglevel"  };
int PARAMETERSCONVERSIONFACTORS[] = {1,             1,                  1000,           1000,       1,                  1,                  1,                      1,                  1,                  1000,                   1,              1           };
int PARAMETERSDEFAULTVALUES[] =     {14,            10,                 45,             60,         20,                 100,                500,                    1,                  60,                 2,                      0,              1           };
int PARAMETERSMINVALUES[] =         {0,             1,                  0,              0,          0,                  0,                  0,                      1,                  1,                  0,                      0,              0           };
int PARAMETERSMAXVALUES[] =         {64,            200,                100,            100,        100,                100,                10000,                  3600,               3600,               20,                     1,              5           };

Configurator::Parameters ORDEREDCOUPLES[3][2] = {{Configurator::TMIN, Configurator::TMAX}, {Configurator::DCMIN, Configurator::DCMAX}, {Configurator::CPMIN, Configurator::CPMAX}};

bool Configurator::existsConfigFileTest() const
{
    std::ifstream infile(CONFIGFILENAME);
    return infile.good();
}

void Configurator::checkForExtraSettings(const std::set<std::string>& keywords) const
{
    if (existsConfigFile)
    {
        for (const auto& subSet : setting)
        {
            if (keywords.find(std::string(subSet.getName())) == keywords.end())
            {
                throw std::runtime_error(std::string("\nThe following setting was not expected: ").append(subSet.getName()));
            }
        }
    }
}

std::array<bool, Configurator::N_OF_PARAMETERS> Configurator::readIfParametersDefined () const
{
    std::array<bool, N_OF_PARAMETERS> defined;
    for (int i=0; i<N_OF_PARAMETERS; ++i)
    {
        if (!existsConfigFile)
        {
            defined[i] = false;
        }
        else
        {
            defined[i] = setting.exists(PARAMETERNAMES[static_cast<Parameters>(i)]);
        }
    }
    return defined;
}

std::array<int, Configurator::N_OF_PARAMETERS> Configurator::readParametersValue (const std::array<int, N_OF_PARAMETERS> & forced) const
{
    std::array<int, N_OF_PARAMETERS> value;
    for (int i=0; i<N_OF_PARAMETERS; ++i)
    {
        if (parameterDefined[static_cast<Parameters>(i)])
        {
            value[i] = (int) setting[PARAMETERNAMES[static_cast<Parameters>(i)]] * PARAMETERSCONVERSIONFACTORS[static_cast<Parameters>(i)];
        }
        else
        {
            if (forced[i] < 0)
            {
                value[i] = PARAMETERSDEFAULTVALUES[static_cast<Parameters>(i)] * PARAMETERSCONVERSIONFACTORS[static_cast<Parameters>(i)];
            }
            else
            {
                value[i] = forced[i];
            }
        }
    }
    return value;
}

Configurator::Configurator(int pn, int fr) :
    existsConfigFile(existsConfigFileTest()),
    config(),
    setting(existsConfigFile ? (config.readFile(CONFIGFILENAME.c_str()), config.getRoot()) : config.getRoot()),
    parameterDefined(readIfParametersDefined()),
    parameterValue(readParametersValue({pn, fr, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}))
{
    std::set<std::string> keywords;
    for (int i=0; i<N_OF_PARAMETERS; ++i)
    {
        keywords.insert(PARAMETERNAMES[i]);
    }
    checkForExtraSettings(keywords);

    for (int i=0; i<N_OF_PARAMETERS; ++i)
    {
        if (parameterValue[i] > PARAMETERSMAXVALUES[i] * PARAMETERSCONVERSIONFACTORS[i])
        {
            throw std::runtime_error("The parameter "+PARAMETERNAMES[i]+ " exceeds the maximum allowed value "+std::to_string(PARAMETERSMAXVALUES[i]));
        }
        else if (parameterValue[i] < PARAMETERSMINVALUES[i] * PARAMETERSCONVERSIONFACTORS[i])
        {
            throw std::runtime_error("The parameter "+PARAMETERNAMES[i]+ " is below the minimum allowed value "+std::to_string(PARAMETERSMINVALUES[i]));
        }
    }

    for (int i=0; i<int(sizeof(ORDEREDCOUPLES)/sizeof(ORDEREDCOUPLES[0])); ++i)
    {
        if (parameterValue[ORDEREDCOUPLES[i][0]] >= parameterValue[ORDEREDCOUPLES[i][1]])
        {
            throw std::runtime_error("The parameter "+PARAMETERNAMES[ORDEREDCOUPLES[i][0]]+ " exceeds "+PARAMETERNAMES[ORDEREDCOUPLES[i][1]]+" while it should be below");
        }
    }
}

Configurator::~Configurator()
{
    //dtor
}
