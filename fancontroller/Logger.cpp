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

Logger::Logger(bool en) :
    enabled(en),
    logLines(0)
{
    if (enabled)
    {
        logLines = countLogFileLines();
        log.open(LOGFILENAME, std::ios_base::app);
        logTime();
        log << "started\n";
        logLines ++;
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
    log << ctime(&now);
    log << " Fan Controller: ";
}

void Logger::append(const std::string& stringToLog)
{
    if (enabled)
    {
        if (logLines > MAXLOGFILELINES )
        {
            makeNewLogFile();
        }

        logTime();

        log << stringToLog;
        log << '\n';

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
}
