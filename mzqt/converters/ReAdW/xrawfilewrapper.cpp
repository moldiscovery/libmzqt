
#include "xrawfilewrapper.h"

#include <qaxtypes.h>

using namespace mzqt;

typedef struct _datapeak
{
  double dMass;
  double dIntensity;
} DataPeak;


XRawfileWrapperException::XRawfileWrapperException(const std::string &msg) :
  Exception(msg)
{
  msg_ = "XRAWFILE WRAPPER ERROR(" + msg + ")";
}

XRawfileWrapperException::~XRawfileWrapperException()
{
}

///////////////////////////////////////////////////////////////////////////////
XRawfileWrapper::XRawfileWrapper() :
  iface_()
{
  IXRawfile5 *pPeakFilter = 0;

  HRESULT hr = CoCreateInstance(CLSID_MSFileReader_XRawfile, NULL,
                                CLSCTX_INPROC_SERVER, IID_IXRawfile4,
                                (void**) &pPeakFilter);

  if (FAILED(hr)) {
    throw DispatchException(hr, "CLSID_MassSpecDataReader instance creation");
  }

  iface_ = pPeakFilter;
}

XRawfileWrapper::~XRawfileWrapper()
{
  if (iface_)
    iface_->Release();
}

IXRawfile5 *XRawfileWrapper::iface()
{
  if(!iface_)
    throw XRawfileWrapperException("Null XRawfileWrapper interface");

  return iface_;
}

void XRawfileWrapper::Open(const QString& fileName)
{
  BSTR bstrFilePath = QStringToBSTR(fileName);

  HRESULT hr = iface()->Open(bstrFilePath);

  SysFreeString(bstrFilePath);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "Failed to open data file");
  }
}

void XRawfileWrapper::GetErrorCode(long &errorCode)
{
  errorCode = 0;

  long e;
  HRESULT hr = iface()->GetErrorCode(&e);

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetErrorCode Failed");
  }

  errorCode = e;
}

void XRawfileWrapper::GetErrorMessage(QString &s)
{
  s.clear();
  BSTR bstr = NULL;

  HRESULT hr = iface()->GetErrorMessage(&bstr);

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetErrorMessage Failed");
  }

  if(bstr != NULL) {
    s = BSTRToQString(bstr);
    ::SysFreeString(bstr);
  }
}

void XRawfileWrapper::checkForError()
{
  long errorCode = 0;
  GetErrorCode(errorCode);

  if (errorCode != 0) {
    QString errorMessage;
    GetErrorMessage(errorMessage);
    throw XRawfileWrapperException(QString("code: %1, message: %2"
                                    "").arg(errorCode).arg(errorMessage).toStdString());
  }
}

void XRawfileWrapper::GetVersionNumber(long &var)
{
  var = 0;

  long e;
  HRESULT hr = iface()->GetVersionNumber(&e);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetVersionNumber Failed");
  }

  var = e;
}

void XRawfileWrapper::SetCurrentController(long controllerType, long controllerNumber)
{
  HRESULT hr = iface()->SetCurrentController(controllerType, controllerNumber);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "SetCurrentController Failed");
  }
}

void XRawfileWrapper::GetFirstSpectrumNumber(long &firstScanNumber)
{
  firstScanNumber = 0;

  long e;
  HRESULT hr = iface()->GetFirstSpectrumNumber(&e);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetFirstSpectrumNumber Failed");
  }

  firstScanNumber = e;

}

void XRawfileWrapper::GetLastSpectrumNumber(long &lastScanNumber)
{
  lastScanNumber = 0;

  long e;
  HRESULT hr = iface()->GetLastSpectrumNumber(&e);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetLastSpectrumNumber Failed");
  }

  lastScanNumber = e;
}

