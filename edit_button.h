// edit_button.h
//
// Edit a SoundPanel Audio Button.
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

#ifndef EDIT_BUTTON_H
#define EDIT_BUTTON_H

#include <QtGui>

class EditButton : public QDialog
{
  Q_OBJECT
  public:
  EditButton(QString *legend,QString *wavefile,QString *path,
	     QColor *color,bool *use_cc,QWidget *parent=0,const char *name=0);
   ~EditButton();
   QSize sizeHint() const;
   QSizePolicy sizePolicy() const;

  private slots:
   void browseData();
   void colorData();
   void okData();
   void cancelData();

 protected:
   void closeEvent(QCloseEvent *e);

  private:
   QLineEdit *edit_legend_edit;
   QLineEdit *edit_wavefile_edit;
   QString *edit_legend;
   QString *edit_wavefile;
   QString *edit_path;
   QColor *edit_color;
   QColor edit_current_color;
   bool *edit_cc;
   QPushButton *edit_color_button;
};


#endif

