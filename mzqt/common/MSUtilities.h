// -*- mode: c++ -*-


/*
 File: MSUtilities.h
 Description: shared utilities for mzqt-related projects.
 Date: July 25, 2007

 Copyright (C) 2007 Natalie Tasman, ISB Seattle
 Copyright (C) 2010 Lead Molecular Design Sl

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 */

#ifndef MZQT_MSUTILITIES_H_
#define MZQT_MSUTILITIES_H_

#include <string>

std::string convertToURI(const std::string& filename,
        const std::string& hostname);

// ex ID string: urn:lsid:unknown.org:OGDEN_c_thermotest_1min.mzML
std::string convertToIDString(const std::string& filename,
        const std::string& hostname);

std::string toString(long value, int precision = -1);
std::string toString(int value, int precision = -1);
std::string toString(double value, int precision = -1);
std::string toString(float value, int precision = -1);

std::string toUpper(const std::string& str);
std::string toLower(const std::string& str);
double toDouble(const std::string& string);
int toInt(const std::string& string);

#endif
