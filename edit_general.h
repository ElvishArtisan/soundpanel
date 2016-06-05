// edit_general.h
//
// Edit SoundPanel General Configuration.
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef EDIT_GENERAL_H
#define EDIT_GENERAL_H

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>

class EditGeneral : public QDialog
{
  Q_OBJECT
  public:
   enum StartupArray {None=0,Previous=1,Specified=2};
  EditGeneral(EditGeneral::StartupArray *array_mode,QString *array,
	      QString *path,QWidget *parent=0,const char *name=0);
   ~EditGeneral();
   QSize sizeHint() const;
   QSizePolicy sizePolicy() const;

  private slots:
   void modeActivatedData(int index);
   void browseData();
   void okData();
   void cancelData();

 protected:
   void closeEvent(QCloseEvent *e);

  private:
   QComboBox *edit_mode_box;
   QLabel *edit_array_label;
   QPushButton *edit_browse_button;
   QLineEdit *edit_array_edit;
   QString *edit_array;
   EditGeneral::StartupArray *edit_startup_mode;
   QString *edit_path;
};


#endif

