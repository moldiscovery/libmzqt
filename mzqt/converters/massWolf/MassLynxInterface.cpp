/*
 File: MassLynxInterface.cpp
 Description: Encapsulation for Waters MassLynx interface.
 Date: July 25, 2007

 Copyright (C) 2007 Joshua Tasman, ISB Seattle
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

#define _CRT_SECURE_NO_WARNINGS

#include <cassert>
#include <cmath>

#include <QString>
#include <QDir>
#include <QTime>
#include <QFileInfo>

#include <iostream>
#include <algorithm>

#include "MassLynxInterface.h"
#include <mzqt/common/Debug.h>
#include <mzqt/common/Scan.h>
#include <mzqt/common/UVScan.h>
#include <mzqt/common/MSUtilities.h>

#include "DACProcessInfo.h"
#include "DACFunctionInfo.h"
#include "DACScanStats.h"
#include "DACHeader.h"

#ifdef USE_MMGR_MEMORY_CHECK
#include <mmgr.h>
#endif

using namespace std;
using namespace mzqt;

#if 0
// function to sort MassLynxScanHeader by retention Time
struct RetentionSorter {
  bool operator()(const MassLynxScanHeader& a, const MassLynxScanHeader& b)
  {
    return a.retentionTimeInSec < b.retentionTimeInSec;
  }
};
#endif

typedef std::pair<float, float> floatfloatpair;
typedef std::vector<floatfloatpair> floatfloatpairVec;

MassLynxInterfaceException::MassLynxInterfaceException(const std::string &msg) :
  Exception(msg)
{
  msg_ = "MASSLYNX INTERFACE ERROR(" + msg + ")";
}

MassLynxInterfaceException::~MassLynxInterfaceException() throw ()
{
}

MassLynxScanHeader::MassLynxScanHeader() :
  funcNum(-1), scanNum(-1), msLevel(-1), numPeaksInScan(-1),
      retentionTimeInSec(-1), lowMass(-1), highMass(-1), TIC(-1),
      basePeakMass(-1), basePeakIntensity(-1), isCalibrated(false), skip(false)
{

}

MassLynxInterface::MassLynxInterface(void)
{
  // InstrumentInterface members
  // store counts for up to +15
  chargeCounts_.resize(16, 0);

  instrumentInfo_.manufacturer_ = WATERS;
  instrumentInfo_.acquisitionSoftware_ = MASSLYNX;

  // TODO: since DAC has no function to retrieve version,
  // add user definable switch
  instrumentInfo_.acquisitionSoftwareVersion_ = "4.1";

  // MassLynxInterface members
  maxFunctionScan_ = -1;
  firstTime_ = true;
  firstUVTime_ = true;
  totalNumFunctions_ = -1;
  functionFilter_ = -1;
}

MassLynxInterface::~MassLynxInterface(void)
{
}

void MassLynxInterface::setFunctionFilter(int functionNumber)
{
    functionFilter_ = functionNumber;
}

void MassLynxInterface::initInterface(void)
{
  //original code was inititalizing COM. now we expect
  //COM to be initialized at the application level, not at
  //the library level

  return;
}

void MassLynxInterface::readFunctionType()
{
  totalNumScans_ = totalNumUVScans_ = 0;

  DACFunctionInfo functionInfo;

  for (int curFunction = 0; curFunction < totalNumFunctions_; curFunction++) {

    functionInfo.getFunctionInfo(inputFileName_, curFunction + 1);

    // determine function type and corresponding MS level
    QString funcType = functionInfo.getFunctionType();

    MSScanType scanType = SCAN_UNDEF;
    if (funcType.contains("MSMSMS")) {
      scanType = FULL;
    }
    else if (funcType.contains("MSMS")) {
      scanType = FULL;
    }
    else if (funcType.contains("Daughter")) {
      scanType = FULL;
    }
    else if (funcType.contains("MS")) {
      scanType = FULL;
    }
    else if (funcType.contains("Scan")) {
      scanType = FULL;
    }
    else if (funcType.contains("Survey")) {
      scanType = FULL;
    }
    else if (funcType.contains("Maldi TOF")) {
      scanType = FULL;
    }
    else if (funcType.contains("Diode Array")) {
      scanType = SCAN_UV;
    }
    else {
      Debug::msg() << "!!Warning!!: ignoring function " << (curFunction + 1)
          << " with type " << funcType;
    }

    if (scanType == FULL) {
        if (functionFilter_ <= 0 || curFunction + 1 == functionFilter_) {
            int numScan = functionInfo.getNumScans();
            totalNumScans_ += numScan;
        }
    }
    else if (scanType == SCAN_UV) {
      int numScan = functionInfo.getNumScans();
      totalNumUVScans_ += numScan;
    }

    functionTypes_.push_back(scanType);
  }
}

void MassLynxInterface::preprocessMSFunctions()
{
  DACFunctionInfo functionInfo;
  DACScanStats scanStats;

  maxFunctionScan_ = 0;

  for (int curFunction = 0; curFunction < (int) functionTypes_.size(); curFunction++) {

    if (functionFilter_ > 0 && curFunction + 1 != functionFilter_)
        continue;

    if (functionTypes_[curFunction] == FULL) {

      functionInfo.getFunctionInfo(inputFileName_, curFunction + 1);

      QString funcType = functionInfo.getFunctionType();

      int curMSLevel = 0;

      // TODO: fix mapping of scan types to OBO types
      if (funcType.contains("MSMSMS")) {
        curMSLevel = 3;
      }
      else if (funcType.contains("MSMS")) {
        curMSLevel = 2;
      }
      else if (funcType.contains("Daughter")) {
        curMSLevel = 2;
      }
      else if (funcType.contains("MS")) {
        curMSLevel = 1;
      }
      else if (funcType.contains("Scan")) {
        curMSLevel = 1;
      }
      else if (funcType.contains("Survey")) {
        curMSLevel = 1;
      }
      else if (funcType.contains("Maldi TOF")) {
        curMSLevel = 1;
      }
      else {
        assert(0); //Type should have been handle previously
      }

      if (verbose_) {
        Debug::msg() << "  function " << curFunction + 1 << " with type "
            << funcType << " set to ms level " << curMSLevel;
      }

      int numScan = functionInfo.getNumScans();

      if (verbose_) {
        Debug::msg() << "  function " << curFunction + 1 << " has " << numScan
            << " scans.";
      }

      double startRT = functionInfo.getStartRT();
      double endRT = functionInfo.getEndRT();

      if(verbose_) {
        Debug::msg() << "startRT: " << startRT;
        Debug::msg() << "endRT: " << endRT;
      }

      //for all the scans of that function
      int realScanCount = 0;
      unsigned scanHeaderStart = scanHeaderVec_.size();
      if(verbose_) {
        Debug::msg() << "scanHeaderStart: " << scanHeaderStart;
      }

      for (int scanIndex = 0; scanIndex < numScan; scanIndex++) {

        MassLynxScanHeader tempScanHeader;

        //In the Xevo-G2S we've got some cases where uninitialized scans
        //are inserted so we need to check the validity of the
        //scan first
        //Unfortunately it doesn't guaranty that all the bad scans will
        //be filtered. The unfiltered spectra themself should be tested later too
        scanStats.getScanStats(inputFileName_, curFunction + 1, 0, scanIndex
                               + 1);

        float rt = scanStats.getRetnTime();  ;
        float tic = scanStats.getTIC();
        double lowMass = scanStats.getLoMass();
        double highMass = scanStats.getHiMass();
        double basePeakMass = scanStats.getBPM();
        double basePeakIntensity = scanStats.getBPI();
        if(basePeakMass < lowMass || basePeakMass > highMass ||
           rt < startRT + 1.0e-5 || rt > endRT + 1.0e-5 ||
           tic < 1.0e-5 || tic > 1.0e+15 ||
           basePeakIntensity < 1.0e-5 || tic > 1.0e+15 ||
           isnan(rt) || isnan(tic) || isnan(basePeakMass) || isnan(basePeakIntensity)) {
          tempScanHeader.skip = true;
          Debug::dbg(Debug::HIGH) << "skip nonsense scan: " << scanIndex << Debug::ENDL;
        }
        else {
          tempScanHeader.skip = false;
          Debug::dbg(Debug::HIGH) << "temporary keep scan: " << scanIndex << Debug::ENDL;
          realScanCount++;

          tempScanHeader.funcNum = curFunction + 1;
          Debug::dbg(Debug::HIGH) << "funcNum: " << tempScanHeader.funcNum << Debug::ENDL;

          tempScanHeader.scanNum = scanIndex + 1;
          Debug::dbg(Debug::HIGH) << "scanNum: " << tempScanHeader.scanNum << Debug::ENDL;

          tempScanHeader.retentionTimeInSec = rt;
          // convert RT to seconds (from minutes)
          tempScanHeader.retentionTimeInSec
              = (float) (tempScanHeader.retentionTimeInSec * 60.0);
          Debug::dbg(Debug::HIGH) << "retentionTimeInSec: " << tempScanHeader.retentionTimeInSec
                                  << Debug::ENDL;

          tempScanHeader.msLevel = curMSLevel;
          Debug::dbg(Debug::HIGH) << "curMSLevel: " << tempScanHeader.msLevel << Debug::ENDL;

          tempScanHeader.numPeaksInScan = scanStats.getPeaksInScan();
          Debug::dbg(Debug::HIGH) << "numPeaksInScan: " << tempScanHeader.numPeaksInScan
                                  << Debug::ENDL;

          tempScanHeader.TIC = tic;
          Debug::dbg(Debug::HIGH) << "TIC: " << tempScanHeader.TIC << Debug::ENDL;

          tempScanHeader.lowMass = lowMass;
          Debug::dbg(Debug::HIGH) << "lowMass: " << tempScanHeader.lowMass << Debug::ENDL;

          tempScanHeader.highMass = highMass;
          Debug::dbg(Debug::HIGH) << "highMass: " << tempScanHeader.highMass << Debug::ENDL;

          tempScanHeader.basePeakMass = basePeakMass;
          Debug::dbg(Debug::HIGH) << "basePeakMass: " << tempScanHeader.basePeakMass << Debug::ENDL;

          tempScanHeader.basePeakIntensity = basePeakIntensity;
          Debug::dbg(Debug::HIGH) << "basePeakIntensity: " << tempScanHeader.basePeakIntensity << Debug::ENDL;

          //From original xml_out.cpp (note, curFunction == 1 is the second function)
          // Kludge.  Reference scans are (always?) in function 2
          //(and above?)
          //This is temporary until ExScanStats is repaired by
          //Waters
          tempScanHeader.isCalibrated = (curFunction > 0);
          Debug::dbg(Debug::HIGH) << "isCalibrated: " << tempScanHeader.isCalibrated << Debug::ENDL;
        }

        scanHeaderVec_.push_back(tempScanHeader);



        emit scanPreprocessed();
      }

      //get the scan time ratio from the time range
      unsigned int scanTimeRatio = 0;
      if(realScanCount > 1 && maxScanTimeRatio_ > 0.0) {

        double timeRange = endRT - startRT;
        double maxScanTimeRatio = maxScanTimeRatio_;
        if (maxScanTimeRatioSlope_ != 0) {
          maxScanTimeRatio = maxScanTimeRatioSlope_ * log(timeRange)
              + maxScanTimeRatio_;
        }

        //round to the closest integer to have an even repartition
        scanTimeRatio = round((realScanCount / timeRange) / maxScanTimeRatio);
        Debug::dbg(Debug::HIGH) << "scanTimeRatio: " << scanTimeRatio << Debug::ENDL;
      }

      int numUnskipedScans = 0;
      int realScanIndex = 0;
      for (int scanIndex = 0; scanIndex < numScan; scanIndex++) {

        if(scanHeaderVec_[scanHeaderStart + scanIndex].skip)
          continue;

        if (scanTimeRatio > 0 && realScanIndex % scanTimeRatio != 0) {
          scanHeaderVec_[scanHeaderStart + scanIndex].skip = true;
          Debug::dbg(Debug::HIGH) << "skip time excluded scan: " << scanIndex << Debug::ENDL;
        }
        else {
          numUnskipedScans++;
        }

        realScanIndex++;
      }

      if (verbose_) {
        Debug::msg() << numScan << " scans preprocessed";
      }

      maxFunctionScan_ = qMax(numUnskipedScans, maxFunctionScan_);
    }
  }

  if (totalNumScans_ != (int) scanHeaderVec_.size()) {
    QString msg = QString("total Number of scan mismatch: %1 %2"
      "").arg(totalNumScans_).arg(scanHeaderVec_.size());
    throw MassLynxInterfaceException(msg.toStdString());
  }
  if (totalNumScans_ == 0)
    throw MassLynxInterfaceException("no scan found");


  startTimeInSec_ = -1;
  endTimeInSec_ = -1;

  for (unsigned int i = 0; i < scanHeaderVec_.size(); ++i) {

    if(scanHeaderVec_[i].skip)
      continue;

    double rt = scanHeaderVec_[i].retentionTimeInSec;
    startTimeInSec_ = startTimeInSec_ == -1 ? rt : min(rt, startTimeInSec_);

    endTimeInSec_ = max(rt, endTimeInSec_);
  }

  firstScanNumber_ = 1;
  lastScanNumber_ = totalNumScans_;

  if (verbose_) {
    Debug::msg() << "MS Function preprocessing done.";
  }
}

void MassLynxInterface::preprocessUVFunctions()
{
  // Get the scan headers
  DACFunctionInfo functionInfo;
  DACScanStats scanStats;

  for (int curFunction = 0; curFunction < (int) functionTypes_.size(); curFunction++) {

    if (functionTypes_[curFunction] == SCAN_UV) {

      functionInfo.getFunctionInfo(inputFileName_, curFunction + 1);

      if (verbose_) {
        QString funcType = functionInfo.getFunctionType();
        Debug::msg() << "  function " << curFunction + 1 << " with type "
            << funcType;
      }

      long numScan = functionInfo.getNumScans();

      if (verbose_) {
        Debug::msg() << "  function " << curFunction + 1 << " has " << numScan
            << " scans.";
      }

      //to keep a reasonable ratio between UV and MS which is
      //sometimes desired when the UV detector scans very quickly
      //TODO I guess that it should be better to have a rate correction
      unsigned int uvmsRatio = 0;
      if (maxUVMSRatio_ > 0 && maxFunctionScan_ > 0) {
        uvmsRatio = numScan / maxFunctionScan_;
        uvmsRatio /= maxUVMSRatio_;
      }

      for (int p = 0; p < numScan; p++) {

        //check if ratio has to be maintain a ratio between UV and MS

        MassLynxScanHeader scanHeader;

        scanHeader.funcNum = curFunction + 1;
        scanHeader.scanNum = p + 1;

        if (uvmsRatio > 0 && (p % uvmsRatio) != 0) {
          scanHeader.skip = true;
        }
        else {
          scanHeader.skip = false;

          scanStats.getScanStats(inputFileName_, curFunction + 1, 0, p + 1);

          scanHeader.numPeaksInScan = scanStats.getPeaksInScan();
          scanHeader.retentionTimeInSec = scanStats.getRetnTime();
          scanHeader.retentionTimeInSec *= 60.0; // convert RT to seconds (from minutes)
        }

        uvScanHeaderVec_.push_back(scanHeader);

        emit scanPreprocessed();
      }

      if (verbose_) {
        Debug::msg() << numScan << " uv scans preprocessed";
      }
    }
  }

  if (totalNumUVScans_ != (int) uvScanHeaderVec_.size()) {
    QString msg = QString("total Number of UV scan mismatch: %1 %2"
      "").arg(totalNumUVScans_).arg(uvScanHeaderVec_.size());
    throw MassLynxInterfaceException(msg.toStdString());
  }

  initUVScan();

  if (verbose_) {
    Debug::msg() << "UV Function preprocessing done.";
  }
}

bool MassLynxInterface::setInputFile(const QString & inputFileName)
{
  QFileInfo fileInfo(inputFileName);
  if (fileInfo.exists() == false)
    throw MassLynxInterfaceException(QString("file \"%1\" doesn't exist"
      "").arg(inputFileName).toStdString());
  if (fileInfo.isDir() == false)
    throw MassLynxInterfaceException(QString("file \"%1\" is not a directory"
      "").arg(inputFileName).toStdString());

  inputFileName_ = inputFileName;

  // Determine number of acquired functions
  int curFunctionNumber = 1;
  FILE* fp = NULL;
  char funcName[500] = { '\0' };
  // TODO: use fstat instead?
  bool fileExists = true;
  totalNumFunctions_ = 0;
  while (fileExists) {

    sprintf(funcName, "%s\\_func%0.3d.dat",
            inputFileName_.toLocal8Bit().data(), curFunctionNumber);

    fp = fopen(funcName, "r");
    if (fp != NULL) {
      fileExists = true;
      totalNumFunctions_++;
      curFunctionNumber++;
      fclose(fp);
    }
    else {
      fileExists = false;
    }
  }

  // Determine number of processes per function
  DACProcessInfo processInfo;

  int numProcesses = 0;
  for (int n = 0; n < totalNumFunctions_; n++) {

    processInfo.getProcessInfo(inputFileName_, n + 1);
    numProcesses = processInfo.getNumProcesses();

    if (verbose_) {
      Debug::msg() << "Function " << n + 1 << " has " << numProcesses
          << " processes.";
    }
  }

  readFunctionType();

  //record filenames
  //
  // get the names of all the files in the raw directory
  QDir dir(inputFileName);
  if (dir.exists() == false)
    throw MassLynxInterfaceException("Could not read input directory: "
        + dir.path().toStdString());

  QFileInfoList list = dir.entryInfoList();
  for (int i = 0; i < list.size(); ++i) {
    QFileInfo fileInfo = list.at(i);

    if (verbose_) {
      Debug::msg() << "included file " << fileInfo.filePath();
    }

    inputFileNameList_.push_back(fileInfo.path());
  }

  if (verbose_) {
    Debug::msg() << "Initialization complete.";
  }

  return true;
}

bool MassLynxInterface::preprocessFunctions(bool uv)
{
  preprocessMSFunctions();

  if (uv)
    preprocessUVFunctions();

  DACHeader header;
  header.getHeader(inputFileName_);

  // TODO: set this correctly
  // set dummy instrument info:
  instrumentInfo_.instrumentModel_ = Q_TOF_MICRO;
  instrumentInfo_.instrumentName_ = "Q-TOF Micro";
  instrumentInfo_.ionSource_ = ESI;
  instrumentInfo_.analyzerList_.push_back(TOFMS);
  instrumentInfo_.detector_ = DETECTOR_UNDEF;

  QString acquDate = header.getAcquDate();
  QString acquTime = header.getAcquTime();

  timeStamp_ = acquDate + "T" + acquTime;

  // TODO: is this correct?
  instrumentInfo_.instrumentSerialNumber_ = header.getInstrument();

  curScanNum_ = 0;

  if (verbose_) {
    Debug::msg() << "Preprocessing complete.";
  }

  return true;
}

void MassLynxInterface::initUVScan()
{
  curUVScanNum_ = 0;
  firstUVScanNumber_ = totalNumUVScans_ == 0 ? 0 : 1;
  lastUVScanNumber_ = totalNumUVScans_;
  firstUVTime_ = true;
}

const MassLynxScanHeader *MassLynxInterface::getCurScanHeader()
{
  const MassLynxScanHeader *header = NULL;

  if ((unsigned int) curScanNum_ < scanHeaderVec_.size())
    return &scanHeaderVec_[curScanNum_];

  return header;
}

const MassLynxScanHeader *MassLynxInterface::getCurUVScanHeader()
{
  const MassLynxScanHeader *header = NULL;

  if ((unsigned int) curUVScanNum_ < uvScanHeaderVec_.size())
    return &uvScanHeaderVec_[curUVScanNum_];

  return header;
}

void MassLynxInterface::setCentroiding(bool centroid)
{
  doCentroid_ = centroid;
}

void MassLynxInterface::setDeisotoping(bool deisotope)
{
  doDeisotope_ = deisotope;
}

void MassLynxInterface::setCompression(bool compression)
{
  doCompression_ = compression;
}

void MassLynxInterface::setVerbose(bool verbose)
{
  verbose_ = verbose;
}

Scan* MassLynxInterface::getScan(void)
{
  if (!firstTime_) {
    ++curScanNum_;
    if (curScanNum_ > lastScanNumber_) {
      // we're done
      return NULL;
    }
  }
  else {
    firstTime_ = false;
  }

  Scan* curScan = new Scan();
  curScan->isMassLynx_ = true;

  // we've already stored a lot of scan info in the header:
  // copy that over to the scan object that we're building
  MassLynxScanHeader curScanHeader = scanHeaderVec_[curScanNum_];
  if (curScanHeader.skip == true)
    return curScan;

  curScan->msLevel_ = curScanHeader.msLevel;
  curScan->setNumDataPoints(curScanHeader.numPeaksInScan);
  curScan->retentionTimeInSec_ = curScanHeader.retentionTimeInSec;
  curScan->minObservedMZ_ = curScanHeader.lowMass;
  curScan->maxObservedMZ_ = curScanHeader.highMass;
  curScan->totalIonCurrent_ = curScanHeader.TIC;
  curScan->basePeakMZ_ = curScanHeader.basePeakMass;
  curScan->basePeakIntensity_ = curScanHeader.basePeakIntensity;
  curScan->scanType_ = functionTypes_[curScanHeader.funcNum - 1];

  // TODO: determine activation type correctly
  curScan->activation_ = CID;

  // MassLynx scans only:
  curScan->isCalibrated_ = curScanHeader.isCalibrated;

  // TODO: get scan range correctly
  // hack: set scan ranges to min/max observed
  curScan->startMZ_ = curScan->minObservedMZ_;
  curScan->endMZ_ = curScan->maxObservedMZ_;

  // go for precursor info
  if (curScan->msLevel_ > 1) {

    exScanStats_.getExScanStats(inputFileName_, curScanHeader.funcNum, 0, // process-- why always fixed to 0?
                                curScanHeader.scanNum);

    curScan->collisionEnergy_ = exScanStats_.getCollisionEnergy();
    curScan->precursorMZ_ = exScanStats_.getSetMass();
    // TODO: set precursor to accurate mass?

    // TODO: get precursor scan number
    //// try to get precursor scan number
    //unsigned char referenceScan = 0;
    //referenceScan = pExScanStats->GetReferenceScan();
    //Debug::msg() << referenceScan << endl;
  }

  // Read the m/z intensity pairs
  spectrum_.getSpectrum(inputFileName_, curScanHeader.funcNum, 0,
                        curScanHeader.scanNum);

  std::vector<float> intensityArray, massArray;

  spectrum_.getIntensities(intensityArray);
  spectrum_.getMasses(massArray);

  // TODO: do centroiding here

  unsigned int numDataPoints = curScanHeader.numPeaksInScan;

  assert(massArray.size() == numDataPoints);
  assert(intensityArray.size() == numDataPoints);

  for (unsigned int c = 0; c < numDataPoints; c++) {
    curScan->mzArray_[c] = massArray[c];
    curScan->intensityArray_[c] = intensityArray[c];
  }

  return curScan;
}

UVScan *MassLynxInterface::getUVScan(void)
{
  if (!firstUVTime_) {
    ++curUVScanNum_;
    if (curUVScanNum_ > lastUVScanNumber_) {
      // we're done
      return NULL;
    }
  }
  else {
    firstUVTime_ = false;
  }

  UVScan* curScan = new UVScan();

  // we've already stored a lot of scan info in the header:
  // copy that over to the scan object that we're building
  MassLynxScanHeader curScanHeader = uvScanHeaderVec_[curUVScanNum_];
  if (curScanHeader.skip == true)
    return curScan;

  curScan->setTime(curScanHeader.retentionTimeInSec);

  // Read the m/z intensity pairs
  spectrum_.getSpectrum(inputFileName_, curScanHeader.funcNum, 0,
                        curScanHeader.scanNum);

  std::vector<float> intensityArray, massArray;

  spectrum_.getIntensities(intensityArray);
  spectrum_.getMasses(massArray);

  assert(massArray.size() == intensityArray.size());

  for (unsigned int c = 0; c < massArray.size(); c++) {
    curScan->getSpectrum().addDataPoint(massArray[c], intensityArray[c]);
  }

  return curScan;
}
