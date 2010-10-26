/*
 DACScanStats.cpp
 Created on: 30/07/2010
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

#include "DACScanStats.h"

#ifdef USE_MMGR_MEMORY_CHECK
#include <mmgr.h>
#endif

using namespace mzqt;

DACScanStats::DACScanStats() :
  IDispatch("DACScanStats.DACScanStats")
{
}

void DACScanStats::getScanStats(const QString &inputFileName, int function,
    int process, int scan)
{
  idispatch_->dynamicCall("GetScanStats(const QString &, int, int, int)",
                          inputFileName, function, process, scan);
}

int DACScanStats::getPeaksInScan()
{
  QVariant peaksInScan = idispatch_->property("PeaksInScan");

  return peaksInScan.toInt();
}

float DACScanStats::getRetnTime()
{
  QVariant rt = idispatch_->property("RetnTime");

  return rt.toDouble();
}

float DACScanStats::getLoMass()
{
  QVariant prop = idispatch_->property("LoMass");

  return prop.toDouble();
}

float DACScanStats::getHiMass()
{
  QVariant prop = idispatch_->property("HiMass");

  return prop.toDouble();
}

float DACScanStats::getTIC()
{
  QVariant prop = idispatch_->property("TIC");

  return prop.toDouble();
}

float DACScanStats::getBPM()
{
  QVariant prop = idispatch_->property("BPM");

  return prop.toDouble();
}

float DACScanStats::getBPI()
{
  QVariant prop = idispatch_->property("BPI");

  return prop.toDouble();
}

