/*
 IDispatch.h
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

#ifndef MZQT_DISPATCH_H_
#define MZQT_DISPATCH_H_

#include <memory>
#include <windows.h>
#include <mzqt/common/Exception.h>

class QAxObject;
class QString;

#define THROW_MZQT_IDISPATCHEXCEPT_HRES(hres) (throw mzqt::IDispatchException(hres, __FILE__, __LINE__))
#define THROW_MZQT_IDISPATCHEXCEPT_MSG(msg) (throw mzqt::IDispatchException(msg, __FILE__, __LINE__))

namespace mzqt {

  class IDispatchException: public Exception {

  public:
    explicit IDispatchException(HRESULT hres, const char *file = 0, int line =
        0);
    explicit IDispatchException(const std::string &msg = "", const char *file =
        0, int line = 0);
    virtual ~IDispatchException() throw ();
  };

  class IDispatch {

  public:
    IDispatch();
    IDispatch(const QString &componentName);
    ~IDispatch();
    bool setComponent(const QString &componentName);

  protected:

    std::auto_ptr<QAxObject> idispatch_;
  };

}
#endif /* MZQT_DISPATCH_H_ */
