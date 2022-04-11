/*
 cominterface.cpp
 Created on: 24/11/2010
 Author: Fabien Fontaine
 */

#include <cassert>

#include <mzqt/common/cominterface.h>

using namespace mzqt;

DispatchException::DispatchException(HRESULT hres, const std::string &msg,
    const char *file, int line) :
  Exception(msg, file, line)
{
  std::string s;
  long code = HRESULT_CODE(hres);
  switch (code) {

  case S_OK:
    s = "Success";
    break;

  case DISP_E_BADVARTYPE:
    s = "DISP_E_BADVARTYPE";
    break;

  case DISP_E_EXCEPTION:
    s = "DISP_E_EXCEPTION";
    break;

  case DISP_E_MEMBERNOTFOUND:
    s = "DISP_E_MEMBERNOTFOUND";
    break;

  case DISP_E_NONAMEDARGS:
    s = "DISP_E_NONAMEDARGS";
    break;

  case DISP_E_OVERFLOW:
    s = "DISP_E_OVERFLOW";
    break;

  case DISP_E_PARAMNOTFOUND:
    s = "DISP_E_PARAMNOTFOUND";
    break;

  case DISP_E_TYPEMISMATCH:
    s = "DISP_E_TYPEMISMATCH";
    break;

  case DISP_E_UNKNOWNNAME:
    s = "DISP_E_UNKNOWNNAME";
    break;

  case DISP_E_UNKNOWNINTERFACE:
    s = "DISP_E_UNKNOWNINTERFACE";
    break;

  case DISP_E_BADINDEX:
    s = "DISP_E_BADINDEX";
    break;

  case DISP_E_UNKNOWNLCID:
    s = "DISP_E_UNKNOWNLCID";
    break;

  case DISP_E_ARRAYISLOCKED:
    s = "DISP_E_ARRAYISLOCKED";
    break;

  case DISP_E_BADPARAMCOUNT:
    s = "DISP_E_BADPARAMCOUNT";
    break;

  case DISP_E_PARAMNOTOPTIONAL:
    s = "DISP_E_PARAMNOTOPTIONAL";
    break;

  case DISP_E_BADCALLEE:
    s = "DISP_E_BADCALLEE";
    break;

  case DISP_E_NOTACOLLECTION:
    s = "DISP_E_NOTACOLLECTION";
    break;

  case DISP_E_DIVBYZERO:
    s = "DISP_E_DIVBYZERO";
    break;

  default:
    s = QString("Unknown HResult code value: 0x%1"
      "").arg(code, 0, 16).toStdString();
  }

  msg_ = "IDISPATCH ERROR(code:" + s + " " + msg_ + ")";
}

DispatchException::DispatchException(const std::string &msg,
    const char *file, int line) :
  Exception(msg, file, line)
{
  msg_ = "IDISPATCH ERROR(" + msg_ + ")";
}

DispatchException::~DispatchException() throw ()
{
}
DISPID COMInterface::dispIDofName(const QByteArray &name, IDispatch *disp)
{
  DISPID dispid = dispIDs_.value(name, DISPID_UNKNOWN);
  if (dispid == DISPID_UNKNOWN) {
    // get the Dispatch ID from the object
    QString unicodeName = QLatin1String(name);
    OLECHAR *names = (TCHAR*) unicodeName.utf16();
    HRESULT hres = disp->GetIDsOfNames(IID_NULL, &names, 1,
                                       LOCALE_USER_DEFAULT, &dispid);

    if (dispid == DISPID_UNKNOWN) {
      throw DispatchException("Unknown DispId for name: "
          + std::string(name.data()));
    }

    if (FAILED(hres))
      THROW_MZQT_IDISPATCHEXCEPT_HRES(hres); //more general error

    if (dispid != DISPID_UNKNOWN)
      dispIDs_.insert(name, dispid);
    else {

    }
  }
  return dispid;
}

void COMInterface::getNum(const char *dispName, int &e)
{
  e = 0;

  //Qt doesn't work for Named enum type so we get an int instead
  DISPID dispid = dispIDofName(dispName, disp_);
  assert(dispid != DISPID_UNKNOWN);

  DISPPARAMS params;

  params.cArgs = 0;
  params.rgdispidNamedArgs = 0;
  params.cNamedArgs = 0;
  params.rgvarg = 0;

  EXCEPINFO excepinfo;
  memset(&excepinfo, 0, sizeof(excepinfo));
  UINT argerr = 0;

  VARIANTARG res;
  ::VariantInit(&res);

  HRESULT hres = disp_->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
                               DISPATCH_METHOD | DISPATCH_PROPERTYGET, &params,
                               &res, &excepinfo, &argerr);

  if (FAILED(hres))
    throw DispatchException(hres, dispName);

  e = res.lVal;
}

