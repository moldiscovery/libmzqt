/*
 XRawfile.cpp
 Created on: 02/08/2010
 Author: Fabien Fontaine

 Access Thermo RAW files through XRawfile2.dll and COM. For
 more details about how to get and use the Thermo library go to:
 http://sjsupport.thermofinnigan.com/public/detail.asp?id=586

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

#include <iostream>
#include <cassert>

#include <mzqt/common/Debug.h>

#include <QUuid>
#include <QString>
#include <QAxObject>

#include "XRawfile.h"

#define _WINDOWS_
#include <qaxtypes.h>

#ifdef USE_MMGR_MEMORY_CHECK
#include <mmgr.h>
#endif

#define DEBUG_GET_LABEL_DATA 0

using namespace mzqt;

XRawfileException::XRawfileException(const std::string &msg) :
  Exception(msg)
{
  msg_ = "XRAWFILE ERROR(" + msg + ")";
}

XRawfileException::~XRawfileException() throw ()
{
}

XRawfile::XRawfile() :
  IDispatch(), disp(0)
{
}

XRawfile::~XRawfile()
{
  if (disp)
    disp->Release();
}

bool XRawfile::init(const QString &component)
{
  bool result = true;

  if (component != "") {
    if (setComponent(component) == true) {
      Debug::msg() << component << " interface initialized.";
    }
    else {
      qWarning() << "Unable to initialize XRawfile interface from"
        " component: " << component;
      result = false;
    }
  }
  else if (setComponent("MSFileReader.XRawfile.1") == true) {
    Debug::msg() << "MSFileReader interface initialized.";
  }
  else if (setComponent("XRawfile.XRawfile.1") == true) {
    Debug::msg() << "Xcalibur 2.0 interface initialized.";
  }
  else {
    //throw something because nothing has been initialized?
    qWarning() << "Unable to initialize XRawfile interface; "
      "check installation";
    result = false;
  }

  if (result == true) {
    //dispatch interface for cases that Qt can't handle directly
    HRESULT hres = idispatch_->queryInterface(QUuid(IID_IDispatch),
                                              (void**) &disp);
    if (FAILED(hres)) {
      qWarning() << "Unable to initialize the IDispatch interface";
      result = false;
    }
  }

  return result;
}

DISPID XRawfile::dispIDofName(const QByteArray &name, ::IDispatch *disp)
{
  DISPID dispid = dispIDs.value(name, DISPID_UNKNOWN);
  if (dispid == DISPID_UNKNOWN) {
    // get the Dispatch ID from the object
    QString unicodeName = QLatin1String(name);
    OLECHAR *names = (TCHAR*) unicodeName.utf16();
    HRESULT hres = disp->GetIDsOfNames(IID_NULL, &names, 1,
                                       LOCALE_USER_DEFAULT, &dispid);

    if (dispid == DISPID_UNKNOWN) {
      throw IDispatchException("Unknown DispId for name: "
          + std::string(name.data()));
    }

    if (FAILED(hres))
      THROW_MZQT_IDISPATCHEXCEPT_HRES(hres); //more general error

    if (dispid != DISPID_UNKNOWN)
      dispIDs.insert(name, dispid);
    else {

    }
  }
  return dispid;
}

void XRawfile::Open(const QString &fileName)
{
  const char s[] = "Open(const QString &)";
  QVariant r = idispatch_->dynamicCall(s, fileName);
  checkForError(s);
}

void XRawfile::GetErrorCode(int &errorCode)
{
  QList<QVariant> args;
  args << QVariant(QVariant::Int);

  const char *s = "GetErrorCode(int&)";
  idispatch_->dynamicCall(s, args);

  if (args.value(0).isValid() == false)
    throw IDispatchException(QString("call failed: %1").arg(s).toStdString());

  errorCode = args.value(0).toInt();
}

void XRawfile::GetErrorMessage(QString &errorMessage)
{
  getString("GetErrorMessage", errorMessage);
}

void XRawfile::checkForError(const QString &callName)
{
  int errorCode = 0;
  GetErrorCode(errorCode);

  if (errorCode != 0) {
    QString errorMessage;
    GetErrorMessage(errorMessage);
    throw XRawfileException(QString("call: %1 code: %2, message: %3"
      "").arg(callName).arg(errorCode).arg(errorMessage).toStdString());
  }
}

void XRawfile::GetVersionNumber(int &versionNumber)
{
  QList<QVariant> args;
  args << QVariant(QVariant::Int);

  idispatch_->dynamicCall("GetVersionNumber(int&)", args);
  checkForError("GetVersionNumber(int&)");
  versionNumber = args.value(0).toInt();
}

void XRawfile::SetCurrentController(int controllerType, int controllerNumber)
{
  idispatch_->dynamicCall("SetCurrentController(int, int)", controllerType,
                          controllerNumber);
  checkForError("SetCurrentController(int, int)");
}

void XRawfile::GetFirstSpectrumNumber(int &firstScanNumber)
{
  QList<QVariant> args;
  args << QVariant(QVariant::Int);

  idispatch_->dynamicCall("GetFirstSpectrumNumber(int&)", args);
  checkForError("GetFirstSpectrumNumber(int&)");
  firstScanNumber = args.value(0).toInt();

}

void XRawfile::GetLastSpectrumNumber(int &lastScanNumber)
{
  QList<QVariant> args;
  args << QVariant(QVariant::Int);

  idispatch_->dynamicCall("GetLastSpectrumNumber(int &)", args);
  checkForError("GetLastSpectrumNumber(int &)");
  lastScanNumber = args.value(0).toInt();
}

void XRawfile::GetNumberOfControllersOfType(int controllerType, int &number)
{
  QList<QVariant> args;
  args << controllerType << QVariant(QVariant::Int);

  idispatch_->dynamicCall("GetNumberOfControllersOfType(int, int &)", args);
  checkForError("GetNumberOfControllersOfType(int, int &)");
  number = args.value(1).toInt();
}

void XRawfile::GetStartTime(double &startTime)
{
  QList<QVariant> args;
  args << QVariant(QVariant::Double);

  idispatch_->dynamicCall("GetStartTime(double &)", args);
  checkForError("GetStartTime(double &)");
  startTime = args.value(0).toDouble();

}

void XRawfile::GetEndTime(double &endTime)
{
  QList<QVariant> args;
  args << QVariant(QVariant::Double);

  idispatch_->dynamicCall("GetEndTime(double &)", args);
  checkForError("GetEndTime(double &)");
  endTime = args.value(0).toDouble();

}

void XRawfile::getString(const char *dispName, QString &string,
    const QVariant &inParam)
{
  string.clear();

  //The XRawFile API expect BSTR that have been initialize to NULL
  //while Qt provide BSTR initialized with an allocation of size 0
  //So the dynamicCall method from Qt can't be used
  DISPID dispid = dispIDofName(dispName, disp);
  assert(dispid != DISPID_UNKNOWN);

  DISPPARAMS params;

  //out string parameter
  BSTR bstrInstModel = NULL;
  VARIANTARG varg[2];
  ::VariantInit(&varg[0]);
  ::VariantInit(&varg[1]);

  varg[0].vt = VT_BYREF | VT_BSTR;
  varg[0].pbstrVal = &bstrInstModel;

  if (inParam.isValid() == false) {
    params.cArgs = 1;
  }
  else {
    //input parameter, we expect the parameter to be the first
    //in the dispinterface specification and therefore to be the
    //last in the VARIANT list
    QVariantToVARIANT(inParam, varg[1]);
    params.cArgs = 2;
  }

  params.rgdispidNamedArgs = 0;
  params.cNamedArgs = 0;
  params.rgvarg = varg;

  EXCEPINFO excepinfo;
  memset(&excepinfo, 0, sizeof(excepinfo));
  UINT argerr = 0;

  VARIANTARG res;
  ::VariantInit(&res);

  HRESULT hres = disp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                              DISPATCH_METHOD | DISPATCH_PROPERTYGET, &params,
                              &res, &excepinfo, &argerr);

  checkForError(dispName);

  if (FAILED(hres))
    THROW_MZQT_IDISPATCHEXCEPT_HRES(hres);

  QVariant vs = VARIANTToQVariant(varg[0], QByteArray("QString"));

  string = vs.toString();

  ::SysFreeString(bstrInstModel);
}

void XRawfile::GetInstModel(QString &model)
{
  getString("GetInstModel", model);
}

void XRawfile::GetInstName(QString &name)
{
  getString("GetInstName", name);
}

void XRawfile::GetInstSoftwareVersion(QString &version)
{
  getString("GetInstSoftwareVersion", version);
}

void XRawfile::GetInstHardwareVersion(QString &version)
{
  getString("GetInstHardwareVersion", version);
}

void XRawfile::GetInstrumentDescription(QString &description)
{
  getString("GetInstrumentDescription", description);
}

void XRawfile::GetInstSerialNumber(QString &serialNumber)
{
  getString("GetInstSerialNumber", serialNumber);
}

void XRawfile::GetFilterForScanNum(int scanNumber, QString &filter)
{
  getString("GetFilterForScanNum", filter, scanNumber);
}

void XRawfile::GetCreationDate(QDateTime &date)
{
  QList<QVariant> args;
  args << QVariant(QVariant::DateTime);

  idispatch_->dynamicCall("GetCreationDate(QDateTime &)", args);
  checkForError("GetCreationDate");
  date = args.value(0).toDateTime();
}

void XRawfile::GetScanHeaderInfoForScanNum(int scanNumber, int &numPackets,
    double &startTime, double &lowMass, double &highMass, double &tic,
    double &basePeakMass, double &basePeakIntensity, int &numChannel,
    bool &uniformTime, double &frequency)
{
  QList<QVariant> args;
  QVariant vd(QVariant::Double);
  QVariant vi(QVariant::Int);
  QVariant vb(QVariant::Bool);

  args << scanNumber << vi << vd << vd << vd << vd << vd << vd << vi << vi
      << vd;

  idispatch_->dynamicCall("GetScanHeaderInfoForScanNum("
    "int, int&, double&, double&, double&, double&, double&, double&,"
    "int&, int&, double&"
    ")", args);

  checkForError("GetScanHeaderInfoForScanNum");

  numPackets = args.value(1).toInt();
  startTime = args.value(2).toDouble();
  lowMass = args.value(3).toDouble();
  highMass = args.value(4).toDouble();
  tic = args.value(5).toDouble();
  basePeakMass = args.value(6).toDouble();
  basePeakIntensity = args.value(7).toDouble();
  numChannel = args.value(8).toInt();
  uniformTime = args.value(9).toBool();
  frequency = args.value(10).toDouble();

}

void XRawfile::GetLabelData(QList<double> &masses, QList<double> &intensities,
    int &scanNumber)
{
  masses.clear();
  intensities.clear();

  VARIANT varLabels;
  ::VariantInit(&varLabels);

  //get dispId
  const char *name = "GetLabelData";
  DISPID dispid = dispIDofName(name, disp);
  assert(dispid != DISPID_UNKNOWN);

  //init the variants
  VARIANTARG varg[3];
  ::VariantInit(&varg[0]);
  ::VariantInit(&varg[1]);
  ::VariantInit(&varg[2]);

  varg[0].vt = VT_BYREF | VT_I4;
  varg[0].plVal = (long *) &scanNumber;
  varg[1].vt = VT_BYREF | VT_VARIANT;
  varg[1].pvarVal = NULL;
  varg[2].vt = VT_BYREF | VT_VARIANT;
  varg[2].pvarVal = &varLabels;

  //set up the parameter
  DISPPARAMS params;
  params.cArgs = 3;
  params.rgdispidNamedArgs = 0;
  params.cNamedArgs = 0;
  params.rgvarg = varg;

  //init the exception
  EXCEPINFO excepinfo;
  memset(&excepinfo, 0, sizeof(excepinfo));
  UINT argerr = 0;

  //init the result
  VARIANTARG res;
  ::VariantInit(&res);

  HRESULT hres = disp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                              DISPATCH_METHOD | DISPATCH_PROPERTYGET, &params,
                              &res, &excepinfo, &argerr);

  checkForError(name);

  if (FAILED(hres))
    THROW_MZQT_IDISPATCHEXCEPT_HRES(hres);

  if (varLabels.vt != VT_EMPTY && varLabels.parray != NULL) {

    SAFEARRAY *parray = varLabels.parray;
    int dim = parray->rgsabound[0].cElements;
    double *pdval = (double *) parray->pvData;

    for (int inx = 0; inx < dim; inx++) {
      double dMass = (double) pdval[((inx) * 6) + 0];
      double dInt = (double) pdval[((inx) * 6) + 1];

      masses.push_back(dMass);
      intensities.push_back(dInt);
    }

    SAFEARRAY * psa = varLabels.parray;
    varLabels.parray = NULL;
    // Delete the SafeArray
    SafeArrayDestroy(psa);
  }
}

void XRawfile::GetFilters(QStringList &filters)
{
  filters.clear();

  VARIANT varFilters;
  ::VariantInit(&varFilters);

  //get dispId
  const char *name = "GetFilters";
  DISPID dispid = dispIDofName(name, disp);
  assert(dispid != DISPID_UNKNOWN);

  //init the variants
  int cArgs = 2;
  VARIANTARG varg[cArgs];
  ::VariantInit(&varg[0]);
  ::VariantInit(&varg[1]);

  long nArraySize = 0;
  varg[0].vt = VT_BYREF | VT_I4;
  varg[0].plVal = &nArraySize;
  varg[1].vt = VT_BYREF | VT_VARIANT;
  varg[1].pvarVal = &varFilters;

  //set up the parameter
  DISPPARAMS params;
  params.cArgs = cArgs;
  params.rgdispidNamedArgs = 0;
  params.cNamedArgs = 0;
  params.rgvarg = varg;

  //init the exception
  EXCEPINFO excepinfo;
  memset(&excepinfo, 0, sizeof(excepinfo));
  UINT argerr = 0;

  //init the result
  VARIANTARG res;
  ::VariantInit(&res);

  HRESULT hres = disp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                              DISPATCH_METHOD | DISPATCH_PROPERTYGET, &params,
                              &res, &excepinfo, &argerr);

  checkForError(name);

  if (FAILED(hres))
    THROW_MZQT_IDISPATCHEXCEPT_HRES(hres);

  if (!nArraySize || varFilters.vt != (VT_ARRAY | VT_BSTR)) {
    return;
  }

  // Get a pointer to the SafeArray
  SAFEARRAY* psa = varFilters.parray;
  varFilters.parray = NULL;
  BSTR* pbstrFilters = NULL;
  if (FAILED(SafeArrayAccessData(psa, (void**) (&pbstrFilters)))) {
    SafeArrayUnaccessData(psa);
    SafeArrayDestroy(psa);
  }

  // display filters one at a time

  for (long i = 0; i < nArraySize; i++) {
    filters.push_back(QString::fromUtf16((const ushort *) pbstrFilters[i]));
  }
  // Delete the SafeArray
  SafeArrayUnaccessData(psa);
  SafeArrayDestroy(psa);

}

void XRawfile::GetMassListFromScanNum(int &scanNumber, const QString &szFilter,
    int intensityCutoffType, int intensityCutoffValue, int maxNumberOfPeaks,
    bool centroidResult, double &centroidPeakWidth, QList<double> &masses,
    QList<double> &intensities)
{
  getMassList("GetMassListFromScanNum", scanNumber, szFilter,
              intensityCutoffType, intensityCutoffValue, maxNumberOfPeaks,
              centroidResult, centroidPeakWidth, masses, intensities);
}

void XRawfile::GetPrevMassListFromScanNum(int &scanNumber,
    const QString &szFilter, int intensityCutoffType, int intensityCutoffValue,
    int maxNumberOfPeaks, bool centroidResult, double &centroidPeakWidth,
    QList<double> &masses, QList<double> &intensities)
{
  getMassList("GetPrevMassListFromScanNum", scanNumber, szFilter,
              intensityCutoffType, intensityCutoffValue, maxNumberOfPeaks,
              centroidResult, centroidPeakWidth, masses, intensities);
}

void XRawfile::getMassList(const char *methodName, int &scanNumber,
    const QString &szFilter, int intensityCutoffType, int intensityCutoffValue,
    int maxNumberOfPeaks, bool centroidResult, double &centroidPeakWidth,
    QList<double> &masses, QList<double> &intensities)
{
  masses.clear();
  intensities.clear();

  typedef struct _datapeak {
    double dMass;
    double dIntensity;
  } DataPeak;

  //get dispId
  DISPID dispid = dispIDofName(methodName, disp);
  assert(dispid != DISPID_UNKNOWN);

  //init the variants
  VARIANTARG varg[10];
  for (int i = 0; i < 10; ++i) {
    ::VariantInit(&varg[i]);
  }

  VARIANT varMassList;
  VariantInit(&varMassList);
  VARIANT varPeakFlags;
  VariantInit(&varPeakFlags);
  long nArraySize = 0;

  varg[0].vt = VT_BYREF | VT_I4;
  varg[0].plVal = &nArraySize;
  varg[1].vt = VT_BYREF | VT_VARIANT;
  varg[1].pvarVal = &varPeakFlags;
  varg[2].vt = VT_BYREF | VT_VARIANT;
  varg[2].pvarVal = &varMassList;
  varg[3].vt = VT_BYREF | VT_R8;
  varg[3].pdblVal = &centroidPeakWidth;
  varg[4].vt = VT_I4;
  varg[4].lVal = (long) centroidResult;
  varg[5].vt = VT_I4;
  varg[5].lVal = maxNumberOfPeaks;
  varg[6].vt = VT_I4;
  varg[6].lVal = intensityCutoffValue;
  varg[7].vt = VT_I4;
  varg[7].lVal = intensityCutoffType;
  varg[8].vt = VT_BSTR;
  varg[8].bstrVal = NULL;

  long sNumber = scanNumber;
  varg[9].vt = VT_BYREF | VT_I4;
  varg[9].plVal = &sNumber;

  //set up the parameter
  DISPPARAMS params;
  params.cArgs = 10;
  params.rgdispidNamedArgs = 0;
  params.cNamedArgs = 0;
  params.rgvarg = varg;

  //init the exception
  EXCEPINFO excepinfo;
  memset(&excepinfo, 0, sizeof(excepinfo));
  UINT argerr = 0;

  //init the result
  VARIANTARG res;
  ::VariantInit(&res);

  HRESULT hres = disp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                              DISPATCH_METHOD | DISPATCH_PROPERTYGET, &params,
                              &res, &excepinfo, &argerr);

  checkForError(methodName);

  if (FAILED(hres))
    THROW_MZQT_IDISPATCHEXCEPT_HRES(hres);

  QString errorMsg;
  GetErrorMessage(errorMsg);
  //std::cout << "error msg: " << errorMsg.toStdString() << std::endl;

  if (nArraySize) {

    int dim = varMassList.parray->rgsabound[0].cElements;

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

}

void XRawfile::GetAveragedMassSpectrum(const QList<int> &scanNumbers,
    bool centroidResult, QList<double> &masses, QList<double> &intensities)
{
  if (scanNumbers.size() == 0)
    throw XRawfileException("Unable to get average spectrum: no scan");

  masses.clear();
  intensities.clear();

  //get dispId
  const char *name = "GetAveragedMassSpectrum";
  DISPID dispid = dispIDofName(name, disp);
  assert(dispid != DISPID_UNKNOWN);

  //init the variants
  const int nArgs = 6;
  VARIANTARG varg[nArgs];
  for (int i = 0; i < nArgs; ++i) {
    ::VariantInit(&varg[i]);
  }

  VARIANT varMassList;
  VariantInit(&varMassList);
  VARIANT varPeakFlags;
  VariantInit(&varPeakFlags);
  long nArraySize = 0;

  varg[0].vt = VT_BYREF | VT_I4;
  varg[0].plVal = &nArraySize;
  varg[1].vt = VT_BYREF | VT_VARIANT;
  varg[1].pvarVal = &varPeakFlags;
  varg[2].vt = VT_BYREF | VT_VARIANT;
  varg[2].pvarVal = &varMassList;
  varg[3].vt = VT_BOOL;
  varg[3].boolVal = centroidResult ? TRUE : FALSE;
  varg[4].vt = VT_I4;
  varg[4].lVal = (long) scanNumbers.size();

  long *sNumbers = new long[scanNumbers.size()];
  for (int i = 0; i < scanNumbers.size(); ++i) {
    sNumbers[i] = scanNumbers.at(i);
  }
  varg[5].vt = VT_BYREF | VT_I4;
  varg[5].plVal = sNumbers;

  //set up the parameter
  DISPPARAMS params;
  params.cArgs = nArgs;
  params.rgdispidNamedArgs = 0;
  params.cNamedArgs = 0;
  params.rgvarg = varg;

  //init the exception
  EXCEPINFO excepinfo;
  memset(&excepinfo, 0, sizeof(excepinfo));
  UINT argerr = 0;

  //init the result
  VARIANTARG res;
  ::VariantInit(&res);

  HRESULT hres = disp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                              DISPATCH_METHOD | DISPATCH_PROPERTYGET, &params,
                              &res, &excepinfo, &argerr);

  checkForError(name);

  if (FAILED(hres))
    THROW_MZQT_IDISPATCHEXCEPT_HRES(hres);

  if (nArraySize) {

    int dim = varMassList.parray->rgsabound[0].cElements;

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

  delete[] sNumbers; //free the allocated scan numbers

}

void XRawfile::GetTrailerExtraValueForScanNum(int nScanNumber,
    const QString &szLabel, QVariant &value)
{
  value.clear();

  VARIANT varValue;
  ::VariantInit(&varValue);

  //get dispId
  const char *name = "GetTrailerExtraValueForScanNum";
  DISPID dispid = dispIDofName(name, disp);
  assert(dispid != DISPID_UNKNOWN);

  //init the variants
  int cArgs = 3;
  VARIANTARG varg[cArgs];
  ::VariantInit(&varg[0]);
  ::VariantInit(&varg[1]);
  ::VariantInit(&varg[2]);

  varg[0].vt = VT_BYREF | VT_VARIANT;
  varg[0].pvarVal = &varValue;
  varg[1].vt = VT_BSTR;
  varg[1].bstrVal = QStringToBSTR(szLabel);
  varg[2].vt = VT_I4;
  varg[2].lVal = nScanNumber;

  //set up the parameter
  DISPPARAMS params;
  params.cArgs = cArgs;
  params.rgdispidNamedArgs = 0;
  params.cNamedArgs = 0;
  params.rgvarg = varg;

  //init the exception
  EXCEPINFO excepinfo;
  memset(&excepinfo, 0, sizeof(excepinfo));
  UINT argerr = 0;

  //init the result
  VARIANTARG res;
  ::VariantInit(&res);

  HRESULT hres = disp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                              DISPATCH_METHOD | DISPATCH_PROPERTYGET, &params,
                              &res, &excepinfo, &argerr);

  checkForError(name);

  if (FAILED(hres))
    THROW_MZQT_IDISPATCHEXCEPT_HRES(hres);

  value = VARIANTToQVariant(varg[0], "");

  VariantClear(&varg[1]); //free the string
}
