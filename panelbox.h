// panelbox.h
//
// A QComboBox with a right-click signal.
//
//   (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
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

#ifndef PANELBOX_H
#define PANELBOX_H

#include <QtGui/QtGui>

class PanelBox : public QComboBox
{
  Q_OBJECT
  public:
   PanelBox(QWidget *parent=0,const char *name=0);

  protected:
   void mousePressEvent(QMouseEvent *e);

  signals:
   void rightPressed(QPoint);
};


#endif

