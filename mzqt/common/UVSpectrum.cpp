/*
 UVSpectrum.cpp
 Created on: 06/08/2010
 Author: Fabien Fontaine
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

#include "UVSpectrum.h"

#ifdef USE_MMGR_MEMORY_CHECK
#include <mmgr.h>
#endif

using namespace mzqt;

void UVSpectrum::clear()
{
    *this = UVSpectrum();
}

bool UVSpectrum::isEmpty() const
{
    return data_.size() == 0;
}

void UVSpectrum::addDataPoint(Lambda wavelength, UVSignal signal)
{
    data_.push_back(UVSPoint(wavelength, signal));
}

std::ostream &mzqt::operator <<(std::ostream &os, const UVSpectrum &spectrum)
{
    os << "wavelength and signal:" << std::endl;

    typedef UVSpectrum::Data Data;
    const Data &data = spectrum.getData();
    Data::const_iterator it = data.begin();
    for (; it != data.end(); ++it) {
        os << (*it).getWavelength() << " " << (*it).getSignal() << std::endl;
    }

    return os;
}

