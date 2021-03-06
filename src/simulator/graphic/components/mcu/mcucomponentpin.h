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

#ifndef MCUCOMPONENTPIN_H
#define MCUCOMPONENTPIN_H

#include "component.h"
//#include "e-element.h"
#include "e-source.h"
#include "pin.h"
#include "mcucomponent.h"

class McuComponentPin : public QObject, public eSource
{
    Q_OBJECT
    public:
        McuComponentPin( McuComponent *mcu, QString id, QString type, QString label, int pos, int xpos, int ypos, int angle );
        ~McuComponentPin();

        Pin* pin() const { return ( static_cast<Pin*>(m_ePin[0]) ); }

        void initialize();
        void terminate();

        void move( int dx, int dy );
        void moveLabel( int dx, int dy );

        //virtual void changed(){;}
        void resetOutput();

    protected:
        //float m_inputImp;
        //float m_outImp;
        //float m_imp;

        McuComponent  *m_mcuComponent;
        
        bool m_attached;
        //eSource *m_out;
        //Pin     *m_pin;

        double m_Rth;
        double m_Vth;

        QString m_type;
        QString m_id;

        QGraphicsSimpleTextItem* m_pinLabel;
};

#endif


