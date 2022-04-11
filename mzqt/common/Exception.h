/*
 exception.h
 Created on: 18/10/2010
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

#ifndef MZQT_EXCEPTION_H_
#define MZQT_EXCEPTION_H_

#include <string>

#include <exception>

#if defined(__GNUC__) || defined(MZQT_STATIC)
#ifndef MZQTDLL_API
#define MZQTDLL_API
#endif
#else
#ifdef MZQTDLL_EXPORTS
#ifndef MZQTDLL_API
#define MZQTDLL_API __declspec(dllexport)
#endif
#else
#ifndef MZQTDLL_API
#define MZQTDLL_API __declspec(dllimport)
#endif
#endif
#endif

namespace mzqt {

  /*! Base exception class
   */
  class Exception: public std::exception {

  public:

    MZQTDLL_API explicit Exception(const std::string &msg = "", const char *file = 0,
        int line = 0);
    MZQTDLL_API virtual ~Exception() throw ();

    MZQTDLL_API virtual const char *what() const throw ();

  protected:
    std::string msg_;
  };

  /*! Exception thrown when the software requested to abort
   */
  class Abort: public Exception {

  public:

    MZQTDLL_API explicit Abort(const std::string &msg = "");
    MZQTDLL_API virtual ~Abort() throw ();

  };

}
#endif /* MZQT_EXCEPTION_H_ */