void XRawfileWrapper::GetIsolationWidthForScanNum(long scanNum, long msOrder, double &isolationWidth)
{
  isolationWidth = 0.0;

  double e;
  HRESULT hr = iface()->GetIsolationWidthForScanNum(scanNum, msOrder, &e);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetIsolationWidthForScanNum Failed");
  }

  isolationWidth = e;
}
void XRawfileWrapper::GetNumberOfControllersOfType(long controllerType, long &number)
{
  number = 0;

  long e;
  HRESULT hr = iface()->GetNumberOfControllersOfType(controllerType, &e);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetNumberOfControllersOfType Failed");
  }

  number = e;
}

void XRawfileWrapper::GetStartTime(double &startTime)
{
  startTime = 0;

  double e;
  HRESULT hr = iface()->GetStartTime(&e);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetStartTime Failed");
  }

  startTime = e;
}

void XRawfileWrapper::GetEndTime(double &endTime)
{
  endTime = 0;

  double e;
  HRESULT hr = iface()->GetEndTime(&e);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetEndTime Failed");
  }

  endTime = e;
}

void XRawfileWrapper::GetInstModel(QString &s)
{
  s.clear();
  BSTR bstr = NULL;

  HRESULT hr = iface()->GetInstModel(&bstr);

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetInstModel Failed");
  }

  if(bstr != NULL) {
    s = BSTRToQString(bstr);
    ::SysFreeString(bstr);
  }
}

void XRawfileWrapper::GetInstName(QString &s)
{
  s.clear();
  BSTR bstr = NULL;

  HRESULT hr = iface()->GetInstName(&bstr);

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetInstName Failed");
  }

  if(bstr != NULL) {
    s = BSTRToQString(bstr);
    ::SysFreeString(bstr);
  }
}

void XRawfileWrapper::GetInstSoftwareVersion(QString &s)
{
  s.clear();
  BSTR bstr = NULL;

  HRESULT hr = iface()->GetInstSoftwareVersion(&bstr);

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetInstSoftwareVersion Failed");
  }

  if(bstr != NULL) {
    s = BSTRToQString(bstr);
    ::SysFreeString(bstr);
  }
}

void XRawfileWrapper::GetInstHardwareVersion(QString &s)
{
  s.clear();
  BSTR bstr = NULL;

  HRESULT hr = iface()->GetInstHardwareVersion(&bstr);

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetInstHardwareVersion Failed");
  }

  if(bstr != NULL) {
    s = BSTRToQString(bstr);
    ::SysFreeString(bstr);
  }
}

void XRawfileWrapper::GetInstrumentDescription(QString &s)
{
  s.clear();
  BSTR bstr = NULL;

  HRESULT hr = iface()->GetInstrumentDescription(&bstr);

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetInstrumentDescription Failed");
  }

  if(bstr != NULL) {
    s = BSTRToQString(bstr);
    ::SysFreeString(bstr);
  }
}

void XRawfileWrapper::GetInstSerialNumber(QString &s)
{
  s.clear();
  BSTR bstr = NULL;

  HRESULT hr = iface()->GetInstSerialNumber(&bstr);

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetInstSerialNumber Failed");
  }

  if(bstr != NULL) {
    s = BSTRToQString(bstr);
    ::SysFreeString(bstr);
  }
}

void XRawfileWrapper::GetFilterForScanNum(long scanNumber, QString &s)
{
  s.clear();
  BSTR bstr = NULL;

  HRESULT hr = iface()->GetFilterForScanNum(scanNumber, &bstr);

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetFilterForScanNum Failed");
  }

  if(bstr != NULL) {
    s = BSTRToQString(bstr);
    ::SysFreeString(bstr);
  }
}

void XRawfileWrapper::GetCreationDate(double &date)
{
  date = 0;

  DATE e;
  HRESULT hr = iface()->GetCreationDate(&e);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetCreationDate Failed");
  }

  date = e;
}

