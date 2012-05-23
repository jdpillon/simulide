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

#include "pin.h"
#include "connector.h"


Pin::Pin( int dir, const QPoint &pos, QString id, int index, Component* parent )
        : QObject(), QGraphicsItem( parent ), ePin( index )
{
    m_component = parent;
    my_connector = 0l;
    m_id   = id;
    m_area = QRect(-4, -4, 12, 8);

    m_length = 8;

    setObjectName( id );
    setConnector( 0l );

    setPos( pos );
    rotate( 180-dir );

    const QFont sansFont( "Helvetica [Cronyx]", 5 );
    m_label = Circuit::self()->addSimpleText( id.toLatin1().data(), sansFont );
    m_label->setParentItem( this );
    m_label->setPos(-4, 4 );
    m_label->rotate( 180-dir );
    m_label->setText("");//( QString("%1 v").arg(m_volt) );

    setCursor(Qt::CrossCursor);
    this->setFlag( QGraphicsItem::ItemStacksBehindParent, true );
    this->setFlag( QGraphicsItem::ItemIsSelectable, true );

    connect( parent, SIGNAL( moved() ), this, SLOT( isMoved() ) );
}
Pin::~Pin(){}

void Pin::reset()
{
    //qDebug() << "Pin::reset "<< m_id << m_numConections;
    setConnector( 0l );
    ePin::reset();
    //qDebug() << "ePin::reset new:" << m_numConections;
    //m_msg = msg_dis;
    m_component->inStateChanged( m_index ); // Only used by node??
}

double Pin::getVolt()
{
    return ePin::getVolt();
}

//void Pin::setChanged( bool changed ) { m_changed = changed; }

void  Pin::setConnector( Connector* connector )
{
    my_connector = connector;
    
    if( my_connector ) 
    {
        //ePin::setEnode( connector->enode() );
        setCursor( Qt::ArrowCursor );
    }
    else               
    {
        //ePin::setEnode( 0l );
        setCursor( Qt::CrossCursor );
    }
}

/*void Pin::setConnected( bool connected )
{
    ePin::m_connected = connected;
    if( my_connector ) setCursor( Qt::ArrowCursor );
    else             { setCursor( Qt::CrossCursor );
                       my_connector = 0l; }
}*/

Connector* Pin::connector() { return my_connector; }

void Pin::isMoved()
{
    if( my_connector ) my_connector->updateConRoute( this, scenePos() );
}

void Pin::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if( event->button() == Qt::LeftButton )
    {
        if( my_connector==0l )
        {
            event->accept();
            if( Circuit::self()->is_constarted() ) Circuit::self()->closeconnector( this );
            else                                   Circuit::self()->newconnector( this );
        }
        else event->ignore();
    }
}

QString Pin::itemID() { return  m_id; }

void Pin::setLength( int length ) { m_length = length; }

void Pin::setConPin( Pin* pin ){ m_conPin = pin; }
Pin* Pin::conPin()             { return m_conPin; }

//bool Pin::changed(){ return m_changed; }

void Pin::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    Q_UNUSED(option); Q_UNUSED(widget);

    QPen pen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    //painter->setBrush( Qt::red );
    //painter->drawRect( boundingRect() );
    
    if( isSelected() ) pen.setColor( Qt::darkGray);

    painter->setPen(pen);

    if( m_length < 1 ) m_length = 1;
    painter->drawLine( 0, 0, m_length-1, 0);
}
#include "moc_pin.cpp"
