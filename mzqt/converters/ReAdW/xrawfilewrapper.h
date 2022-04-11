#ifndef XRAWFILEWRAPPER_H
#define XRAWFILEWRAPPER_H

#define WIN32_LEAN_AND_MEAN             // Excluir material rara vez utilizado de encabezados de Windows
// Archivos de encabezado de Windows
#include <Windows.h>

// Archivos de encabezado en tiempo de ejecución de C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#include <tchar.h>
#include "xrawfile2_x64.tlh"
#include <mzqt/common/Exception.h>
#include <mzqt/common/cominterface.h>

namespace mzqt {

/**
 * @brief The XRawfileWrapperException class, Exception thrown by the XRawfileWrapper
 */
class XRawfileWrapperException: public Exception {

  public:
    explicit XRawfileWrapperException(const std::string &msg = "");
    virtual ~XRawfileWrapperException();
  };

/**
 * @brief The XRawfileWrapper class, Wrapper for accessing the XRawfile COM object
 * without the Qt activeX mechanism. Use direct calls instead of using IDispatch interface
 */
class XRawfileWrapper: public COMInterface {

public:
  XRawfileWrapper();
  ~XRawfileWrapper();
  IXRawfile5 *iface();

  void Open(const QString& fileName);
  void GetErrorCode(long &errorCode);
  void GetErrorMessage(QString &s);
  void checkForError();
  void GetVersionNumber(long &var);
  void SetCurrentController(long controllerType, long controllerNumber);
  void GetFirstSpectrumNumber(long &firstScanNumber);
  void GetLastSpectrumNumber(long &lastScanNumber);
  void GetNumberOfControllersOfType(long controllerType, long &number);
  void GetStartTime(double &startTime);
  void GetEndTime(double &endTime);
  void GetInstModel(QString &s);
  void GetInstName(QString &s);
  void GetInstSoftwareVersion(QString &s);
  void GetInstHardwareVersion(QString &s);
  void GetInstrumentDescription(QString &s);
  void GetInstSerialNumber(QString &s);
  void GetFilterForScanNum(long scanNumber, QString &s);
  void GetCreationDate(double &date);
  void GetScanHeaderInfoForScanNum(long scanNumber, long &numPackets,
      double &startTime, double &lowMass, double &highMass, double &tic,
      double &basePeakMass, double &basePeakIntensity, long &numChannel,
      long &uniformTime, double &frequency);
  void GetLabelData(QVector<double> &masses, QVector<double> &intensities,
      long &scanNum);
  void GetMassListFromScanNum(long &scanNumber, const QString &szFilter,
      long intensityCutoffType, long intensityCutoffValue, long maxNumberOfPeaks,
      long centroidResult, double &centroidPeakWidth, QVector<double> &masses,
      QVector<double> &intensities);
  void GetPrevMassListFromScanNum(long &scanNumber, const QString &szFilter,
      long intensityCutoffType, long intensityCutoffValue, long maxNumberOfPeaks,
      long centroidResult, double &centroidPeakWidth, QVector<double> &masses,
      QVector<double> &intensities);
  void GetAveragedMassSpectrum(const QVector<long> &scanNumbers,
      bool centroidResult, QVector<double> &masses, QVector<double> &intensities);
  void GetChromatogram(long chroTrace, QVector<double> &times,
                       QVector<double> &intensities);
  void GetTrailerExtraValueForScanNum(long nScanNumber,
      const QString &szLabel, QVariant &value);
  void GetIsolationWidthForScanNum(long scanNum, long msOrder, double &isolationWidth);

protected:
  IXRawfile5 *iface_;
};

}
#endif // XRAWFILEWRAPPER_H
