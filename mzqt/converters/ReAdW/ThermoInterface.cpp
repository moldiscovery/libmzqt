/*
 File: ThermoInterface.cpp
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

#include <cmath>
#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QFile>
#include <QVariant>
#include <QStringList>
#include <QProcess>

#include "ThermoInterface.h"
#include <mzqt/common/Scan.h>
#include <mzqt/common/UVScan.h>
#include <mzqt/common/MSUtilities.h>
#include <mzqt/common/Debug.h>

#ifdef USE_MMGR_MEMORY_CHECK
#include <mmgr.h>
#endif

#define DEBUG_GET_SCAN 0
#define DEBUG_PRECURSOR_INFO 0

using namespace mzqt;

namespace {
  enum ControllerType {
    BEGIN_CONTROLLER_TYPE = 0,
    MS_CONTROLLER_TYPE = BEGIN_CONTROLLER_TYPE,
    ANALOG_CONTROLLER_TYPE,
    AD_CARD_CONTROLLER_TYPE,
    PDA_CONTROLLER_TYPE, //Photo Diode Array
    UV_CONTROLLER_TYPE,
    END_CONTROLLER_TYPE
  };

}

ThermoInterfaceException::ThermoInterfaceException(const std::string &msg) :
  Exception(msg)
{
  msg_ = "THERMO INTERFACE ERROR(" + msg + ")";
}

ThermoInterfaceException::~ThermoInterfaceException() throw ()
{
}

ThermoInterface::ThermoInterface(void)
{
  // InstrumentInterface members
  instrumentInfo_.manufacturer_ = THERMO;
  instrumentInfo_.acquisitionSoftware_ = XCALIBUR;
  chargeCounts_.resize(1, 0);

  //This interface members
  forcePrecursorFromFilter_ = false;
  IXRawfileVersion_ = -1;

  firstTime_ = true;
  firstUVTime_ = true;

  getPreInfoCount_ = 0;
  filterLineCount_ = 0;
  oldAPICount_ = 0;
}

ThermoInterface::~ThermoInterface(void)
{
}

void ThermoInterface::initInterface(void)
{
  QStringList env = QProcess::systemEnvironment();
  QString componentName;
  foreach(const QString &s, env)
    {
      QStringList var = s.split('=');
      if (var.value(0) == "MZQT_THERMO_COMPONENT") {
        componentName = var.value(1);
      }
    }

  bool result = xrawfile2_.init(componentName);

  if (result == false) {
    QString msg = "Unable to init interface " + componentName;
    throw ThermoInterfaceException(msg.toStdString());
  }

  return;
}

void ThermoInterface::initUVScan()
{
  firstUVScanNumber_ = 1;
  curUVScanNum_ = firstUVScanNumber_;
  lastUVScanNumber_ = totalNumUVScans_;
  firstUVTime_ = true;
}

void ThermoInterface::setMSController()
{
  xrawfile2_.SetCurrentController(MS_CONTROLLER_TYPE, 1);
}

void ThermoInterface::setUVController()
{
  xrawfile2_.SetCurrentController(PDA_CONTROLLER_TYPE, 1);
}

bool ThermoInterface::setInputFile(const QString& filename)
{
  if (QFile::exists(filename) == false)
    throw ThermoInterfaceException(QString("file \"%1\" doesn't exist"
      "").arg(filename).toStdString());

  // open raw file
  xrawfile2_.Open(filename);

  if (verbose_)
    Debug::msg() << "(Thermo lib opened file: " << filename << ")";

  // test the file format version number, also useful to check if the
  // file is a valid XCalibur file
  int fileVersionNumber = -1;
  xrawfile2_.GetVersionNumber(fileVersionNumber);
  if (fileVersionNumber == 0) {
    QString msg =
        QString("Unable to get file version for file: %1").arg(filename);
    throw ThermoInterfaceException(msg.toStdString());
  }

#if 0
  Debug::dbg(Debug::MEDIUM) << "file version is " << fileVersionNumber
  << ", interface version is ";
  if (IXRawfileVersion_ == 2) {
    Debug::dbg(Debug::MEDIUM) << "1.4";
  }
  else if (IXRawfileVersion_ >= 3) {
    Debug::dbg(Debug::MEDIUM) << "2.0 or greater";
  }
  Debug::flush();
#endif

  if (Debug::possible(Debug::HIGH)) {
    //Controllers
    for (int i = BEGIN_CONTROLLER_TYPE; i < END_CONTROLLER_TYPE; ++i) {

      int numOfControllers = 0;
      xrawfile2_.GetNumberOfControllersOfType(i, numOfControllers);
      Debug::dbg(Debug::HIGH) << "number of controllers for type: " << i
          << " = " << numOfControllers << endl;
    }

    Debug::flush();
  }

  //UV
  setUVController();
  xrawfile2_.GetFirstSpectrumNumber(firstUVScanNumber_);
  xrawfile2_.GetLastSpectrumNumber(lastUVScanNumber_);
  Debug::dbg(Debug::MEDIUM) << "file should contain UV scan numbers "
      << firstUVScanNumber_ << " through " << lastUVScanNumber_ << Debug::ENDL;

  totalNumUVScans_ = (lastUVScanNumber_ - firstUVScanNumber_) + 1;
  initUVScan();

  //MS

  // Get the total number of scans
  setMSController();
  xrawfile2_.GetFirstSpectrumNumber(firstScanNumber_);
  xrawfile2_.GetLastSpectrumNumber(lastScanNumber_);
  Debug::dbg(Debug::MEDIUM) << "file should contain scan numbers "
      << firstScanNumber_ << " through " << lastScanNumber_ << Debug::ENDL;

  curScanNum_ = firstScanNumber_;

  totalNumScans_ = (lastScanNumber_ - firstScanNumber_) + 1;

  // get the start and the end time of the run
  double startTime, endTime;
  xrawfile2_.GetStartTime(startTime);
  xrawfile2_.GetEndTime(endTime);
  // convert from minutes to seconds
  startTimeInSec_ = 60.0 * startTime;
  endTimeInSec_ = 60.0 * endTime;

  // get the instrument model
  QString instModel;
  xrawfile2_.GetInstModel(instModel);
  instModel = instModel.toUpper();

  Debug::dbg(Debug::MEDIUM) << "Instrument model: " << instModel << Debug::ENDL;

  if (instModel == "LTQ") {
    instrumentInfo_.instrumentModel_ = LTQ;
    instrumentInfo_.manufacturer_ = THERMO_SCIENTIFIC;
  }
  else if (instModel == "LTQ XL") {
    instrumentInfo_.instrumentModel_ = LTQ_XL;
    instrumentInfo_.manufacturer_ = THERMO_FINNIGAN;
  }
  else if (instModel == "LTQ FT") {
    instrumentInfo_.instrumentModel_ = LTQ_FT;
    instrumentInfo_.manufacturer_ = THERMO_SCIENTIFIC;
  }
  else if (instModel == "LTQ FT ULTRA") {
    instrumentInfo_.instrumentModel_ = LTQ_FT_ULTRA;
    instrumentInfo_.manufacturer_ = THERMO_SCIENTIFIC;
  }
  else if (instModel == "LTQ ORBITRAP") {
    instrumentInfo_.instrumentModel_ = LTQ_ORBITRAP;
    instrumentInfo_.manufacturer_ = THERMO_SCIENTIFIC;
  }
  else if (instModel == "LTQ ORBITRAP DISCOVERY") {
    instrumentInfo_.instrumentModel_ = LTQ_ORBITRAP_DISCOVERY;
    instrumentInfo_.manufacturer_ = THERMO_SCIENTIFIC;
  }
  else if (instModel == "LTQ ORBITRAP XL") {
    instrumentInfo_.instrumentModel_ = LTQ_ORBITRAP_XL;
    instrumentInfo_.manufacturer_ = THERMO_SCIENTIFIC;
  }
  else if (instModel == "LXQ") {
    instrumentInfo_.instrumentModel_ = LXQ;
    instrumentInfo_.manufacturer_ = THERMO_SCIENTIFIC;
  }
  else if (instModel == "TSQ QUANTUM ACCESS") {
    instrumentInfo_.instrumentModel_ = TSQ_QUANTUM_ACCESS;
    instrumentInfo_.manufacturer_ = THERMO_SCIENTIFIC;
  }
  else if (instModel == "TSQ QUANTUM") {
    instrumentInfo_.instrumentModel_ = TSQ_QUANTUM;
    instrumentInfo_.manufacturer_ = THERMO_SCIENTIFIC;
  }
  else if (instModel == "LCQ") {
    instrumentInfo_.instrumentModel_ = LCQ;
    instrumentInfo_.manufacturer_ = THERMO_FINNIGAN; // TODO: verify "finnigan"
  }
  else if (instModel == "LCQ ADVANTAGE") {
    instrumentInfo_.instrumentModel_ = LCQ_ADVANTAGE;
    instrumentInfo_.manufacturer_ = THERMO_FINNIGAN;
  }
  else if (instModel == "LCQ CLASSIC") {
    instrumentInfo_.instrumentModel_ = LCQ_CLASSIC;
    instrumentInfo_.manufacturer_ = THERMO_FINNIGAN;
  }
  else if (instModel == "LCQ DECA") {
    instrumentInfo_.instrumentModel_ = LCQ_DECA;
    instrumentInfo_.manufacturer_ = THERMO_FINNIGAN;
  }
  else if (instModel == "LCQ DECA XP") {
    instrumentInfo_.instrumentModel_ = LCQ_DECA_XP;
    instrumentInfo_.manufacturer_ = THERMO_FINNIGAN;
  }
  else if (instModel == "LCQ DECA XP PLUS") {
    instrumentInfo_.instrumentModel_ = LCQ_DECA_XP_PLUS;
    instrumentInfo_.manufacturer_ = THERMO_FINNIGAN;
  }
  else if (instModel == "LCQ FLEET") {
    instrumentInfo_.instrumentModel_ = LCQ_FLEET;
    instrumentInfo_.manufacturer_ = THERMO_FINNIGAN;
  }
  else if (instModel == "TSQ VANTAGE STANDARD") {
    instrumentInfo_.instrumentModel_ = TSQ_VANTAGE_STANDARD;
    instrumentInfo_.manufacturer_ = THERMO_FINNIGAN;
  }
  else if (instModel == "LTQ VELOS") {
    instrumentInfo_.instrumentModel_ = LTQ_VELOS;
    instrumentInfo_.manufacturer_ = THERMO_FINNIGAN;
  }
  else if (instModel == "LTQ ORBITRAP VELOS") {
    instrumentInfo_.instrumentModel_ = LTQ_ORBITRAP_VELOS;
    instrumentInfo_.manufacturer_ = THERMO_FINNIGAN;
  }
  else {
    instrumentInfo_.instrumentModel_ = INSTRUMENTMODEL_UNDEF;
    instrumentInfo_.manufacturer_ = THERMO_FINNIGAN;
  }

  // get instrument name
  xrawfile2_.GetInstName(instrumentInfo_.instrumentName_);

  // TODO: is this the Xcalibur or inst version?
  // get acquisition software version
  xrawfile2_.GetInstSoftwareVersion(instrumentInfo_.acquisitionSoftwareVersion_);

  // get instrument hardware version
  xrawfile2_.GetInstHardwareVersion(instrumentInfo_.instrumentHardwareVersion_);

#if 0
  // get instrument description
  QString instDescription;
  xrawfile2_.GetInstrumentDescription(&instDescription);
  // get instrument serial number
  QString instSerialNumber;
  xrawfile2_->GetInstSerialNumber(&instSerialNumber);
#endif

  // get instrument ion source, analyzer
  // assuming first scan's filter line info is true for all scans
  // TODO: This is known to be wrong in LTQ-FT case (both FT and ion trap)
  QString filter;
  xrawfile2_.GetFilterForScanNum(1, filter);

  FilterLine filterLine;
  if (!filterLine.parse(filter.toStdString())) {
    QString msg = "error parsing filter line: " + filter;
    throw ThermoInterfaceException(msg.toStdString());
  }

  // TODO: deal with muliple analyzers, like FT/IT
  MSAnalyzerType analyzer = filterLine.analyzer_;
  // TODO: hack! do this properly with library somehow?
  if (analyzer == ANALYZER_UNDEF) {
    // set default
    analyzer = ITMS;
  }
  instrumentInfo_.analyzerList_.push_back(analyzer);

  MSIonizationType ionization = filterLine.ionizationMode_;
  // TODO: hack! determine instrument info better
  if (ionization == IONIZATION_UNDEF) {
    // set default
    ionization = NSI;
  }
  instrumentInfo_.ionSource_ = ionization;

  // get time from file
  QDateTime date;
  xrawfile2_.GetCreationDate(date);
  timeStamp_ = date.toString();

#if 0
  QStringList filters;
  xrawfile2_.GetFilters(filters);
  foreach(QString s, filters)
  {
    Debug::dbg(Debug::MEDIUM) << "filter:" << s.toStdString();
  }
#endif

  return true;
}

void ThermoInterface::setVerbose(bool verbose)
{
  verbose_ = verbose;
}

void ThermoInterface::setCentroiding(bool centroid)
{
  doCentroid_ = centroid;
}

void ThermoInterface::setDeisotoping(bool deisotope)
{
  doDeisotope_ = deisotope;
}

void ThermoInterface::forcePrecursorFromFilter(bool force)
{
  forcePrecursorFromFilter_ = force;
}

void ThermoInterface::setCompression(bool compression)
{
  doCompression_ = compression;
}

Scan* ThermoInterface::getScan(void)
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

  Debug::dbg(Debug::MEDIUM) << "getting scan: " << curScanNum_ << Debug::ENDL;

  Scan* curScan = new Scan();

  curScan->isThermo_ = true;

  //// test the "scan event" call
  //// gives a more through scan filter line
  //BSTR bstrScanEvent = NULL;
  //xrawfile2_->GetScanEventForScanNum(curScanNum_, &bstrScanEvent);
  //SysFreeString(bstrScanEvent);

  // Get the scan filter
  // (ex: "ITMS + c NSI Full ms [ 300.00-2000.00]")
  Debug::dbg(Debug::VERY_HIGH) << "getting filter line" << Debug::ENDL;

  xrawfile2_.GetFilterForScanNum(curScanNum_, curScan->thermoFilterLine_);

  Debug::dbg(Debug::VERY_HIGH) << "parsing filter line" << Debug::ENDL;

  FilterLine filterLine;
  if (!filterLine.parse(curScan->thermoFilterLine_.toStdString())) {
    QString msg = "error parsing filter line: " + curScan->thermoFilterLine_;
    throw ThermoInterfaceException(msg.toStdString());
  }

  // we should now have:
  // msLevel
  // polarity
  // scanType (full, zoom, etc)
  // ionization type
  // analyzer
  // scan range
  // parent mass and CID energy, if MS >=2

  // record msLevel from filter line
  curScan->msLevel_ = filterLine.msLevel_;

  // record polarity from filter line
  curScan->polarity_ = filterLine.polarity_;

  // record analyzer from filter line
  curScan->analyzer_ = filterLine.analyzer_;

  // record ionization from filter line
  curScan->ionization_ = filterLine.ionizationMode_;

  // record scan type from filter line
  // (zoom, full, srm, etc)
  curScan->scanType_ = filterLine.scanType_;

  // record data-dependent scan from filter line
  curScan->dependentActive_ = filterLine.dependentActive_
      == FilterLine::BOOL_TRUE ? true : false;

  // record activation (CID, etc)
  // check FilterLine: this may be default set to CID now
  if (filterLine.activationMethod_ != ACTIVATION_UNDEF) {
    curScan->activation_ = filterLine.activationMethod_;
  }
  else {
    curScan->activation_ = CID;
  }

  // record scan ranges from filter line
  // Note: SRM fills this with range of q3 transtion lists
  if (curScan->scanType_ != SRM) {
    curScan->startMZ_
        = filterLine.scanRangeMin_[filterLine.scanRangeMin_.size() - 1];
    curScan->endMZ_ = filterLine.scanRangeMax_[filterLine.scanRangeMax_.size()
        - 1];
  }
  else {
    // SRM: record range of q3 transition lists
    // start mz is average of first transition range
    // end mz is average of last transition range
    curScan->startMZ_ = (filterLine.transitionRangeMin_[0]
        + filterLine.transitionRangeMin_[filterLine.transitionRangeMin_.size()
            - 1]) / 2;
    curScan->endMZ_ = (filterLine.transitionRangeMax_[0]
        + filterLine.transitionRangeMax_[filterLine.transitionRangeMax_.size()
            - 1]) / 2;
  }
  // get additional header information through Xcalibur:
  // retention time
  // min/max observed mz
  // total ion current
  // base peak mz and intensity
  // precursor (JMT: if avalible from interface, else use filter line info)

  Debug::dbg(Debug::VERY_HIGH) << "getting scan header info" << Debug::ENDL;

  int numDataPoints = -1; // points in both the m/z and intensity arrays
  double retentionTimeInMinutes = -1;
  int channel; // unused
  bool uniformTime; // unused
  double frequency; // unused

  xrawfile2_.GetScanHeaderInfoForScanNum(curScanNum_, numDataPoints,
                                         retentionTimeInMinutes,
                                         curScan->minObservedMZ_,
                                         curScan->maxObservedMZ_,
                                         curScan->totalIonCurrent_,
                                         curScan->basePeakMZ_,
                                         curScan->basePeakIntensity_, channel, // unused
                                         uniformTime, // unused
                                         frequency // unused
  );

  //Good summary of what the scan is
  Debug::dbg(Debug::LOW) << "getting scan: " << curScanNum_ << " time: "
      << retentionTimeInMinutes << " filter: " << curScan->thermoFilterLine_
      << Debug::ENDL;

  // NOTE! the returned numDataPoints is invalid!
  // use the value from GetMassListFromScanNum below

  // record the retention time
  curScan->retentionTimeInSec_ = retentionTimeInMinutes * 60.0;

  // if ms level 2 or above, get precursor info
  if (curScan->msLevel_ > 1) {

    Debug::dbg(Debug::VERY_HIGH) << "getting precursor info" << Debug::ENDL;

    getPrecursorInfo(*curScan, curScanNum_, filterLine);
  }

  //
  // get the m/z intensity pairs list for the current scan
  // !and correct min/max observed m/z info here!
  //

  // Debug::dbg(Debug::VERY_HIGH) << "reading data points for scan " << curScanNum_ << endl;

  curScan->minObservedMZ_ = 0;
  curScan->maxObservedMZ_ = 0;

  Debug::dbg(Debug::VERY_HIGH) << "get spectrum data: " << numDataPoints
      << Debug::ENDL;

  if (numDataPoints != 0) {

    // set up the parameters to read the scan
    // TODO make centroid parameter user customizable
    int dataPoints = 0;
    int scanNum = curScanNum_;
    QString szFilter = curScan->thermoFilterLine_;

    // record centroiding info
    //
    // scan may have been centroided at accquision time,
    // rather than conversion time (now)
    // even if user didn't request it.
    if (doCentroid_ || filterLine.scanData_ == CENTROID) {
      curScan->isCentroided_ = true;
    }

    // Note: special case for FT centroiding, contributed from Matt Chambers
    if (doCentroid_ && (curScan->analyzer_ == FTMS)) {
      // use GetLabelData to workaround bug in Thermo centroiding of FT profile data

      Debug::dbg(Debug::VERY_HIGH) << "using get label data" << Debug::ENDL;

      QList<double> masses, intensities;

      xrawfile2_.GetLabelData(masses, intensities, scanNum);

      // record the number of data point (allocates memory for arrays)

      Debug::dbg(Debug::VERY_HIGH) << "saving: " << masses.size()
          << " data points" << Debug::ENDL;

      assert(intensities.size() == masses.size());
      dataPoints = masses.size();
      curScan->setNumDataPoints(dataPoints);
      for (long i = 0; i < dataPoints; ++i) {
        curScan->mzArray_[i] = masses.at(i);
        curScan->intensityArray_[i] = intensities.at(i);
      }
    }
    else {

      Debug::dbg(Debug::VERY_HIGH) << "using average function" << Debug::ENDL;

      // centroid is not done anyway because of thermo bug
      if (curScan->isCentroided_ == true && filterLine.scanData_ == PROFILE)
        curScan->isCentroided_ = false;

      bool centroidThisScan = curScan->isCentroided_;

      //work around
      //call average function with only one scan since normal mass list call
      //doesn't seem to always works
      QList<double> masses, intensities;
      QList<int> scanNumbers;
      scanNumbers << scanNum;
      xrawfile2_.GetAveragedMassSpectrum(scanNumbers, centroidThisScan, masses,
                                         intensities);

      Debug::dbg(Debug::VERY_HIGH) << "saving: " << intensities.size()
          << " data points" << Debug::ENDL;

      // record the number of data point (allocates memory for arrays)
      assert(masses.size() == intensities.size());
      dataPoints = masses.size();
      curScan->setNumDataPoints(dataPoints);
      // record mass list information in scan object
      for (long j = 0; j < dataPoints; j++) {
        curScan->mzArray_[j] = masses[j];
        curScan->intensityArray_[j] = intensities[j];
      }

      /*
       if (doCentroid_ && filterLine.scanData_ == PROFILE)
       {
       curScan->centroid(""); //TODO check if the algorithm works well
       }
       */

    }

    // If we centroided the data we need to correct the basePeak m/z and intensity
    if (doCentroid_) {

      Debug::dbg(Debug::VERY_HIGH) << "recomputing base peak" << Debug::ENDL;

      curScan->basePeakIntensity_ = 0;
      for (long j = 0; j < curScan->getNumDataPoints(); j++) {
        if (curScan->intensityArray_[j] > curScan->basePeakIntensity_) {
          curScan->basePeakMZ_ = curScan->mzArray_[j];
          curScan->basePeakIntensity_ = curScan->intensityArray_[j];
        }
      }
    }

    // !!
    // Fix to overcome bug in ThermoFinnigan library GetScanHeaderInfoForScanNum() function
    // !!
    if (curScan->getNumDataPoints() > 0) {

      Debug::dbg(Debug::VERY_HIGH) << "fix min/max mz" << Debug::ENDL;

      // don't do this on an empty scan!
      curScan->minObservedMZ_ = curScan->mzArray_[0];
      curScan->maxObservedMZ_ = curScan->mzArray_[curScan->getNumDataPoints()
          - 1];
    }
  } // end 'not empty scan'

  else {
    // if empty scan:
    if (verbose_) {
      Debug::msg() << "Note: empty scan detected (scan # " << curScanNum_
          << ")" << endl;
    }
  }

  return curScan;
}

