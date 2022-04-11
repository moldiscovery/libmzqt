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

#include "IDispatch.h"

#ifdef __GNUC__
#define MZQTDLL_API
#else
#ifdef MZQTDLL_EXPORTS
#define MZQTDLL_API __declspec(dllexport)
#else
#define MZQTDLL_API __declspec(dllimport)
#endif
#endif

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

    MZQTDLL_API bool init(const QString &component="");

    MZQTDLL_API void Open(const QString &inputFileName);
    MZQTDLL_API void GetErrorCode(int &errorCode);
    MZQTDLL_API void GetErrorMessage(QString &errorMessage);

    MZQTDLL_API void GetVersionNumber(int &versionNumber);
    MZQTDLL_API void SetCurrentController(int controllerType, int controllerNumber);
    MZQTDLL_API void GetFirstSpectrumNumber(int &firstScanNumber);
    MZQTDLL_API void GetLastSpectrumNumber(int &lastScanNumber);
    MZQTDLL_API void GetStartTime(double &startTime);
    MZQTDLL_API void GetEndTime(double &endTime);
    MZQTDLL_API void GetInstModel(QString &model);
    MZQTDLL_API void GetInstName(QString &name);
    MZQTDLL_API void GetInstSoftwareVersion(QString &version);
    MZQTDLL_API void GetInstHardwareVersion(QString &version);
    MZQTDLL_API void GetInstrumentDescription(QString &description);
    MZQTDLL_API void GetInstSerialNumber(QString &serialNumber);
    MZQTDLL_API void GetFilterForScanNum(int scanNumber, QString &filter);
    MZQTDLL_API void GetCreationDate(QDateTime &date);
    MZQTDLL_API void GetScanHeaderInfoForScanNum(int scanNumber, int &numPackets,
        double &startTime, double &lowMass, double &highMass, double &tic,
        double &basePeakMass, double &basePeakIntensity, int &numChannel,
        bool &uniformTime, double &frequency);

    MZQTDLL_API void GetLabelData(QList<double> &masses, QList<double> &intensities,
        int &scanNumber);

    MZQTDLL_API void GetMassListFromScanNum(int &scanNumber, const QString &szFilter,
        int intensityCutoffType, int intensityCutoffValue,
        int maxNumberOfPeaks, bool centroidResult, double &centroidPeakWidth,
        QList<double> &masses, QList<double> &intensities);

    MZQTDLL_API void GetPrevMassListFromScanNum(int &scanNumber, const QString &szFilter,
        int intensityCutoffType, int intensityCutoffValue,
        int maxNumberOfPeaks, bool centroidResult, double &centroidPeakWidth,
        QList<double> &masses, QList<double> &intensities);

    MZQTDLL_API void GetMassListFromRt(double &rt, const QString &szFilter,
        int intensityCutoffType, int intensityCutoffValue,
        int maxNumberOfPeaks, bool centroidResult, double &centroidPeakWidth,
        QList<double> &masses, QList<double> &intensities);

    MZQTDLL_API void GetAveragedMassSpectrum(const QList<int> &scanNumbers,
        bool centroidResult, QList<double> &masses, QList<double> &intensities);

    MZQTDLL_API void GetTrailerExtraValueForScanNum(int nScanNumber,
        const QString &szLabel, QVariant &value);

    MZQTDLL_API void GetFilters(QStringList &filters);

    MZQTDLL_API void GetNumberOfControllersOfType(int controllerType, int &number);

    MZQTDLL_API void GetChromatogram(long chroTrace, QVector<double> &times,
                                     QVector<double> &intensities);

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