void XRawfileWrapper::GetScanHeaderInfoForScanNum(long scanNumber, long &numPackets,
                                                  double &startTime, double &lowMass, double &highMass, double &tic,
                                                  double &basePeakMass, double &basePeakIntensity, long &numChannel,
                                                  long &uniformTime, double &frequency)
{
  numPackets = numChannel = uniformTime = 0.0;
  startTime = lowMass = highMass = tic = basePeakMass = basePeakIntensity = frequency = 0.0;

  HRESULT hr = iface()->GetScanHeaderInfoForScanNum(scanNumber, &numPackets, &startTime, &lowMass,
                                                   &highMass, &tic, &basePeakMass, &basePeakIntensity,
                                                   &numChannel, &uniformTime, &frequency);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetScanHeaderInfoForScanNum Failed");
  }

}

void XRawfileWrapper::GetLabelData(QVector<double> &masses, QVector<double> &intensities,
                                   long &scanNum)
{
  masses.clear();
  intensities.clear();

  VARIANT varLabels;
  ::VariantInit(&varLabels);
  _variant_t vSpecData;
  HRESULT hr = iface()->GetLabelData(&varLabels, NULL, &scanNum);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetLabelData Failed");
  }

  vSpecData.Attach(varLabels);

  long dataPoints = vSpecData.parray->rgsabound[0].cElements;
  masses.resize(dataPoints);
  intensities.resize(dataPoints);

  double* pdval = (double*) vSpecData.parray->pvData;
  for(long i=0; i < dataPoints; ++i) {
    masses[i] = (double) pdval[(i*6)+0];
    intensities[i] = (double) pdval[(i*6)+1];
  }
}

void XRawfileWrapper::GetMassListFromScanNum(long &scanNumber, const QString &szFilter,
                                             long intensityCutoffType, long intensityCutoffValue, long maxNumberOfPeaks,
                                             long centroidResult, double &centroidPeakWidth, QVector<double> &masses,
                                             QVector<double> &intensities)
{
  centroidPeakWidth = 0;
  masses.clear();
  intensities.clear();

  BSTR bstrFilter = QStringToBSTR(szFilter);
  VARIANT varMassList;
  // initiallize variant to VT_EMPTY
  VariantInit(&varMassList);

  VARIANT varPeakFlags; // unused
  // initiallize variant to VT_EMPTY
  VariantInit(&varPeakFlags);

  long dataPoints = 0;
  HRESULT hr = iface()->GetMassListFromScanNum(&scanNumber, bstrFilter, intensityCutoffType,
                                              intensityCutoffValue, maxNumberOfPeaks, centroidResult,
                                              &centroidPeakWidth, &varMassList, &varPeakFlags, &dataPoints);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetMassListFromScanNum Failed");
  }

  if (dataPoints) {

    int dim = varMassList.parray->rgsabound[0].cElements;
    masses.reserve(dim);
    intensities.reserve(dim);

    SAFEARRAY *parray = varMassList.parray;
    double *pdval = (double *) parray->pvData;

    for (int inx = 0; inx < dim; inx++) {
      double dMass = (double) pdval[((inx) * 2) + 0];
      double dInt = (double) pdval[((inx) * 2) + 1];

      masses.push_back(dMass);
      intensities.push_back(dInt);
    }
  }

  if (varMassList.vt != VT_EMPTY) {
    SAFEARRAY FAR* psa = varMassList.parray;
    varMassList.parray = NULL;
    // Delete the SafeArray
    SafeArrayDestroy( psa);
  }

  if (varPeakFlags.vt != VT_EMPTY) {
    SAFEARRAY FAR* psa = varPeakFlags.parray;
    varPeakFlags.parray = NULL;
    // Delete the SafeArray
    SafeArrayDestroy( psa);
  }

  VariantClear(&varPeakFlags);
  VariantClear(&varMassList);
  ::SysReleaseString(bstrFilter);
}

