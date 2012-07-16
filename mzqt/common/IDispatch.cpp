/*
 IDispatch.cpp
 Created on: 29/07/2010
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

#include <QString>
#include <QAxObject>
#include <winerror.h>
#include <mzqt/common/IDispatch.h>

#ifdef USE_MMGR_MEMORY_CHECK
#include <mmgr.h>
#endif

using namespace mzqt;

IDispatchException::IDispatchException(HRESULT hres, const char *file, int line) :
  Exception("", file, line)
{
  std::string s;
  unsigned long code = HRESULT_CODE(hres);
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

  msg_ = "IDISPATCH ERROR(code:" + s + msg_ + ")";
}

IDispatchException::IDispatchException(const std::string &msg,
    const char *file, int line) :
  Exception(msg, file, line)
{
  msg_ = "IDISPATCH ERROR(" + msg_ + ")";
}

IDispatchException::~IDispatchException() throw ()
{
}

mzqt::IDispatch::IDispatch() :
  idispatch_(new QAxObject())
{
}

mzqt::IDispatch::IDispatch(const QString &componentName) :
  idispatch_(new QAxObject())
{
  //perhaps if we install a message handle on the warning it would be
  //better. If so we may use the default constructor
  //and throw something from the message handler
  if (setComponent(componentName) == false) {
    throw IDispatchException("Unable to set component: "
        + componentName.toStdString());
  }
}

mzqt::IDispatch::~IDispatch()
{
}
bool mzqt::IDispatch::setComponent(const QString &componentName)
{
  return idispatch_->setControl(componentName);
}
