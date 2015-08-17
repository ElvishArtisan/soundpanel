// panel.h
//
// Abstract a SoundPanel button panel.
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

#ifndef PANEL_H
#define PANEL_H

#include <QtGui/QtGui>

#include <defaults.h>

class Panel
{
  public:
   Panel();
   ~Panel();
   void initColor(QColor);
   QString name() const;
   void setName(QString name);
   QString legend(int offset) const;
   QString legend(int col,int row) const;
   void setLegend(int offset,QString legend);
   void setLegend(int col,int row,QString legend);
   QString wavefile(int offset) const;
   QString wavefile(int col,int row) const;
   void setWavefile(int offset,QString name);
   void setWavefile(int col,int row,QString name);
   int length(int offset);
   int length(int col,int row);
   QColor color(int offset) const;
   QColor color(int col,int row) const;
   void setColor(int offset,QColor color);
   void setColor(int col,int row,QColor color);

  private:
   QString panel_name;
   QString panel_legend[BUTTON_COLUMNS*BUTTON_ROWS];
   QString panel_wavefile[BUTTON_COLUMNS*BUTTON_ROWS];
   QColor panel_color[BUTTON_COLUMNS*BUTTON_ROWS];
};


#endif