// get precursor m/z, collision energy, precursor charge, and precursor intensity
void ThermoInterface::getPrecursorInfo(Scan& scan, long scanNumber,
    FilterLine& filterLine)
{
  Debug::dbg(Debug::MEDIUM) << "getting precursor info for scan: "
      << scanNumber << Debug::ENDL;

  assert(scan.msLevel_ > 1);

  //save filterline param that may be useful for further processing
  scan.cidEnergy_ = filterLine.cidEnergy_;
  scan.cidParentMass_ = filterLine.cidParentMass_;

  if (scanNumber == 1) {

    Debug::dbg(Debug::VERY_HIGH) << "get first scan precursor data"
        << Debug::ENDL;

    // if this is the first scan, only use the info from the filter lin
    // (API calls are no use, as there's no precursor scan)
    // An example of a first scan WITH filterline precursor info:
    // "+ c NSI sid=5.00  SRM ms2 580.310@cid25.00 [523.955-523.965, 674.405-674.415, 773.295-773.305]"

    scan.accuratePrecursorMZ_ = false;
    // use the low-precision parent mass in the filter line
    inaccurateMasses_++;
    scan.precursorMZ_
        = filterLine.cidParentMass_[filterLine.cidParentMass_.size() - 1];
    scan.accuratePrecursorMZ_ = false;

    // use the low-precision collision energy recorded in the filter line
    scan.collisionEnergy_ = filterLine.cidEnergy_[filterLine.cidEnergy_.size()
        - 1];

    scan.precursorCharge_ = -1; // undetermined
    chargeCounts_[0]++; // with "charge 0" being undetermined

    scan.precursorIntensity_ = 0; // undetermined

    return;
  }

  /*
   // first try with the more direct GetPrecursorInfoFromScanNum call;
   // if this fails, continue with older methods
   if (IXRawfileVersion_ > 2) {
   try
   {
   VARIANT vPrecursorInfos;
   VariantInit(&vPrecursorInfos);
   long nPrecursorInfos = 0;


   // Get the precursor scan information
   XRAWFILE2Lib::IXRawfile3 * ix3ptr = (XRAWFILE2Lib::IXRawfile3 *)xrawfile2_.GetInterfacePtr();

   ix3ptr->GetPrecursorInfoFromScanNum(scanNumber,
   &vPrecursorInfos,
   &nPrecursorInfos);


   // for now, exit if anything more than 1 precursor found
   if (nPrecursorInfos > 1) {
   cerr << "halting; multiple precursors detected.  please contact developer." << endl;
   exit(1);
   }

   // Access the safearray buffer
   BYTE* pData;
   SafeArrayAccessData(vPrecursorInfos.parray, (void**)&pData);

   for (int i=0; i < nPrecursorInfos; ++i) {
   // Copy the scan information from the safearray buffer
   //PrecursorInfo info;
   XRAWFILE2Lib::MS_PrecursorInfo precursorInfo;
   memcpy(&precursorInfo,
   pData + i * sizeof(XRAWFILE2Lib::MS_PrecursorInfo),
   sizeof(XRAWFILE2Lib::MS_PrecursorInfo));

   // set the scan info
   scan.precursorCharge_ = precursorInfo.nChargeState;
   scan.precursorScanNumber_ = precursorInfo.nScanNumber;
   scan.precursorMZ_ = precursorInfo.dMonoIsoMZ;
   if (precursorInfo.dMonoIsoMZ != 0) {
   scan.accuratePrecursorMZ_ = true;
   accurateMasses_++;
   getPreInfoCount_++;
   }
   }

   SafeArrayUnaccessData(vPrecursorInfos.parray);
   }
   catch (...)
   {
   cerr << "There was a problem while getting the precursor scan information (scan " <<
   scanNumber <<"); exiting" << endl;
   cerr << "please contact developer with this error message." << endl;
   }
   }

   // hopefully we have everything except precursor intensity and collision energy here.
   // if not, fall back on the older versions.

   */
#if 0
  VARIANT varValue;
  VariantInit(&varValue);
#else
  QVariant varValue;
#endif

  /*
   // compare new vs old API
   double oldMZ = 0;
   xrawfile2_->GetTrailerExtraValueForScanNum(curScanNum_, "Monoisotopic M/Z:" , &varValue);

   if( varValue.vt == VT_R4 ) {
   oldMZ = varValue.fltVal;
   }
   else if( varValue.vt == VT_R8 ) {
   oldMZ = varValue.dblVal;
   }
   else if ( varValue.vt != VT_ERROR ) {
   cerr << "Scan: " << curScanNum_ << " MS level: " << scan.msLevel_
   << " unexpected type when looking for precursorMz\n";
   exit(-1);
   }
   if (oldMZ != scan.precursorMZ_) {
   cerr << "scan: " << setprecision(11) << curScanNum_ << " old mz=" << oldMZ << " new mz=" << scan.precursorMZ_ << endl;
   }
   */

  //
  // precursor m/z fallbacks
  // try the Thermo API first, then falling back on the value extracted from the filter line
  //

  //if (scan.precursorMZ_ <= 0) { // restore if getPrec() call works in the future
  if (1) {
    // we'll try to get the "accurate mass" recorded by the machine, and fall back to the filter line value otherwise.
    scan.accuratePrecursorMZ_ = false;

    // see if we can get an accurate value from the machine

    double precursorMZ = 0;

    // don't try to get Monoisotopic M/Z if the user has selected "force precursor from filter line"
    if (!forcePrecursorFromFilter_) {

      Debug::dbg(Debug::VERY_HIGH) << "try to get mz extra value..."
          << Debug::ENDL;
      // ignore return value from this call
      xrawfile2_.GetTrailerExtraValueForScanNum(curScanNum_,
                                                "Monoisotopic M/Z:", varValue);

      precursorMZ = varValue.toDouble();
    }

    if (filterLine.cidParentMass_.size() == 0) {
      Debug::dbg(Debug::VERY_HIGH) << "unavailable parent mass" << Debug::ENDL;
      return;
    }
    scan.precursorMZ_
        = filterLine.cidParentMass_[filterLine.cidParentMass_.size() - 1];

    if (precursorMZ > 0 && fabs(precursorMZ - scan.precursorMZ_) <= 10.0) {
      // (note: this could only true if we tried and had sucess
      // with the monoisotopic call above.)
      // Sanity check to make sure mono mass is in ballpark of filter line mass.

      // sucess-- higher accuracy m/z recorded through API
      Debug::dbg(Debug::VERY_HIGH) << "got higher accuracy mass" << Debug::ENDL;

      accurateMasses_++;
      scan.precursorMZ_ = precursorMZ;
      scan.accuratePrecursorMZ_ = true;
      oldAPICount_++;
    }
    else {
      Debug::dbg(Debug::VERY_HIGH) << "got higher accuracy mass" << Debug::ENDL;

      // use the low-precision parent mass in the filter line
      inaccurateMasses_++;
      //scan.precursorMZ_ = filterLine.cidParentMass_[filterLine.cidParentMass_.size() - 1];
      scan.accuratePrecursorMZ_ = false;
      filterLineCount_++;
    }
  }

  //
  // collision energy, trying the Thermo API first, then falling back on the value extracted from the filter line
  //
  Debug::dbg(Debug::VERY_HIGH) << "try to get collision energy extra value..."
      << Debug::ENDL;

  double collisionEnergy = 0;
  xrawfile2_.GetTrailerExtraValueForScanNum(curScanNum_,
                                            "API Source CID Energy:", varValue);

  collisionEnergy = varValue.toDouble();

  if (collisionEnergy != 0) {
    Debug::dbg(Debug::VERY_HIGH) << "got higher accuracy collision"
        << Debug::ENDL;
    // sucess-- collision energy recorded through API
    scan.collisionEnergy_ = collisionEnergy;
  }
  else {

    Debug::dbg(Debug::VERY_HIGH) << "got lower accuracy collision"
        << Debug::ENDL;
    if (filterLine.cidEnergy_.size() == 0) {
      Debug::dbg(Debug::VERY_HIGH) << "unavailable collision energy"
          << Debug::ENDL;
      return;
    }

    // use the low-precision collision energy recorded in the filter line
    scan.collisionEnergy_ = filterLine.cidEnergy_[filterLine.cidEnergy_.size()
        - 1];
  }

  int trailerPrecursorCharge = 0;
  if (scan.precursorCharge_ < 1) {
    // precursor charge state fallbacks, again trying first from the API, then resorting to filter line value

    Debug::dbg(Debug::VERY_HIGH) << "try to get charge state extra value..."
        << Debug::ENDL;
    xrawfile2_.GetTrailerExtraValueForScanNum(curScanNum_, "Charge State:",
                                              varValue);

    trailerPrecursorCharge = varValue.toInt();

    if (trailerPrecursorCharge != 0) {

      Debug::dbg(Debug::VERY_HIGH) << "got charge state" << Debug::ENDL;

      // sucess-- precursor charge recorded through API
      scan.precursorCharge_ = trailerPrecursorCharge;
    }
    else {
      // no luck
      Debug::dbg(Debug::VERY_HIGH) << "precursor charge not available"
          << Debug::ENDL;
      scan.precursorCharge_ = -1; // undetermined
    }
  }

  Debug::dbg(Debug::VERY_HIGH) << "tracking precursor charges" << Debug::ENDL;

  // track the counts of precursor charges
  // with "0" being undetermined
  if (scan.precursorCharge_ < 1) {
    assert(chargeCounts_.size() > 0);
    chargeCounts_[0]++;
  }
  else {
    if ((scan.precursorCharge_ + 1) > int(chargeCounts_.size())) {
      chargeCounts_.resize(scan.precursorCharge_ + 1, 0);
      //if (verbose_) {
      //	Debug::dbg(Debug::VERY_HIGH) << "new max observed charge: " << scan.precursorCharge_ << "(scan " << curScanNum_ << ")" << endl;
      //	Debug::dbg(Debug::VERY_HIGH).flush();
      //}
    }
    chargeCounts_[scan.precursorCharge_]++;
  }

  //
  // precursor intensity determination
  //
  Debug::dbg(Debug::VERY_HIGH) << "getting precursor intensity" << Debug::ENDL;

  // go to precursor scan and try to find highest intensity around precursor m/z.
  // (could this be improved to handle more complex acquisition methods?)

  //if( numDataPoints != 0 ) { // if this isn't an empty scan

  QList<double> masses, intensities;

  // set up the parameters to read the precursor scan
  QString szFilter = "!d"; // First previous not-dependent scan
  long intensityCutoffType = 0; // No cutoff
  long intensityCutoffValue = 0; // No cutoff
  long maxNumberOfPeaks = 0; // Return all data peaks
  bool centroidResult = false; // No centroiding of the precursor
  double centroidPeakWidth = 0; // (see above: no centroiding)

  // Debug::dbg(Debug::VERY_HIGH) << "reading scan " << curScanNum_ << endl;

  int curScanNum = scanNumber;

  // the goal is to get the parent scan's info
  xrawfile2_.GetPrevMassListFromScanNum(curScanNum, szFilter, // filter
                                        intensityCutoffType, // intensityCutoffType
                                        intensityCutoffValue, // intensityCutoffValue
                                        maxNumberOfPeaks, // maxNumberOfPeaks
                                        centroidResult, // centroidResult
                                        centroidPeakWidth, // centroidingPeakWidth
                                        masses, intensities);

  // record the precursor scan number
  scan.precursorScanNumber_ = curScanNum; // set during last xrawfile2 call


  Debug::dbg(Debug::VERY_HIGH) << "precursor scan number: "
      << scan.precursorScanNumber_ << Debug::ENDL;
  Debug::dbg(Debug::VERY_HIGH) << "looking at precursor mass list for "
      << masses.size() << " data points" << Debug::ENDL;

  assert(masses.size() == intensities.size());
  double precursorIntensity = 0;
  for (int j = 0; j < masses.size(); ++j) {
    double dMass = masses[j];

    if (fabs(scan.precursorMZ_ - dMass) < 0.05) {
      if (intensities[j] > precursorIntensity) {
        precursorIntensity = intensities[j];
      }
    }
    // stop if we've gone too far
    if (dMass - scan.precursorMZ_ > 0.05) {
      break;
    }
  }

}

