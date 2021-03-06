//   transportbutton.cpp
//
//   An audio transport button widget.
//
//   (C) Copyright 2002,2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//

#include <QBitmap>
#include <QPainter>
#include <QTimer>

#include "transportbutton.h"

TransportButton::TransportButton(TransportButton::TransType type,
				   QWidget *parent=0,
				   const char *name=0) :
  QPushButton(parent,name)
{
  button_type=type;
  button_state=TransportButton::Off;
  on_color=QColor(TRANSPORTBUTTON_DEFAULT_ON_COLOR);
  accent_color=QColor(colorGroup().shadow());
  on_cap=new QPixmap();
  drawOnCap();
  off_cap=new QPixmap();
  drawOffCap();
  setPixmap(*off_cap);

  flash_timer=new QTimer(this,"flash_timer");
  connect(flash_timer,SIGNAL(timeout()),this,SLOT(flashClock()));
  flash_state=false;
}


TransportButton::TransType TransportButton::getType() const
{
  return button_type;
}


void TransportButton::setType(TransportButton::TransType type)
{
  button_type=type;
}


QColor TransportButton::onColor() const
{
  return on_color;
}


void TransportButton::setOnColor(QColor color)
{
  if(color!=on_color) {
    on_color=color;
    drawOnCap();
    drawOffCap();
    updateCaps();
  }
}

QColor TransportButton::accentColor() const
{
  return accent_color;
}


void TransportButton::setAccentColor(QColor color)
{
  if(color!=accent_color) {
    accent_color=color;
    drawOnCap();
    drawOffCap();
    updateCaps();
  }
}

void TransportButton::setState(TransportButton::TransState state)
{
  QKeySequence a=accel();
  button_state=state;
  switch(button_state) {
      case TransportButton::On:
	flashOff();
	if(isEnabled()) {
	  setPixmap(*on_cap);
	}
	break;
      case TransportButton::Off:
	flashOff();
	if(isEnabled()) {
	  setPixmap(*off_cap);
	}
	break;
      case TransportButton::Flashing:
	if(isEnabled()) {
	  flashOn();
	}
	break;
  }
  setAccel(a);
}


void TransportButton::on()
{
  setState(TransportButton::On);
}


void TransportButton::off()
{
  setState(TransportButton::Off);
}


void TransportButton::flash()
{
  setState(TransportButton::Flashing);
}


void TransportButton::resizeEvent(QResizeEvent *event)
{
  QKeySequence a=accel();
  drawOnCap();
  drawOffCap();
  switch(button_state) {
      case TransportButton::Off:
	setPixmap(*off_cap);
	break;
      case TransportButton::On:
	setPixmap(*on_cap);
	break;
      default:
	setPixmap(*off_cap);
	break;
  }
  setAccel(a);
}


void TransportButton::enabledChange(bool oldEnabled)
{
  QKeySequence a=accel();
  if(isEnabled()&&!oldEnabled) {
    setState(button_state);
    update();
  }
  if(!isEnabled()&&oldEnabled) {
    update();
    setAccel(a);
  }
}
 

void TransportButton::flashClock()
{
  QKeySequence a=accel();
  if(flash_state) {
    flash_state=false;
    setPixmap(*off_cap);
  }
  else {
    flash_state=true;
    setPixmap(*on_cap);
  }
  setAccel(a);
}


void TransportButton::updateCaps()
{
  switch(button_state) {
      case TransportButton::On:
	setPixmap(*on_cap);
	break;

      case TransportButton::Flashing:
	if(flash_state) {
	  setPixmap(*on_cap);
	}
	else {
	  setPixmap(*off_cap);
	}
	break;

      case TransportButton::Off:
	setPixmap(*off_cap);
	break;
  }
}


