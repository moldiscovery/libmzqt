/*
 DACProcessInfo.cpp
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

#include "DACProcessInfo.h"

#ifdef USE_MMGR_MEMORY_CHECK
#include <mmgr.h>
#endif

using namespace mzqt;

DACProcessInfo::DACProcessInfo() :
  IDispatch("DACProcessInfo.DACProcessInfo")
{
}

void DACProcessInfo::getProcessInfo(const QString &inputFileName, int n)
{
  idispatch_->dynamicCall("GetProcessInfo(const QString &, int)",
                          inputFileName, n);
}

int DACProcessInfo::getNumProcesses()
{
  //should we look if the variant is empty first before converting
  //or we check the message using the message handler?
  QVariant numProcesses = idispatch_->property("NumProcesses");

  return numProcesses.toInt();
}
