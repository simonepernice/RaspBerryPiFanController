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

#include "Logger.h"

#include <ctime>
#include <iostream>
#include <stdio.h>
#include <iomanip>

Logger::Logger(const Configurator&  c) :
    enabled(c.isLogEnabled()),
    logLevel(c.getLogLevel()),
    logLines(0)
{
    if (enabled)
    {
        logLines = countLogFileLines();
        log.open(LOGFILENAME, std::ios_base::app);
        logTime();
        log << "started\n";
        logLines ++;

        log << "Config file found: " ;
        if (c.isThereConfigFile()) log << "yes";
        else log << "no";
        log << '\n';

        if (logLevel >= 5)
        {
            for (int i=0; i<Configurator::N_OF_PARAMETERS; ++ i)
            {
                Configurator::Parameters p = static_cast<Configurator::Parameters>(i);
                log << "Parameter " << c.getParameterName(p);
                if (c.isParameterForced(p))
                {
                    log << " was forced at ";
                }
                else if (c.isParameterDefined(p))
                {
                    log << " was read from config file at ";
                }
                else
                {
                    log << " was set by default value at ";
                }
                log << c.getParameterValue(p) << ' ' << c.getParameterMeasurementUnit(p) << "\n";
            }
        }
    }
}

Logger::~Logger()
{
    if (enabled)
    {
        logTime();
        log << "stopped\n";
        log.close();
    }
}

int Logger::countLogFileLines()
{
    std::ifstream log;
    log.open(LOGFILENAME);

    if (! log.good())
    {
        log.close();
        return 0;
    }

    std::string line;
    int lines = 0;
    while(std::getline(log, line)) lines ++;

    log.close();

    return lines;
}

void Logger::logTime()
{
    time_t now = time(0);
    tm *local_time = localtime(&now);

    char timestr[20];
    strftime(timestr, 20, "%x %X", local_time);

    log << timestr;

    log << " Fan Controller: ";
}

void Logger::append(const std::string& stringToLog, const int level)
{
    if (enabled && level <= logLevel)
    {
        if (logLines > MAXLOGFILELINES )
        {
            makeNewLogFile();
        }

        logTime();

        log << stringToLog;
        log << std::endl;

        logLines ++;
    }
}

bool Logger::makeNewLogFile()
{
    if (enabled)
    {
        log.close();
        if (! rename(LOGFILENAME.c_str(), (LOGFILENAME+".1").c_str()))
        {
            log.open(LOGFILENAME.c_str(), std::ios_base::trunc);

            logTime();
            log << "log file cleaned\n";
            logLines = 1;
            return true;
        }
        else
        {
            log.open(LOGFILENAME, std::ios_base::app);
            return false;
        }
    }
    return false;
}
