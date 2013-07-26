// -*- mode: c++ -*-


/*
 File: InstrumentInterface.h
 Description: common interface to vendor-specific functions.
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

#ifndef MZQT_INSTRUMENTINTERFACE_H_
#define MZQT_INSTRUMENTINTERFACE_H_


#include <vector>

#include <QString>
#include <QObject>

#include <mzqt/common/InstrumentInfo.h>

namespace mzqt {

  class Scan;
  class UVScan;

  class InstrumentInterface : public QObject {

  public:

    long totalNumScans_;
    long curScanNum_;
    int firstScanNumber_;
    int lastScanNumber_;

    double startTimeInSec_;
    double endTimeInSec_;
    QString timeStamp_;

    bool doCompression_;
    bool doCentroid_;
    bool doDeisotope_;
    bool shotgunFragmentation_;
    bool lockspray_;
    bool verbose_;
    int accurateMasses_;
    int inaccurateMasses_;
    unsigned int maxUVMSRatio_;
    double maxScanTimeRatio_;
    double maxScanTimeRatioSlope_;
    std::vector<int> chargeCounts_;

    //UV
    long totalNumUVScans_;
    long curUVScanNum_;
    int firstUVScanNumber_;
    int lastUVScanNumber_;

    // used for MassLynx and MassHunter
    std::vector<QString> inputFileNameList_;

    InstrumentInfo instrumentInfo_;

  public:
    InstrumentInterface(void);

    virtual ~InstrumentInterface(void);


    virtual void initInterface(void) = 0;
    virtual bool setInputFile(const QString& fileName) = 0;
    virtual void setCentroiding(bool centroid) = 0;
    virtual void setDeisotoping(bool deisotope) = 0;
    virtual void setCompression(bool compression) = 0;
    virtual void setShotgunFragmentation(bool sf) = 0;
    virtual void setLockspray(bool ls) = 0;
    virtual void setVerbose(bool verbose) = 0;
    virtual Scan* getScan(void) = 0; // returns next available scan (first, initally)
    virtual UVScan *getUVScan(void); // returns next available UV scan (first, initally)
  };

}

inline mzqt::UVScan *mzqt::InstrumentInterface::getUVScan(void)
{
  return NULL;
}

#endif /* MZQT_INSTRUMENTINTERFACE_H_ */
