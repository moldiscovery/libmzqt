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

#if defined(__GNUC__) || defined(MZQT_STATIC)
#ifndef MZQTDLL_API
#define MZQTDLL_API
#endif
#else
#ifdef MZQTDLL_EXPORTS
#ifndef MZQTDLL_API
#define MZQTDLL_API __declspec(dllexport)
#endif
#else
#ifndef MZQTDLL_API
#define MZQTDLL_API __declspec(dllimport)
#endif
#endif
#endif

namespace mzqt {

    class UVScan {

    public:
        MZQTDLL_API UVScan();

        MZQTDLL_API Time getTime() const;
        MZQTDLL_API const UVSpectrum &getSpectrum() const;
        MZQTDLL_API UVSpectrum &getSpectrum();

        MZQTDLL_API void setTime(Time rt);
        MZQTDLL_API void setSpectrum(const UVSpectrum &spectrum);

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
