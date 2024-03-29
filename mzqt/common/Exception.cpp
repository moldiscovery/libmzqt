/*
	exception.cpp
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

#include <QString>
#include "Exception.h"

using namespace mzqt;

///////////////////////////////////////////////////////////////////////////////
Exception::Exception(const std::string &msg, const char *file, int line)
{
  msg_ = msg;
  if(file) {
    msg_ += " file: ";
    msg_ += file;
  }
  if(line) {
    msg_ += " line: ";
    msg_ += QString::number(line).toStdString();
  }
}

Exception::~Exception() throw ()
{
}

const char *Exception::what() const throw ()
{
  return msg_.c_str();
}

///////////////////////////////////////////////////////////////////////////////
Abort::Abort(const std::string &msg) :
  Exception(msg)
{
  msg_ = "ABORT(" + msg + ")";
}

Abort::~Abort() throw ()
{
}
