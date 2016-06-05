// edit_ripper.h
//
// Edit SoundPanel CD Ripper Configuration.
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

#ifndef EDIT_RIPPER_H
#define EDIT_RIPPER_H

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

class EditRipper : public QDialog
{
  Q_OBJECT
 public:
  EditRipper(QString *ripdev,int *paranoia_level,QString *cddb_serv,
	     int *normal_level,QWidget *parent=0,const char *name=0);
  ~EditRipper();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void okData();
  void cancelData();

 protected:
   void closeEvent(QCloseEvent *e);

 private:
  QLineEdit *snd_ripdev_edit;
  QComboBox *snd_paranoia_box;
  QLineEdit *snd_cddb_edit;
  QSpinBox *snd_normal_box;
  QString *snd_ripdev;
  int *snd_paranoia_level;
  QString *snd_cddb_server;
  int *snd_normal_level;
};


#endif

