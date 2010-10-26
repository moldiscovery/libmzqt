/*
 UVSpoint.h
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

#ifndef MZQT_UVSPOINT_H_
#define MZQT_UVSPOINT_H_

#include <vector>

#include <mzqt/common/UVTypes.h>

namespace mzqt {

    class UVSPoint {

    public:
        UVSPoint(Lambda l = 0, UVSignal s = 0);

        UVSignal getSignal() const;
        Lambda getWavelength() const;
        void setSignal(UVSignal signal);
        void setWavelength(Lambda wavelength);

    private:
        Lambda wavelength_;
        UVSignal signal_;

    };

    typedef std::vector<UVSPoint> UVSPoints;
}

inline mzqt::UVSPoint::UVSPoint(Lambda w, UVSignal s) :
    wavelength_(w), signal_(s)
{
}

inline mzqt::UVSignal mzqt::UVSPoint::getSignal() const
{
    return signal_;
}

inline mzqt::Lambda mzqt::UVSPoint::getWavelength() const
{
    return wavelength_;
}

inline void mzqt::UVSPoint::setSignal(UVSignal signal)
{
    this->signal_ = signal;
}

inline void mzqt::UVSPoint::setWavelength(Lambda wavelength)
{
    this->wavelength_ = wavelength;
}

#endif /* MZQT_UVSPOINT_H_ */
