/*
 DACFunctionInfo.cpp
 Created on: 30/07/2010
 Author: Fabien Fontaine
 Copyright (C) 2010 Lead Molecular Design Sl
 
 Access MassLynx raw files through the Datafile Access Component (DAC). For
 more details about how to get and use the DAC library go to:
 www.waters.com/webassets/cms/support/docs/71500123505ra.pdf

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

#include <QAxObject>

#include "DACFunctionInfo.h"

#ifdef USE_MMGR_MEMORY_CHECK
#include <mmgr.h>
#endif

using namespace mzqt;

DACFunctionInfo::DACFunctionInfo() :
  IDispatch("DACFunctionInfo.DACFunctionInfo")
{
}

void DACFunctionInfo::getFunctionInfo(const QString &inputFileName, int n)
{
  idispatch_->dynamicCall("GetFunctionInfo(const QString &, int)",
                          inputFileName, n);
}

QString DACFunctionInfo::getFunctionType()
{
  //should we look if the variant is empty first before converting
  //or we check the message using the message handler?
  QVariant fType = idispatch_->property("FunctionType");

  return fType.toString();
}

int DACFunctionInfo::getNumScans()
{
  //should we look if the variant is empty first before converting
  //or we check the message using the message handler?
  QVariant numScans = idispatch_->property("NumScans");

  return numScans.toInt();
}

