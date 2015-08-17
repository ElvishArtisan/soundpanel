// array_button.cpp
//
// Array Buttons for SoundPanel.
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

#include <QtCore/QStringList>

#include <conf.h>
#include <array_button.h>

ArrayButton::ArrayButton(QWidget *parent,const char *name)
  : QPushButton(parent,name)
{
  button_parent=parent;
  clear();
}


QSize ArrayButton::sizeHint() const
{
  return QSize(80,50);
}


void ArrayButton::setLabel(QString label)
{
  button_label=label;
  SetKeycap();
}


void ArrayButton::setLength(int msecs)
{
  button_length=msecs;
  SetKeycap();
}


QColor ArrayButton::color() const
{
  return button_color;
}


void ArrayButton::setColor(QColor color)
{
  button_color=color;
  setPalette(QPalette(color,button_parent->backgroundColor()));
}


void ArrayButton::clear()
{
  button_length=-1;
  button_label="";
  setColor(backgroundColor());
  SetKeycap();
}


void ArrayButton::keyPressEvent(QKeyEvent *e)
{
  switch(e->key()) {
      case Qt::Key_Up:
      case Qt::Key_Down:
      case Qt::Key_Left:
      case Qt::Key_Right:
      case Qt::Key_Tab:
      case Qt::Key_Backtab:
      case Qt::Key_Home:
      case Qt::Key_End:
	e->ignore();
	break;

      case Qt::Key_Space:
	QPushButton::keyPressEvent(e);
	break;
  }
}


void ArrayButton::focusInEvent(QFocusEvent *e)
{
  QPushButton::focusInEvent(e);
  emit focused();
}


void ArrayButton::resizeEvent(QResizeEvent *e)
{
  SetKeycap();
  QPushButton::resizeEvent(e);
}


void ArrayButton::SetKeycap()
{
  if(button_length<0) {
    QPushButton::setText("");
  }
  else {
    QPushButton::setText(QString().sprintf("%s\n%s",
		 (const char *)WrapText(button_label),
		 (const char *)GetTimeLength(button_length,true,false)));
  }
}


QString ArrayButton::WrapText(QString text)
{
  QFontMetrics fm(font());
  int max_lines=geometry().height()/fm.height()-1;
  int lines=0;
  QString line;
  QString ret;
  int c=0;

  QStringList f0=text.split(" ");
  while(c<f0.size()) {
    if(line.isEmpty()||(lines==(max_lines-1))) {
      line+=f0[c];
      c++;
    }
    else {
      if(fm.boundingRect(line+" "+f0[c]).width()<width()) {
	line+=(" "+f0[c]);
	c++;
      }
      else {
	ret+=(line+"\n");
	lines++;
	line="";
      }
    }
  }
  if(!line.isEmpty()) {
    ret+=line;
    lines++;
  }

  return ret;
}
