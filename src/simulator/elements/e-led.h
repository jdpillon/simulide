/***************************************************************************
 *   Copyright (C) 2012 by santiago González                               *
 *   santigoro@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>.  *
 *                                                                         *
 ***************************************************************************/

#ifndef ELED_H
#define ELED_H

#include "e-diode.h"
#include "pin.h"

class eLed : public eDiode
{
    public:
        eLed();
        ~eLed();

        float maxCurrent() const             { return m_maxCurrent; }
        void  setMaxCurrent( float current ) { m_maxCurrent = current; }

        void runStep();

    protected:
        void updateBright();
        virtual void updateVI();

        unsigned long long m_prevStep;

        uint  m_bright;

        float m_maxCurrent;
        float m_lastCurrent;
        float lastUpdatePeriod;
        float avg_brightness;
        float disp_brightness;
};

#endif

