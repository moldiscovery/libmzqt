/*
 InstrumentInterface.cpp
 Created on: 22/10/2010
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

#include <mzqt/common/InstrumentInterface.h>

using namespace mzqt;

InstrumentInterface::InstrumentInterface(void)
{
  totalNumScans_ = -1;
  curScanNum_ = -1;
  firstScanNumber_ = -1;
  lastScanNumber_ = -1;
  curScanNum_ = -1;

  startTimeInSec_ = -1;
  endTimeInSec_ = -1;

  doCompression_ = false;
  doCentroid_ = false;
  doDeisotope_ = false;
  shotgunFragmentation_ = false;
  lockspray_ = false;

  verbose_ = false;
  accurateMasses_ = 0;
  inaccurateMasses_ = 0;
  maxUVMSRatio_ = 0;

  totalNumUVScans_ = -1;
  curUVScanNum_ = -1;
  firstUVScanNumber_ = -1;
  lastUVScanNumber_ = -1;
}

InstrumentInterface::~InstrumentInterface(void)
{
}
