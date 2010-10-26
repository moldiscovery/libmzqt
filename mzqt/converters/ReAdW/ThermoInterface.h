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

namespace mzqt {

  typedef struct _datapeak {
    double dMass;
    double dIntensity;
  } DataPeak;

  class ThermoInterfaceException: public Exception {

  public:
    explicit ThermoInterfaceException(const std::string &msg = "");
    virtual ~ThermoInterfaceException() throw ();
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
    ThermoInterface(void);
    ~ThermoInterface(void);

    virtual void initInterface(void);
    void initUVScan();

    void setMSController();
    void setUVController();

    virtual bool setInputFile(const QString& fileName);
    virtual void setCentroiding(bool centroid);
    virtual void setDeisotoping(bool deisotope);
    virtual void setCompression(bool compression);
    virtual void forcePrecursorFromFilter(bool mode);
    virtual void setVerbose(bool verbose);
    virtual void setShotgunFragmentation(bool /*sf*/)
    {
    }
    virtual void setLockspray(bool /*ls*/)
    {
    }
    virtual Scan* getScan(void);
    virtual UVScan* getUVScan(void);
  };

}
#endif