void XRawfileWrapper::GetPrevMassListFromScanNum(long &scanNumber, const QString &szFilter,
                                                 long intensityCutoffType, long intensityCutoffValue, long maxNumberOfPeaks,
                                                 long centroidResult, double &centroidPeakWidth, QVector<double> &masses,
                                                 QVector<double> &intensities)
{
  centroidPeakWidth = 0;
  masses.clear();
  intensities.clear();

  BSTR bstrFilter = QStringToBSTR(szFilter);
  VARIANT varMassList;
  // initiallize variant to VT_EMPTY
  VariantInit(&varMassList);

  VARIANT varPeakFlags; // unused
  // initiallize variant to VT_EMPTY
  VariantInit(&varPeakFlags);

  long dataPoints = 0;
  HRESULT hr = iface()->GetPrevMassListFromScanNum(&scanNumber, bstrFilter, intensityCutoffType,
                                                  intensityCutoffValue, maxNumberOfPeaks, centroidResult,
                                                  &centroidPeakWidth, &varMassList, &varPeakFlags, &dataPoints);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetPrevMassListFromScanNum Failed");
  }

  if (dataPoints) {

    int dim = varMassList.parray->rgsabound[0].cElements;
    masses.reserve(dim);
    intensities.reserve(dim);

    SAFEARRAY *parray = varMassList.parray;
    double *pdval = (double *) parray->pvData;

    for (int inx = 0; inx < dim; inx++) {
      double dMass = (double) pdval[((inx) * 2) + 0];
      double dInt = (double) pdval[((inx) * 2) + 1];

      masses.push_back(dMass);
      intensities.push_back(dInt);
    }
  }

  if (varMassList.vt != VT_EMPTY) {
    SAFEARRAY FAR* psa = varMassList.parray;
    varMassList.parray = NULL;
    // Delete the SafeArray
    SafeArrayDestroy( psa);
  }

  if (varPeakFlags.vt != VT_EMPTY) {
    SAFEARRAY FAR* psa = varPeakFlags.parray;
    varPeakFlags.parray = NULL;
    // Delete the SafeArray
    SafeArrayDestroy( psa);
  }

  VariantClear(&varPeakFlags);
  VariantClear(&varMassList);
  ::SysReleaseString(bstrFilter);
}

void XRawfileWrapper::GetAveragedMassSpectrum(const QVector<long> &scanNumbers,
                                              bool centroidResult, QVector<double> &masses,
                                              QVector<double> &intensities)
{
  if (scanNumbers.size() == 0)
    throw XRawfileWrapperException("Unable to get average spectrum: no scan");

  masses.clear();
  intensities.clear();

  long *sNumbers = new long[static_cast<size_t>(scanNumbers.size())];
  for (int i = 0; i < scanNumbers.size(); ++i) {
    sNumbers[i] = scanNumbers.at(i);
  }

  VARIANT_BOOL b = centroidResult ? TRUE : FALSE;

  VARIANT varMassList;
  // initiallize variant to VT_EMPTY
  VariantInit(&varMassList);

  VARIANT varPeakFlags; // unused
  // initiallize variant to VT_EMPTY
  VariantInit(&varPeakFlags);

  long dataPoints = 0;
  HRESULT hr = iface()->GetAveragedMassSpectrum(sNumbers, scanNumbers.size(), b,
                                               &varMassList, &varPeakFlags, &dataPoints);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetAveragedMassSpectrum Failed");
  }

  // Get a pointer to the SafeArray
  SAFEARRAY FAR* psa = varMassList.parray;
  DataPeak* pDataPeaks = NULL;
  SafeArrayAccessData(psa, (void**)(&pDataPeaks));


  masses.resize(dataPoints);
  intensities.resize(dataPoints);
  // record mass list information in scan object
  for (long j=0; j<dataPoints; j++) {
    masses[j] = pDataPeaks[j].dMass;
    intensities[j] = pDataPeaks[j].dIntensity;
  }


  // cleanup
  SafeArrayUnaccessData(psa); // Release the data handle
  VariantClear(&varMassList); // Delete all memory associated with the variant
  VariantClear(&varPeakFlags); // and reinitialize to VT_EMPTY

  if( varMassList.vt != VT_EMPTY ) {
    SAFEARRAY FAR* psa = varMassList.parray;
    varMassList.parray = NULL;
    SafeArrayDestroy( psa ); // Delete the SafeArray
  }

  if(varPeakFlags.vt != VT_EMPTY ) {
    SAFEARRAY FAR* psa = varPeakFlags.parray;
    varPeakFlags.parray = NULL;
    SafeArrayDestroy( psa ); // Delete the SafeArray
  }

  delete[] sNumbers; //free the allocated scan numbers
}

