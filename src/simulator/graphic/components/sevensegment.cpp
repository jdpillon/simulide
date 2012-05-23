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

#include "sevensegment.h"
#include "itemlibrary.h"
#include "pin.h"

Component* SevenSegment::construct( QObject* parent, QString type, QString id )
{
    return new SevenSegment( parent, type, id );
}

LibraryItem* SevenSegment::libraryItem()
{
    return new LibraryItem(
        tr( "7 Segment" ),
        tr( "Outputs" ),
        "seven_segment.png",
        "Seven Segment",
        SevenSegment::construct );
}

SevenSegment::SevenSegment( QObject* parent, QString type, QString id )
    : Component( parent, type, id )
{
    setId( id );
    label->setPos( 30, -70);

    m_color = QColor(0,0,0);
    m_commonCathode = true;
    m_numDisplays = 0;
    m_area = QRect( -16, -24-1, 32, 48+2 );

    // Create a virtual ground and connect to all pins: avoid unconnected pins
    m_virtGnd = new eNode( "scr" );
    m_virtGnd->setNodeNumber(0);
    Simulator::self()->remFromEnodeList( m_virtGnd, /*delete=*/ false );

    m_ePin.resize(8);

    // Create Pins
    QString nodid;
    QString pinid;

    // Create Pins & eNodes for 7 segments
    for( int i=0; i<7; i++ )
    {
        pinid = QString( 97+i ); // a..g

        /*nodid = m_id;
        nodid.append(QString("-enod_")).append( pinid );
        m_enode[i] = new eNode(nodid);*/

        nodid = m_id;
        nodid.append(QString("-pin_")).append( pinid );
        m_ePin[i] = new Pin( 180, QPoint( -16-8, -24+i*8 ), nodid, 0, this );
        //m_ePin[i]->setEnode( m_enode[i] );
        //m_ePin[i]->setEnodeComp( m_virtGnd );
    }

    // eNode dot
   /* nodid = m_id;
    nodid.append(QString("-enod_dot"));
    m_enode[7] = new eNode(nodid);*/

    // Pin dot
    nodid = m_id;
    nodid.append(QString("-pin_dot"));
    m_ePin[7] = new Pin( 270, QPoint( -8, 24+8 ), nodid, 0, this );
    //m_ePin[7]->setEnode( m_enode[7] );
    //m_ePin[7]->setEnodeComp( m_virtGnd );

    setNumDisplays(1);
}
SevenSegment::~SevenSegment() { }

int SevenSegment::numDisplays()
{
    return m_numDisplays;
}

void SevenSegment::setNumDisplays( int displays )
{
    if( displays == m_numDisplays ) return;

    m_area = QRect( -16, -24-1, displays*32, 48+2 );
    m_commonPin.resize( displays );
    m_cathodePin.resize( displays*8 );
    m_anodePin.resize( displays*8 );

    if( displays > m_numDisplays )
    {
        for( int i=m_numDisplays; i<displays; i++ ) createDisplay( i );
    }
    m_numDisplays = displays;
    update();
}

bool SevenSegment::isCommonCathode()
{
    return m_commonCathode;
}

void SevenSegment::setCommonCathode( bool isCommonCathode )
{
    bool pauseSim = Simulator::self()->isRunning();
    if( pauseSim ) Simulator::self()->pauseSim();

    m_commonCathode = isCommonCathode;

    if( pauseSim ) Simulator::self()->runContinuous();
}

void SevenSegment::initialize()
{
    for( int i=0; i<8; i++ )
    {
        m_ePin[i]->setEnodeComp( m_virtGnd );
        m_enode[i] = m_ePin[i]->getEnode();     // Get the actual eNode of pin
    }
    for( int i=0; i<m_numDisplays; i++ )
    {
        m_commonPin[i]->setEnodeComp( m_virtGnd );
        eNode* commonEnode = m_commonPin[i]->getEnode();     // Get the actual eNode of pin

        int pin;
        if( m_commonCathode )
        {
            for( int j=0; j<8; j++ )
            {
                pin = i*8+j;
                m_cathodePin[pin]->setEnode( commonEnode );
                m_anodePin[pin]->setEnode( m_enode[j] );
            }
        }
        else
        {
            for( int j=0; j<8; j++ )
            {
                pin = i*8+j;
                m_anodePin[pin]->setEnode( commonEnode );
                m_cathodePin[pin]->setEnode( m_enode[j] );
            }
        }
    }
}

void SevenSegment::stamp()
{
    for( int i=0; i<8; i++ )  m_ePin[i]->stampAdmitance( 1e-6 );
    for( int i=0; i<m_numDisplays; i++ ) m_commonPin[i]->stampAdmitance( 1e-6 );
}

void SevenSegment::createDisplay( int dispNumber )
{
    int x = 32*dispNumber;
    QString nodid;
    QString pinid;
    LedSmd* segment[8];

    // eNode common
    /*nodid = m_id;
    nodid.append(QString("-enod_common-")).append( QString( 97+dispNumber ) );
    eNode* commonEnode = new eNode(nodid);*/

    // Pin common
    nodid = m_id;
    nodid.append(QString("-pin_common")).append( QString( 97+dispNumber ) );
    m_commonPin[dispNumber] = new Pin( 270, QPoint( x+8, 24+8 ), nodid, 0, this );
    //m_commonPin[dispNumber]->setEnode( commonEnode );
    //m_commonPin[dispNumber]->setEnodeComp( m_virtGnd );

    // Create segments
    for( int i=0; i<8; i++ )
    {
        nodid = m_id;
        pinid = QString( 97+i );
        nodid.append(QString("-led_")).append( pinid );
        LedSmd* lsmd;
        if( i<7 ) lsmd = new LedSmd( this, "LEDSMD", nodid, QRectF(0, 0, 13.5, 1.5) );
        else      lsmd = new LedSmd( this, "LEDSMD", nodid, QRectF(0, 0, 1.5, 1.5) );
        lsmd->setParentItem(this);
        lsmd->setEnabled(false);
        lsmd->setMaxCurrent( 0.015 );

        m_anodePin[dispNumber*8+i]= lsmd->getEpin(0);
        m_cathodePin[dispNumber*8+i] = lsmd->getEpin(1);


        //ePin* epin0 = lsmd->getEpin(0);
        //ePin* epin1 = lsmd->getEpin(1);

        //epin0->setEnode( m_enode[i] ); // Connect led anode to pin
        //epin0->setEnodeComp( commonEnode );

        //epin1->setEnode( commonEnode ); // Connect led cathode to common
        //epin1->setEnodeComp( m_enode[i] );

        segment[i] = lsmd;
    }

    segment[0]->setPos( x-5, -20 );
    segment[1]->setPos( x+11.5, -16 );
    segment[1]->setRotation(96);
    segment[2]->setPos( x+10, 3 );
    segment[2]->setRotation(96);
    segment[3]->setPos( x-8, 19 );
    segment[4]->setPos( x-9, 3 );
    segment[4]->setRotation(96);
    segment[5]->setPos( x-7.5, -16 );
    segment[5]->setRotation(96);
    segment[6]->setPos( x-6.5, 0 );
    segment[7]->setPos( x+12, 19 );
}

void SevenSegment::paint( QPainter* p, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Q_UNUSED(option); Q_UNUSED(widget);

    Component::paint( p, option, widget );

    p->drawRect( boundingRect() );
}

#include "moc_sevensegment.cpp"

