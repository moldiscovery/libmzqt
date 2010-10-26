/*
 XRawfile.h
 Created on: 02/08/2010
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

#ifndef MZQT_XRAWFILE_H_
#define MZQT_XRAWFILE_H_

#include <QString>
#include <QDateTime>
#include <QHash>
#include <QByteArray>
#include <QVariant>

#include <ocidl.h>

#include <mzqt/common/IDispatch.h>

namespace mzqt {

  class XRawfileException: public Exception {

    public:
      explicit XRawfileException(const std::string &msg = "");
      virtual ~XRawfileException() throw ();
    };

  class XRawfile: public IDispatch {

  public:

    XRawfile();
    ~XRawfile();

    bool init(const QString &component="");

    void Open(const QString &inputFileName);
    void GetErrorCode(int &errorCode);
    void GetErrorMessage(QString &errorMessage);

    void GetVersionNumber(int &versionNumber);
    void SetCurrentController(int controllerType, int controllerNumber);
    void GetFirstSpectrumNumber(int &firstScanNumber);
    void GetLastSpectrumNumber(int &lastScanNumber);
    void GetStartTime(double &startTime);
    void GetEndTime(double &endTime);
    void GetInstModel(QString &model);
    void GetInstName(QString &name);
    void GetInstSoftwareVersion(QString &version);
    void GetInstHardwareVersion(QString &version);
    void GetInstrumentDescription(QString &description);
    void GetInstSerialNumber(QString &serialNumber);
    void GetFilterForScanNum(int scanNumber, QString &filter);
    void GetCreationDate(QDateTime &date);
    void GetScanHeaderInfoForScanNum(int scanNumber, int &numPackets,
        double &startTime, double &lowMass, double &highMass, double &tic,
        double &basePeakMass, double &basePeakIntensity, int &numChannel,
        bool &uniformTime, double &frequency);

    void GetLabelData(QList<double> &masses, QList<double> &intensities,
        int &scanNumber);

    void GetMassListFromScanNum(int &scanNumber, const QString &szFilter,
        int intensityCutoffType, int intensityCutoffValue,
        int maxNumberOfPeaks, bool centroidResult, double &centroidPeakWidth,
        QList<double> &masses, QList<double> &intensities);

    void GetPrevMassListFromScanNum(int &scanNumber, const QString &szFilter,
        int intensityCutoffType, int intensityCutoffValue,
        int maxNumberOfPeaks, bool centroidResult, double &centroidPeakWidth,
        QList<double> &masses, QList<double> &intensities);

    void GetMassListFromRt(double &rt, const QString &szFilter,
        int intensityCutoffType, int intensityCutoffValue,
        int maxNumberOfPeaks, bool centroidResult, double &centroidPeakWidth,
        QList<double> &masses, QList<double> &intensities);

    void GetAveragedMassSpectrum(const QList<int> &scanNumbers,
        bool centroidResult, QList<double> &masses, QList<double> &intensities);

    void GetTrailerExtraValueForScanNum(int nScanNumber,
        const QString &szLabel, QVariant &value);

    void GetFilters(QStringList &filters);



    void GetNumberOfControllersOfType(int controllerType, int &number);

  protected:
    void checkForError(const QString &callName);

    void getString(const char *dispName, QString &string,
        const QVariant &inParam = QVariant());

    void getMassList(const char *methodName, int &scanNumber,
        const QString &szFilter, int intensityCutoffType,
        int intensityCutoffValue, int maxNumberOfPeaks, bool centroidResult,
        double &centroidPeakWidth, QList<double> &masses,
        QList<double> &intensities);

    DISPID dispIDofName(const QByteArray &name, ::IDispatch *disp);

    ::IDispatch *disp;
    QHash<QByteArray, DISPID> dispIDs;
  };

}

#endif /* MZQT_XRAWFILE_H_ */
