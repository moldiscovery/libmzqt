/*
 UVSpectrum.h
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

#ifndef MZQT_UVSPECTRUM_H_
#define MZQT_UVSPECTRUM_H_

#include <iostream>
#include <mzqt/common/UVSPoint.h>

#ifdef __GNUC__
#define MZQTDLL_API
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

    /*! Diode-Array like Spectrum
     */
    class UVSpectrum {

    public:

        typedef UVSPoints Data;

        MZQTDLL_API void clear();

        //! \brief return true if there is no data point
        MZQTDLL_API bool isEmpty() const;

        //! \brief add a peak at the end of the profile
        MZQTDLL_API void addDataPoint(Lambda wavelength, UVSignal signal);
        MZQTDLL_API const Data &getData() const;
        MZQTDLL_API void setData(const Data &data);

    private:

        Data data_; //!< brief the signal for each lambda of the spectrum
    };

    MZQTDLL_API std::ostream &operator <<(std::ostream &os, const UVSpectrum &spectrum);
}


inline const mzqt::UVSpectrum::Data &mzqt::UVSpectrum::getData() const
{
    return data_;
}

inline void mzqt::UVSpectrum::setData(const Data &data)
{
    this->data_ = data;
}

#endif /* MZQT_UVSPECTRUM_H_ */
