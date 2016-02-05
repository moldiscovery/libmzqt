/*
 File: ThermoInterface.h
 Description: Encapsulation for Thermo Xcalibur interface.
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

#ifndef _INCLUDED_MZQT_THERMOINTERFACE_H_
#define _INCLUDED_MZQT_THERMOINTERFACE_H_

#include <string>

#include <mzqt/common/Exception.h>
#include <mzqt/common/InstrumentInterface.h>
#include <mzqt/converters/ReAdW/FilterLine.h>
#include <mzqt/converters/ReAdW/XRawfile.h>

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

  typedef struct _datapeak {
    double dMass;
    double dIntensity;
  } DataPeak;

  class ThermoInterfaceException: public Exception {

  public:
    MZQTDLL_API explicit ThermoInterfaceException(const std::string &msg = "");
    MZQTDLL_API virtual ~ThermoInterfaceException() throw ();
  };


  class ThermoInterface: public InstrumentInterface {
  private:

    XRawfile xrawfile2_;

    int IXRawfileVersion_; // which IXRawfile interface was initialized?


    bool firstTime_;
    bool firstUVTime_;

    void getPrecursorInfo(Scan& scan, long scanNumber, FilterLine& filterLine);
    bool forcePrecursorFromFilter_;

  public:
    int getPreInfoCount_;
    int filterLineCount_;
    int oldAPICount_;


  public:
    MZQTDLL_API ThermoInterface(void);
    MZQTDLL_API ~ThermoInterface(void);

    MZQTDLL_API virtual void initInterface(void);
    MZQTDLL_API void initUVScan();

    MZQTDLL_API void setMSController();
    MZQTDLL_API void setUVController();

    MZQTDLL_API virtual bool setInputFile(const QString& fileName);
    MZQTDLL_API virtual void setCentroiding(bool centroid);
    MZQTDLL_API virtual void setDeisotoping(bool deisotope);
    MZQTDLL_API virtual void setCompression(bool compression);
    MZQTDLL_API virtual void forcePrecursorFromFilter(bool mode);
    MZQTDLL_API virtual void setVerbose(bool verbose);
    MZQTDLL_API virtual void setShotgunFragmentation(bool /*sf*/)
    {
    }
    MZQTDLL_API virtual void setLockspray(bool /*ls*/)
    {
    }
    MZQTDLL_API virtual Scan* getScan(void);
    MZQTDLL_API virtual UVScan* getUVScan(void);
  };

}
#endif
