//   transportbutton.h
//
//   An audio transport button widget.
//
//   (C) Copyright 2002 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: transportbutton.h,v 1.2 2008/09/06 12:02:49 fredg Exp $
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

#ifndef TRANSPORTBUTTON_H
#define TRANSPORTBUTTON_H

#include <QtGui/QtGui>

#define TRANSPORTBUTTON_DEFAULT_ON_COLOR Qt::green

class TransportButton : public QPushButton
{
  Q_OBJECT
 public:
  enum TransType {Play=0,Stop=1,Record=2,FastForward=3,Rewind=4,Eject=5,
		  Pause=6,PlayFrom=7,PlayBetween=8,Loop=9,Up=10,Down=11,
		  PlayTo=12};
  enum TransState {On=0,Off=1,Flashing=2}; 
  TransportButton(TransportButton::TransType type,QWidget *parent,
		    const char *name);
  TransportButton::TransType getType() const;
  void setType(TransportButton::TransType type);
  QColor onColor() const;
  void setOnColor(QColor color);
  QColor accentColor() const;
  void setAccentColor(QColor color);
  void setState(TransportButton::TransState state);
  
 public slots:
  void on();
  void off();
  void flash();

 protected:
  virtual void resizeEvent(QResizeEvent *);
  virtual void enabledChange(bool oldEnabled);
  
 private slots:
  void flashClock();

 private:
 void updateCaps();
 void drawMask(QPixmap *cap);
 void drawOnCap();
 void drawOffCap();
 void flashOn();
 void flashOff();
 bool flash_state;
 TransportButton::TransType button_type;
 TransportButton::TransState button_state;
 QColor on_color;
 QColor accent_color;
 QPixmap *on_cap;
 QPixmap *off_cap;
 QTimer *flash_timer;
};


#endif  // TRANSPORTBUTTON_H
