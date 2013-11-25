/*
 DACScanStats.h
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

#ifndef MZQT_DACSCANSTATS_H_
#define MZQT_DACSCANSTATS_H_

#include <mzqt/common/IDispatch.h>

namespace mzqt {

    class DACScanStats: public IDispatch {

    public:

        DACScanStats();

        void getScanStats(const QString &inputFileName, int function,
                int process, int scan);

        int getPeaksInScan();
        float getRetnTime();
        float getLoMass();
        float getHiMass();
        float getTIC();
        float getBPM();
        float getBPI();
        int getAccurateMass();
        int getCalibrated();
        bool getContinuum();
        int getMolecularMass();
        int getOverload();

    };

}

#endif /* MZQT_DACSCANSTATS_H_ */
