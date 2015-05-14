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

namespace mzqt {

  struct MassLynxScanHeader {

    MassLynxScanHeader();

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
    explicit MassLynxInterfaceException(const std::string &msg = "");
    virtual ~MassLynxInterfaceException() throw ();
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
    MassLynxInterface(void);
    ~MassLynxInterface(void);

    virtual void initInterface(void);
    bool preprocessFunctions(bool uv);
    virtual bool setInputFile(const QString& fileName);
    virtual void setCentroiding(bool centroid);
    virtual void setDeisotoping(bool deisotope);
    virtual void setCompression(bool compression);
    virtual void setVerbose(bool verbose);
    virtual void setFunctionFilter(int functionNumber);
    virtual Scan* getScan(void);
    virtual UVScan *getUVScan(void);

    virtual void setShotgunFragmentation(bool /*sf*/)
    {
    }
    ;
    virtual void setLockspray(bool /*ls*/)
    {
    }
    ;

    const MassLynxScanHeader *getCurScanHeader();
    const MassLynxScanHeader *getCurUVScanHeader();
  };

}

#endif
