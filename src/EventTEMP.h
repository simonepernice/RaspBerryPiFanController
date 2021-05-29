/**
*   homelarm is a burglar alarm running on Raspberry
*   Copyright (C) 2020 Simone Pernice <pernice@libero.it>
*
*   This file is part of homelarm.
*
*   homelarm is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   homelarm is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*/

#ifndef EVENTTEMP_H
#define EVENTTEMP_H

#include <fstream>

#include "libconfig.h++"

class EventTEMP : public Event
{
    public:
        EventTEMP(const libconfig::Setting&);

        void checkActive();
        void writeRoots (std::ostream&, int) const;
        void writeDetails(std::ostream& log) const;

    private:
        const int threshold;
        const int hysteresis;
        const int thigh;
        const int tlow;
        const std::string tempfilename;
        std::ifstream tempfile;
        bool out;


};

#endif // EVENTTEMP_H
