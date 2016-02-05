/*
 File: MassLynxInterface.h
 Description: Encapsulation for Waters MassLynx interface.
 Date: July 25, 2007

 Copyright (C) 2007 Joshua Tasman, ISB Seattle


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

#ifndef MZQT_MASSLYNXINTERFACE_H_
#define MZQT_MASSLYNXINTERFACE_H_

#include <vector>
#include <utility> // for pair
#include <QString>

#include <mzqt/common/InstrumentInterface.h>
#include <mzqt/converters/MassWolf/DACSpectrum.h>
#include <mzqt/converters/MassWolf/DACExScanStats.h>

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

  struct MassLynxScanHeader {

    MZQTDLL_API MassLynxScanHeader();

    int funcNum;
    int scanNum;
    int msLevel;
    long numPeaksInScan;
    float retentionTimeInSec; // in seconds
    float lowMass;
    float highMass;
    float TIC;
    float basePeakMass;
    float basePeakIntensity;
    bool isCalibrated;
    bool skip;
  };

  class MassLynxInterfaceException: public Exception {

  public:
    MZQTDLL_API explicit MassLynxInterfaceException(const std::string &msg = "");
    MZQTDLL_API virtual ~MassLynxInterfaceException() throw ();
  };

  class MassLynxInterface: public InstrumentInterface {

  Q_OBJECT

  signals:
    void scanPreprocessed();
    void uvScanPreprocessed();

  private:
    QString inputFileName_;
    bool verbose_;

    // dll interface stuff here

    bool firstTime_;
    bool firstUVTime_;

    int maxFunctionScan_; //maximum number of scans in a single function
    int totalNumFunctions_;
    std::vector<MSScanType> functionTypes_;

    // Scan information
    std::vector<MassLynxScanHeader> scanHeaderVec_;
    std::vector<MassLynxScanHeader> uvScanHeaderVec_;
    //DAC interfaces that have to be constructed only once
    DACSpectrum spectrum_;
    DACExScanStats exScanStats_;

    int functionFilter_;

    void initUVScan();
    void readFunctionType();

    void preprocessMSFunctions();
    void preprocessUVFunctions();

  public:
    MZQTDLL_API MassLynxInterface(void);
    MZQTDLL_API ~MassLynxInterface(void);

    MZQTDLL_API virtual void initInterface(void);
    MZQTDLL_API bool preprocessFunctions(bool uv);
    MZQTDLL_API virtual bool setInputFile(const QString& fileName);
    MZQTDLL_API virtual void setCentroiding(bool centroid);
    MZQTDLL_API virtual void setDeisotoping(bool deisotope);
    MZQTDLL_API virtual void setCompression(bool compression);
    MZQTDLL_API virtual void setVerbose(bool verbose);
    MZQTDLL_API virtual void setFunctionFilter(int functionNumber);
    MZQTDLL_API virtual Scan* getScan(void);
    MZQTDLL_API virtual UVScan *getUVScan(void);

    MZQTDLL_API virtual void setShotgunFragmentation(bool /*sf*/)
    {
    }

    MZQTDLL_API virtual void setLockspray(bool /*ls*/)
    {
    }


    MZQTDLL_API const MassLynxScanHeader *getCurScanHeader();
    MZQTDLL_API const MassLynxScanHeader *getCurUVScanHeader();
  };

}

#endif
