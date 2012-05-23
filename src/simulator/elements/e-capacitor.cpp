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

// capacitor companion model using trapezoidal approximation
// (Norton equivalent) consists of a current source in
// parallel with a resistor.

#include <cmath>

#include "e-capacitor.h"

eCapacitor::eCapacitor() : eResistor()
{
    m_cap = 1; // uF
    m_tStep = 1/(double)Simulator::self()->simuClock();
    m_resist = m_tStep/(2*m_cap*1e-6);// capacitance property(m_cap) is in uF, covert to F
    m_curSource = 0;

    Simulator::self()->addToReacList( this );
}
eCapacitor::~eCapacitor(){ Simulator::self()->remFromReacList( this ); }

void eCapacitor::initialize()
{
    if( m_ePin[0]->isConnected() ) m_ePin[0]->getEnode()->addToChangedList(this);
    if( m_ePin[1]->isConnected() ) m_ePin[1]->getEnode()->addToChangedList(this);
    eResistor::initialize();
}

void eCapacitor::runStep()
{
    if( !m_vChanged ) return;
    m_vChanged = false;

    double volt0 = m_ePin[0]->getVolt();
    double volt1 = m_ePin[1]->getVolt();

    double volt = volt0 - volt1;

    m_curSource = -2*volt/m_resist-m_curSource;

    //m_volt[0] = volt0;
    //m_volt[1] = volt1;
    //qDebug() << "eCapacitor::setVChanged voltdiff " <<volt<< " m_curSource "<<m_curSource;

    m_ePin[0]->stampCurrent( -m_curSource );
    m_ePin[1]->stampCurrent(  m_curSource );
}

double eCapacitor::uF()             { return m_cap; }
void  eCapacitor::setuF( double c ) { m_cap = c; eResistor::setRes( m_tStep/(m_cap/1e6) ); }

