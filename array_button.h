// array_button.h
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

#ifndef ARRAY_BUTTON_H
#define ARRAY_BUTTON_H

#include <QtGui/QtGui>


class ArrayButton : public QPushButton
{
  Q_OBJECT
 public:
  ArrayButton(QWidget *parent=0,const char *name=0);
  QSize sizeHint() const;
  void setLabel(QString label);
  void setLength(int msecs);
  QColor color() const;
  void setColor(QColor color);
  void clear();

 signals:
  void focused();

 protected:
  void keyPressEvent(QKeyEvent *);
  void focusInEvent(QFocusEvent *);
  void resizeEvent(QResizeEvent *);

 private:
  void SetKeycap();
  QString WrapText(QString text);
  QString button_label;
  int button_length;
  QColor button_color;
  QWidget *button_parent;
};


#endif

