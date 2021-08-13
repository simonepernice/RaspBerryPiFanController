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
    {
        for (const auto& subSet : setting)
        {
            if (keywords.find(subSet.getName()) == keywords.end())
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
            defined[i] = setting.exists(PARAMETERNAMES[i]);
        }
    }
    return defined;
}

std::array<int, Configurator::N_OF_PARAMETERS> Configurator::readParametersValue () const
{
    std::array<int, N_OF_PARAMETERS> value;
    for (int i=0; i<N_OF_PARAMETERS; ++i)
    {
        if (parameterForced[i] >= 0)
        {
            value[i] = parameterForced[i] * PARAMETERSCONVERSIONFACTORS[i];
        }
        else
        {
            if (parameterDefined[i])
            {
                value[i] = (int) setting[PARAMETERNAMES[i]] * PARAMETERSCONVERSIONFACTORS[i];
            }
            else
            {
                value[i] = PARAMETERSDEFAULTVALUES[i] * PARAMETERSCONVERSIONFACTORS[i];
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
    parameterForced({pn, fr, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}),
    parameterValue(readParametersValue())
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
            throw std::runtime_error("The parameter "+PARAMETERNAMES[i]+ " is above the maximum allowed value "+std::to_string(PARAMETERSMAXVALUES[i]));
        }
        else if (parameterValue[i] < PARAMETERSMINVALUES[i] * PARAMETERSCONVERSIONFACTORS[i])
        {
            throw std::runtime_error("The parameter "+PARAMETERNAMES[i]+ " is below the minimum allowed value "+std::to_string(PARAMETERSMINVALUES[i]));
        }
    }

    for (int i=0; i<int(sizeof(ORDEREDCOUPLES))/int(sizeof(ORDEREDCOUPLES[0])); ++i)
    {
        if (parameterValue[ORDEREDCOUPLES[i][0]] >= parameterValue[ORDEREDCOUPLES[i][1]])
        {
            throw std::runtime_error("The parameter "+PARAMETERNAMES[ORDEREDCOUPLES[i][0]]+ " is higher than "+PARAMETERNAMES[ORDEREDCOUPLES[i][1]]+" while it should be lower");
        }
    }
}

Configurator::~Configurator()
{
    //dtor
}