void TransportButton::drawMask(QPixmap *cap)
{
  QPolygon triangle=QPolygon(3);
  QPainter b;
  QBitmap *bitmap=new QBitmap(size());
  int edge;

  if(height()<width()) {
    edge=height();
  }
  else {
    edge=width();
  }
  cap->resize(size());
  b.begin(bitmap);
  b.fillRect(0,0,size().width(),size().height(),QColor(Qt::color0));
  b.setPen(QColor(Qt::color1));
  b.setBrush(QColor(Qt::color1));

  switch(button_type) {
      case TransportButton::Play:
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case TransportButton::Stop:
	b.fillRect(width()/2-edge*3/10,height()/2-edge*3/10,
		   edge*3/5,edge*3/5,QColor(Qt::color1));
	break;
      case TransportButton::Record:
	b.drawEllipse(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		      (3*edge)/5,(3*edge)/5);
	break;
      case TransportButton::FastForward:
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case TransportButton::Rewind:
	triangle.setPoint(0,width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2,height()/2);
	triangle.setPoint(2,width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2-(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case TransportButton::Eject:
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2);
	b.drawPolygon(triangle);
	b.fillRect(width()/2-(3*edge)/10,height()/2+edge/10,
		   (3*edge)/5,edge/5,QColor(Qt::color1));
	break;
      case TransportButton::Pause:
	b.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   (3*edge)/15,(3*edge)/5,QColor(Qt::color1));
	b.fillRect(width()/2+(3*edge)/30,height()/2-(3*edge)/10,
		   (3*edge)/15,(3*edge)/5,QColor(Qt::color1));
	break;
      case TransportButton::PlayFrom:
	b.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(Qt::color1));
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case TransportButton::PlayBetween:
	b.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(Qt::color1));
	b.fillRect(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(Qt::color1));
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(2*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case TransportButton::Loop:
//	b.moveTo(width()/2-(2*edge)/10+1,height()/2+(edge)/4);
//	b.moveTo(width()/2+(edge)/10+1,height()/2-edge/10);
//	b.moveTo(width()/2-(2*edge)/10+1,height()/2+(edge)/4);
	b.drawArc(width()/6,height()/2-edge/9,2*width()/3,
		  height()/3+edge/10,1440,5760);
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(edge)/4);
	triangle.setPoint(1,width()/2+(edge)/10+1,height()/2-edge/10);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+edge/20);
	b.drawPolygon(triangle);
	break;
      case TransportButton::Up:
	triangle.setPoint(0,width()/2,(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()-(3*edge)/10);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()-(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case TransportButton::Down:
	triangle.setPoint(0,width()/2,height()-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,(3*edge)/10);
	triangle.setPoint(2,width()/2-(3*edge)/10,(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case TransportButton::PlayTo:
	b.fillRect(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(Qt::color1));
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(2*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      default:
	b.fillRect(0,0,width(),height(),QColor(Qt::color1));
  }
  b.end();
  cap->setMask(*bitmap);

  delete bitmap;
}


void TransportButton::drawOnCap()
{
  QPainter p;
  QPolygon triangle=QPolygon(3);
  int edge;

  if(height()<width()) {
    edge=height();
  }
  else {
    edge=width();
  }
  drawMask(on_cap);
  p.begin(on_cap);
  p.setPen(on_color);
  p.setBrush(on_color);
  switch(button_type) {
      case TransportButton::Play:
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(3*edge)/10,height()/2+(3*edge)/10,
		  width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   width()/2+(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(3*edge)/10,height()/2,
		   width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	break;
      case TransportButton::Stop:
	p.fillRect(width()/2-edge*3/10,height()/2-edge*3/10,
		   edge*3/5,edge*3/5,QColor(on_color));
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-edge*3/10,height()/2+edge*3/10,
		   width()/2-edge*3/10,height()/2-edge*3/10);
	p.drawLine(width()/2-edge*3/10,height()/2-edge*3/10,
		   width()/2+edge*3/10,height()/2-edge*3/10);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+edge*3/10,height()/2-edge*3/10,
		   width()/2+edge*3/10,height()/2+edge*3/10);
	p.drawLine(width()/2+edge*3/10,height()/2+edge*3/10,
		   width()/2-edge*3/10,height()/2+edge*3/10);
	break;
      case TransportButton::Record:
	p.setPen(QColor(Qt::red));
	p.setBrush(QColor(Qt::red));
	p.drawEllipse(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		      (3*edge)/5,(3*edge)/5);
	break;
      case TransportButton::FastForward:
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(3*edge)/10,height()/2+(3*edge)/10,
		   width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   width()/2,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2,height()/2,
		   width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2,height()/2+(3*edge)/10,
		   width()/2,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2,height()/2-(3*edge)/10,
		   width()/2+(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(3*edge)/10,height()/2,
		   width()/2,height()/2+(3*edge)/10);
	break;
      case TransportButton::Rewind:
	triangle.setPoint(0,width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2,height()/2);
	triangle.setPoint(2,width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   width()/2,height()/2);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2,height()/2,
		   width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(3*edge)/10,height()/2+(3*edge)/10,
		   width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2-(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2,height()/2-(3*edge)/10,
		   width()/2-(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2-(3*edge)/10,height()/2,
		   width()/2,height()/2+(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2,height()/2+(3*edge)/10,
		   width()/2,height()/2-(3*edge)/10);
	break;
      case TransportButton::Eject:
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(3*edge)/10,height()/2,
		   width()/2,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2,height()/2-(3*edge)/10,
		   width()/2+(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(3*edge)/10,height()/2,
		   width()/2-(3*edge)/10,height()/2);
	p.fillRect(width()/2-(3*edge)/10,height()/2+edge/10,
		   (3*edge)/5,edge/5,on_color);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(3*edge)/10,height()/2+edge/10+edge/5,
		   width()/2-(3*edge)/10,height()/2+edge/10);
	p.drawLine(width()/2-(3*edge)/10,height()/2+edge/10,
		   width()/2+(3*edge)/10,height()/2+edge/10);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(3*edge)/10,height()/2+edge/10,
		   width()/2+(3*edge)/10,height()/2+edge/10+edge/5);
	p.drawLine(width()/2+(3*edge)/10,height()/2+edge/10+edge/5,
		   width()/2-(3*edge)/10,height()/2+edge/10+edge/5);
	break;
      case TransportButton::Pause:
	p.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   (3*edge)/15,(3*edge)/5,on_color);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(3*edge)/10,height()/2+(3*edge)/10,
		   width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.drawLine(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   width()/2-(3*edge)/10+(3*edge)/15,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2-(3*edge)/10+(3*edge)/15,height()/2-(3*edge)/10,
		   width()/2-(3*edge)/10+(3*edge)/15,height()/2+(3*edge)/10);
	p.drawLine(width()/2-(3*edge)/10+(3*edge)/15,height()/2+(3*edge)/10,
		   width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.fillRect(width()/2+(3*edge)/30,height()/2-(3*edge)/10,
		   (3*edge)/15,(3*edge)/5,on_color);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2+(3*edge)/30,height()/2+(3*edge)/10,
		   width()/2+(3*edge)/30,height()/2-(3*edge)/10);
	p.drawLine(width()/2+(3*edge)/30,height()/2-(3*edge)/10,
		   width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	p.drawLine(width()/2+(3*edge)/10,height()/2+(3*edge)/10,
		   width()/2+(3*edge)/30,height()/2+(3*edge)/10);
	break;
      case TransportButton::PlayFrom:
	p.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(2*edge)/10+1,height()/2+(3*edge)/10,
		   width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);  
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2-(2*edge)/10+1,height()/2-(3*edge)/10,
		   width()/2+(3*edge)/10+1,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(3*edge)/10+1,height()/2,
		   width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	break;
      case TransportButton::PlayBetween:
	p.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	p.fillRect(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(2*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(2*edge)/10+1,height()/2+(3*edge)/10,
		   width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2-(2*edge)/10+1,height()/2-(3*edge)/10,
		   width()/2+(2*edge)/10+1,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(2*edge)/10+1,height()/2,
		   width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	break;
      case TransportButton::Loop:
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(2*edge)/10+1,height()/2+(edge)/4,
		   width()/2+(edge)/10+1,height()/2-edge/10);
	p.setPen(QColor(colorGroup().dark()));
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(edge)/10+1,height()/2-edge/10,
		   width()/2-(2*edge)/10+1,height()/2+(edge)/4);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawArc(width()/6,height()/2-edge/9,2*width()/3,
		  height()/3+edge/10,1440,5760);
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(edge)/4);
	triangle.setPoint(1,width()/2+(edge)/10+1,height()/2-edge/10);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+edge/20);
	p.drawPolygon(triangle);
	break;
      case TransportButton::Up:
	triangle.setPoint(0,width()/2,(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()-(3*edge)/10);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()-(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(3*edge)/10,height()-(3*edge)/10,
		   width()/2,(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2,(3*edge)/10,
		   width()/2+(3*edge)/10,height()-(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(3*edge)/10,height()-(3*edge)/10,
		   width()/2-(3*edge)/10,height()-(3*edge)/10);
	break;
      case TransportButton::Down:
	triangle.setPoint(0,width()/2,height()-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,(3*edge)/10);
	triangle.setPoint(2,width()/2-(3*edge)/10,(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2-(3*edge)/10,(3*edge)/10,
		   width()/2,height()-(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2,height()-(3*edge)/10,
		   width()/2+(3*edge)/10,(3*edge)/10);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2+(3*edge)/10,(3*edge)/10,
		 width()/2-(3*edge)/10,(3*edge)/10);
	break;
      case TransportButton::PlayTo:
	p.fillRect(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(2*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(3*edge)/10,height()/2+(3*edge)/10,
		   width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   width()/2+(2*edge)/10+1,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(2*edge)/10+1,height()/2,
		   width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	break;
  }  
  p.end();
}


void TransportButton::drawOffCap()
{
  QPainter p;
  QPolygon triangle=QPolygon(3);
  int edge;

  if(height()<width()) {
    edge=height();
  }
  else {
    edge=width();
  }
  drawMask(off_cap);
  p.begin(off_cap);
  p.setPen(QColor(Qt::black));
  p.setBrush(QColor(Qt::black));
  switch(button_type) {
      case TransportButton::Play:
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(3*edge)/10,height()/2+(3*edge)/10,
		   width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   width()/2+(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(3*edge)/10,height()/2,
		   width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	break;
      case TransportButton::Stop:
	p.fillRect(width()/2-edge*3/10,height()/2-edge*3/10,
		   edge*3/5,edge*3/5,QColor(colorGroup().shadow()));
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-edge*3/10,height()/2+edge*3/10,
		   width()/2-edge*3/10,height()/2-edge*3/10);
	p.drawLine(width()/2-edge*3/10,height()/2-edge*3/10,
		   width()/2+edge*3/10,height()/2-edge*3/10);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+edge*3/10,height()/2-edge*3/10,
		   width()/2+edge*3/10,height()/2+edge*3/10);
	p.drawLine(width()/2+edge*3/10,height()/2+edge*3/10,
		   width()/2-edge*3/10,height()/2+edge*3/10);
	break;
      case TransportButton::Record:
	p.setPen(QColor(Qt::darkRed));
	p.setBrush(QColor(Qt::darkRed));
	p.drawEllipse(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		      (3*edge)/5,(3*edge)/5);
	break;
      case TransportButton::FastForward:
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(3*edge)/10,height()/2+(3*edge)/10,
		   width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   width()/2,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2,height()/2,
		   width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2,height()/2+(3*edge)/10,
		   width()/2,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2,height()/2-(3*edge)/10,
		   width()/2+(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(3*edge)/10,height()/2,
		   width()/2,height()/2+(3*edge)/10);
	break;
      case TransportButton::Rewind:
	triangle.setPoint(0,width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2,height()/2);
	triangle.setPoint(2,width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   width()/2,height()/2);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2,height()/2,
		   width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(3*edge)/10,height()/2+(3*edge)/10,
		   width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2-(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2,height()/2-(3*edge)/10,
		   width()/2-(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2-(3*edge)/10,height()/2,
		   width()/2,height()/2+(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2,height()/2+(3*edge)/10,
		   width()/2,height()/2-(3*edge)/10);
	break;
      case TransportButton::Eject:
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2);
	p.drawPolygon(triangle);
	p.fillRect(width()/2-(3*edge)/10,height()/2+edge/10,
		   (3*edge)/5,edge/5,QColor(Qt::black));		   
	break;
      case TransportButton::Pause:
	p.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   (3*edge)/15,(3*edge)/5,QColor(Qt::black));
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(3*edge)/10,height()/2+(3*edge)/10,
		   width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.drawLine(width()/2-(3*edge)/10,height()/2+(3*edge)/10,
		   width()/2-(3*edge)/10+(3*edge)/15,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2-(3*edge)/10+(3*edge)/15,height()/2-(3*edge)/10,
		   width()/2-(3*edge)/10+(3*edge)/15,height()/2+(3*edge)/10);
	p.drawLine(width()/2-(3*edge)/10+(3*edge)/15,height()/2+(3*edge)/10,
		   width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.fillRect(width()/2+(3*edge)/30,height()/2-(3*edge)/10,
		   (3*edge)/15,(3*edge)/5,QColor(Qt::black));
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2+(3*edge)/30,height()/2+(3*edge)/10,
		   width()/2+(3*edge)/30,height()/2-(3*edge)/10);
	p.drawLine(width()/2+(3*edge)/30,height()/2-(3*edge)/10,
		  width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	p.drawLine(width()/2+(3*edge)/10,height()/2+(3*edge)/10,
		   width()/2+(3*edge)/30,height()/2+(3*edge)/10);
	break;
      case TransportButton::PlayFrom:
	p.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(2*edge)/10+1,height()/2+(3*edge)/10,
		   width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2-(2*edge)/10+1,height()/2-(3*edge)/10,
		   width()/2+(3*edge)/10+1,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(3*edge)/10+1,height()/2,
		   width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	break;
      case TransportButton::PlayBetween:
	p.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	p.fillRect(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(2*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(2*edge)/10+1,height()/2+(3*edge)/10,
		   width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2-(2*edge)/10+1,height()/2-(3*edge)/10,
		   width()/2+(2*edge)/10+1,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(2*edge)/10+1,height()/2,
		   width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	break;
      case TransportButton::Loop:
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(edge)/4);
	triangle.setPoint(1,width()/2+(edge)/10+1,height()/2-edge/10);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+edge/20);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(2*edge)/10+1,height()/2+(edge)/4,
		   width()/2+(edge)/10+1,height()/2-edge/10);
	p.setPen(QColor(colorGroup().dark()));
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(edge)/10+1,height()/2-edge/10,
		   width()/2-(2*edge)/10+1,height()/2+(edge)/4);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawArc(width()/6,height()/2-edge/9,2*width()/3,
		  height()/3+edge/10,1440,5760);
	break;
      case TransportButton::Up:
	triangle.setPoint(0,width()/2,(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()-(3*edge)/10);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()-(3*edge)/10);
	p.drawPolygon(triangle);
	break;
      case TransportButton::Down:
	triangle.setPoint(0,width()/2,height()-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,(3*edge)/10);
	triangle.setPoint(2,width()/2-(3*edge)/10,(3*edge)/10);
	p.drawPolygon(triangle);
	break;
      case TransportButton::PlayTo:
	p.fillRect(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(2*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawLine(width()/2-(3*edge)/10,height()/2+(3*edge)/10,
		   width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.drawLine(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   width()/2+(2*edge)/10+1,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.drawLine(width()/2+(2*edge)/10+1,height()/2,
		   width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	break;
  }  
  p.end();
}

void TransportButton::flashOn()
{
  if(!flash_timer->isActive()) {
    flash_timer->start(500);
  }
}


void TransportButton::flashOff()
{
  if(flash_timer->isActive()) {
    flash_timer->stop();
  }
}



