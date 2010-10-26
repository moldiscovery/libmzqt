/*
 DACExScanStats.cpp
 Created on: 02/08/2010
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

#include "DACExScanStats.h"

#ifdef USE_MMGR_MEMORY_CHECK
#include <mmgr.h>
#endif

using namespace mzqt;

DACExScanStats::DACExScanStats() :
  IDispatch("DACExScanStats.DACExScanStats")
{
}

void DACExScanStats::getExScanStats(const QString &inputFileName, int function,
    int process, int scan)
{
  idispatch_->dynamicCall("GetExScanStats(const QString &, int, int, int)",
                          inputFileName, function, process, scan);
}

float DACExScanStats::getCollisionEnergy()
{
  QVariant prop = idispatch_->property("CollisionEnergy");

  return prop.toDouble();
}

float DACExScanStats::getSetMass()
{
  QVariant prop = idispatch_->property("SetMass");

  return prop.toDouble();
}

