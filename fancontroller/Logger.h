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

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

#include "Configurator.h"

class Logger
{
    public:
        Logger(Configurator);
        virtual ~Logger();
        void append(const std::string&, const int);

    private:
        const bool enabled;
        std::ofstream log;
        const std::string LOGFILENAME = "/var/log/fancontroller.log";
        const int MAXLOGFILELINES = 1000;

        const int logLevel;
        int logLines;
        void logTime();
        int countLogFileLines();
        bool makeNewLogFile();
};

#endif // LOGGER_H
