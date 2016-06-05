// panel.cpp
//
// Abstract a SoundPanel button panel.
//
//   (C) Copyright 2002-2003,2016 Fred Gleason <fredg@paravelsystems.com>
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

#include "panel.h"
#include "wavefile.h"

Panel::Panel()
{
}


void Panel::initColor(QColor color)
{
  for(int i=0;i<(BUTTON_COLUMNS*BUTTON_ROWS);i++) {
    panel_color[i]=color;
  }
}


Panel::~Panel()
{
}


QString Panel::name() const
{
  return panel_name;
}


void Panel::setName(QString name)
{
  panel_name=name;
}


QString Panel::legend(int offset) const
{
  return panel_legend[offset];
}


QString Panel::legend(int col,int row) const
{
  return panel_legend[row*BUTTON_COLUMNS+col];
}


void Panel::setLegend(int offset,QString legend)
{
  panel_legend[offset]=legend;
}


void Panel::setLegend(int col,int row,QString legend)
{
  panel_legend[row*BUTTON_COLUMNS+col]=legend;
}


QString Panel::wavefile(int offset) const
{
  return panel_wavefile[offset];
}


QString Panel::wavefile(int col,int row) const
{
  return panel_wavefile[row*BUTTON_COLUMNS+col];
}


void Panel::setWavefile(int offset,QString name)
{
  panel_wavefile[offset]=name;
}


void Panel::setWavefile(int col,int row,QString name)
{
  panel_wavefile[row*BUTTON_COLUMNS+col]=name;
}


int Panel::length(int offset)
{
  int l;

  WaveFile *wave=new WaveFile(panel_wavefile[offset]);
  if(!wave->openWave()) {
    delete wave;
    return -1;
  }
  l=wave->getExtTimeLength();
  wave->closeWave();
  delete wave;
  return l;
}


int Panel::length(int col,int row)
{
  return length(row*BUTTON_COLUMNS+col);
}


QColor Panel::color(int offset) const
{
  return panel_color[offset];
}


QColor Panel::color(int col,int row) const
{
  return color(row*BUTTON_COLUMNS+col);
}


void Panel::setColor(int offset,QColor color)
{
  panel_color[offset]=color;
}


void Panel::setColor(int col,int row,QColor color)
{
  panel_color[row*BUTTON_COLUMNS+col]=color;
}