void XRawfileWrapper::GetChromatogram(long chroTrace, QVector<double> &times,
                                      QVector<double> &intensities)
{
  times.clear();
  intensities.clear();

  // example for GetChroData to return the MS TIC trace
  typedef struct _datapeak
  {
    double dTime;
    double dIntensity;
  } ChroDataPeak;

  long nArraySize = 0;
  double startTime = 0.0, endTime = 0.0, delay = 0.0;
  long smoothingType = 0, smoothingValue = 0;
  QString massRanges1 = "", massRanges2 = "", filter = "";
  long nChroType2 = 0, nChroOperator = 0, nChroType1 = 0;

  BSTR bstrFilter = QStringToBSTR(filter);
  BSTR bstrMassRanges1 = QStringToBSTR(massRanges1);
  BSTR bstrMassRanges2 = QStringToBSTR(massRanges2);

  VARIANT varChroData;
  VariantInit(&varChroData);
  VARIANT varPeakFlags;
  VariantInit(&varPeakFlags);

  HRESULT hr = iface()->GetChroData(nChroType1, nChroOperator, nChroType2, bstrFilter, bstrMassRanges1,
                                   bstrMassRanges2, delay, &startTime, &endTime, smoothingType,
                                   smoothingValue, &varChroData, &varPeakFlags, &nArraySize);

  checkForError();

  if (FAILED(hr)) {
    throw DispatchException(hr, "GetChroData Failed");
  }

  if( nArraySize ) {

    // Get a pointer to the SafeArray
    SAFEARRAY FAR* psa = varChroData.parray;
    ChroDataPeak* pDataPeaks = NULL;
    SafeArrayAccessData( psa, (void**)(&pDataPeaks) );
    for( long j=0; j<nArraySize; j++ )
    {
      double dTime = pDataPeaks[j].dTime;
      double dIntensity = pDataPeaks[j].dIntensity;
      times.append(dTime);
      intensities.append(dIntensity);
    }
    // Release the data handle
    SafeArrayUnaccessData( psa );
  }

  if(varChroData.vt != VT_EMPTY )
  {
    SAFEARRAY FAR* psa =
        varChroData.parray;
    varChroData.parray = NULL;
    // Delete the SafeArray
    SafeArrayDestroy( psa );
  }

  if(varPeakFlags.vt != VT_EMPTY )
  {
    SAFEARRAY FAR* psa =
        varPeakFlags.parray;
    varPeakFlags.parray = NULL;
    // Delete the SafeArray
    SafeArrayDestroy( psa );
  }

  ::SysFreeString(bstrFilter);
  ::SysFreeString(bstrMassRanges1);
  ::SysFreeString(bstrMassRanges2);
}

void XRawfileWrapper::GetTrailerExtraValueForScanNum(long nScanNumber,
    const QString &szLabel, QVariant &value)
{
  value.clear();

  BSTR bstrLabel = QStringToBSTR(szLabel);

  VARIANT varValue;
  ::VariantInit(&varValue);


  HRESULT hr = iface()->GetTrailerExtraValueForScanNum(nScanNumber, bstrLabel, &varValue);

  checkForError();

  if (FAILED(hr))
    throw DispatchException(hr, "GetTrailerExtraValueForScanNum Failed");

  value = VARIANTToQVariant(varValue, "");
  ::VariantClear(&varValue);
  SysFreeString(bstrLabel); //free the string
}
