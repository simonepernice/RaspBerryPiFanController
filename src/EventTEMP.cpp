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

#include "Event.h"
#include "EventTEMP.h"

EventTEMP::EventTEMP(const libconfig::Setting& e):
    Event(e),
    threshold(e["type"]["threshold"]),
    hysteresis(e["type"].exists("hysteresis") ? e["type"]["hysteresis"] : 2),
    thigh((threshold + hysteresis/2)*1000),
    tlow ((threshold - hysteresis/2)*1000),
    tempfilename(e["type"].exists("tmpfilename") ? e["type"]["tmpflename"].c_str() : "/sys/class/thermal/thermal_zone0/temp"),
    tempfile(tempfilename),
    out (false)
{
    type = "Event TEMP";

    if (! tempfile) throw libconfig::SettingException(e["type"], "\nIt was not possible to read the temperature file");

    int t;
    tempfile >> t;
    if (! t) throw libconfig::SettingException(e["type"], "\nThe temperature file does not contain numbers");

    static const std::set<std::string> keywords{"link", "threshold", "hysteresis", "tmpfilename"};
    checkForExtraSettings(keywords, e["type"]);
}

void EventTEMP::checkActive()
{
    tempfile.clear();
    tempfile.seekg(0);

    int t;
    tempfile >> t;

    if(out)
    {
        if (t <= tlow) out = false;
    }
    else
    {
        if (t >= thigh) out = true;
    }

    setActiveFiltered(out);
}

void EventTEMP::writeRoots (std::ostream& log, int depth) const
{
    Event::writeRoots(log, depth);
    writeSpaces(log, depth);
    log << "TEMP,T" << threshold << "H" << hysteresis;
}

void EventTEMP::writeDetails(std::ostream& log) const
{
    Event::writeDetails(log);
    log << "| temp threshold: " << threshold;
    log << "| temp hiseresys: " <<  hysteresis;
    log << "| temp file: " << tempfilename;
}