UVScan *ThermoInterface::getUVScan(void)
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

  int numDataPoints = -1;
  double retentionTimeInMinutes = -1;
  int channel = 0; // unused
  bool uniformTime = false; // unused
  double minObservedMZ = 0, maxObservedMZ = 0, frequency = 0,
      basePeakIntensity = 0, basePeakMZ = 0, totalIonCurrent = 0; // unused

  xrawfile2_.GetScanHeaderInfoForScanNum(curUVScanNum_, numDataPoints,
                                         retentionTimeInMinutes, minObservedMZ, // unused
                                         maxObservedMZ, // unused
                                         totalIonCurrent, // unused
                                         basePeakMZ, // unused
                                         basePeakIntensity, // unused
                                         channel, // unused
                                         uniformTime, // unused
                                         frequency // unused
  );

  curScan->setTime(retentionTimeInMinutes * 60.0);

  if (numDataPoints != 0) {

    //call average function with only one scan since normal mass list call
    //doesn't seem to always works
    QList<double> masses, intensities;
    QList<int> scanNumbers;
    scanNumbers << curUVScanNum_;
    xrawfile2_.GetAveragedMassSpectrum(scanNumbers, false, masses, intensities);

    // record the number of data point (allocates memory for arrays)
    assert(masses.size() == intensities.size());
    for (long j = 0; j < masses.size(); j++) {
      curScan->getSpectrum().addDataPoint(masses[j], intensities[j]);
    }
  }

  return curScan;
}
