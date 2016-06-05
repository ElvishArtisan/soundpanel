//   pushbutton.cpp
//
//   An flashing button widget.
//
//   (C) Copyright 2002-2003,2016 Fred Gleason <fredg@paravelsystems.com>
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

#include <QMouseEvent>

#include "pushbutton.h"

PushButton::PushButton(QWidget *parent=0,const char *name) :
  QPushButton(parent,name)
{
  Init();
}


PushButton::PushButton(const QString &text,QWidget *parent,const char *name)
  : QPushButton(text,parent,name)
{
  Init();
}

PushButton::PushButton(const QIcon &icon,const QString &text,
			 QWidget *parent,const char *name)
  : QPushButton(text,parent,name)
{
  Init();
}


QColor PushButton::flashColor() const
{
  return flash_color;
}


void PushButton::setFlashColor(QColor color)
{
  int h=0;
  int s=0;
  int v=0;

  flash_color=color;  
  flash_palette=QPalette(QColor(flash_color),backgroundColor());

  color.getHsv(&h,&s,&v);
  if((h>180)&&(h<300)) {
    v=255;
  }
  else {
    if(v<168) {
      v=255;
    }
    else {
      v=0;
    }
  }
  s=0;
  color.setHsv(h,s,v);
  flash_palette.setColor(QPalette::Active,QColorGroup::ButtonText,color);
  flash_palette.setColor(QPalette::Inactive,QColorGroup::ButtonText,color);
}


bool PushButton::flashingEnabled() const
{
  return flashing_enabled;
}


void PushButton::setFlashingEnabled(bool state)
{
  flashing_enabled=state;
  if(flashing_enabled) {
    flashOn();
  }
  else {
    flashOff();
  }
}


int PushButton::id() const
{
  return button_id;
}


void PushButton::setId(int id)
{
  button_id=id;
}


void PushButton::setPalette(const QPalette &pal)
{
  off_palette=pal;
  QPushButton::setPalette(pal);
}


void PushButton::mousePressEvent(QMouseEvent *e)
{
  switch(e->button()) {
      case Qt::LeftButton:
	QPushButton::mousePressEvent(e);
	break;
	
      case Qt::MidButton:
	emit centerPressed();
	break;
	
      case Qt::RightButton:
	emit rightPressed();
	break;

      default:
	break;
  }
}


void PushButton::mouseReleaseEvent(QMouseEvent *e)
{
  switch(e->button()) {
      case Qt::LeftButton:
	QPushButton::mouseReleaseEvent(e);
	break;
	
      case Qt::MidButton:
	e->accept();
	emit centerReleased();
	if((e->x()>=0)&&(e->x()<geometry().width())&&
	   (e->y()>=0)&&(e->y()<geometry().height())) {
	  emit centerClicked();
	  emit centerClicked(button_id,QPoint(e->x(),e->y()));
	}
	break;
	
      case Qt::RightButton:
	e->accept();
	emit rightReleased();
	if((e->x()>=0)&&(e->x()<geometry().width())&&
	   (e->y()>=0)&&(e->y()<geometry().height())) {
	  emit rightClicked();
	  emit rightClicked(button_id,QPoint(e->x(),e->y()));
	}
	break;

      default:
	break;
  }
}


int PushButton::flashPeriod() const
{
  return flash_period;
}


void PushButton::setFlashPeriod(int period)
{
  flash_period=period;
  if(flash_timer->isActive()) {
    flash_timer->changeInterval(flash_period);
  }
}


PushButton::ClockSource PushButton::clockSource() const
{
  return flash_clock_source;
}


void PushButton::setClockSource(ClockSource src)
{
  if(src==flash_clock_source) {
    return;
  }
  flash_clock_source=src;
  if((src==PushButton::ExternalClock)&&(flash_timer->isActive())) {
    flash_timer->stop();
  }
  if((src==PushButton::InternalClock)&&flashing_enabled) {
    flashOn();
  }
}


void PushButton::tickClock()
{
  if(!flashing_enabled) {
    return;
  }
  QKeySequence a=accel();
  if(flash_state) {
    flash_state=false;
    QPushButton::setPalette(flash_palette);
  }
  else {
    flash_state=true;
    QPushButton::setPalette(off_palette);
  }
  setAccel(a);
}


void PushButton::tickClock(bool state)
{
  if(!flashing_enabled) {
    return;
  }
  QKeySequence a=accel();
  if(state) {
    flash_state=false;
    QPushButton::setPalette(flash_palette);
  }
  else {
    flash_state=true;
    QPushButton::setPalette(off_palette);
  }
  setAccel(a);
}


void PushButton::flashOn()
{
  if((!flash_timer->isActive())&&
     (flash_clock_source==PushButton::InternalClock)) {
    flash_timer->start(flash_period);
  }
}


void PushButton::flashOff()
{
  if(flash_timer->isActive()&&
     (flash_clock_source==PushButton::InternalClock)) {
    flash_timer->stop();
  }
  setPalette(off_palette);
}


void PushButton::Init()
{
  flash_timer=new QTimer();
  connect(flash_timer,SIGNAL(timeout()),this,SLOT(tickClock()));
  flash_state=true;
  flashing_enabled=false;
  off_palette=palette();
  flash_clock_source=PushButton::InternalClock;
  flash_period=PUSHBUTTON_DEFAULT_FLASH_PERIOD;
  setFlashColor(PUSHBUTTON_DEFAULT_FLASH_COLOR);
  button_id=-1;
}


