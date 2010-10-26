/*
 DACHeader.cpp
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

#include <QAxObject>

#include "DACHeader.h"

#ifdef USE_MMGR_MEMORY_CHECK
#include <mmgr.h>
#endif

using namespace mzqt;

DACHeader::DACHeader() :
    IDispatch("DACHeader.DACHeader")
{
}

void DACHeader::getHeader(const QString &inputFileName)
{
    idispatch_->dynamicCall("GetHeader(const QString &)", inputFileName);
}

QString DACHeader::getAcquDate()
{
    QVariant prop = idispatch_->property("AcquDate");

    return prop.toString();
}

QString DACHeader::getAcquTime()
{
    QVariant prop = idispatch_->property("AcquTime");

    return prop.toString();
}

QString DACHeader::getInstrument()
{
    QVariant prop = idispatch_->property("Instrument");
    return prop.toString();
}

