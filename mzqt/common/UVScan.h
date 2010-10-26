/*
 UVScan.h
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

#ifndef MZQT_UVSCAN_H_
#define MZQT_UVSCAN_H_

#include <mzqt/common/UVSpectrum.h>

namespace mzqt {

    class UVScan {

    public:
        UVScan();

        Time getTime() const;
        const UVSpectrum &getSpectrum() const;
        UVSpectrum &getSpectrum();

        void setTime(Time rt);
        void setSpectrum(const UVSpectrum &spectrum);

    private:
        Time time_; //!< retention time
        UVSpectrum spectrum_; //!< absorbance spectrum

    };
}

inline mzqt::UVScan::UVScan() :
    time_(-1.0)
{
}

inline mzqt::Time mzqt::UVScan::getTime() const
{
    return time_;
}

inline const mzqt::UVSpectrum &mzqt::UVScan::getSpectrum() const
{
    return spectrum_;
}

inline mzqt::UVSpectrum &mzqt::UVScan::getSpectrum()
{
    return spectrum_;
}

inline void mzqt::UVScan::setTime(Time rt)
{
    this->time_ = rt;
}

inline void mzqt::UVScan::setSpectrum(const UVSpectrum &spectrum)
{
    this->spectrum_ = spectrum;
}

#endif /* UVSCAN_H_ */
