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

Logger::Logger(bool en) :
    enabled(en)
{
    if (enabled)
    {
        log.open(LOGFILENAME, std::ios_base::app);
        logTime();
        log << "started\n";
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
        logTime();

        log << stringToLog;
        log << '\n';
    }
}

void Logger::clean()
{
    if (enabled)
    {
        log.close();
        log.open(LOGFILENAME, std::ios_base::trunc);
        logTime();
        log << "log file cleaned\n";
    }
}
