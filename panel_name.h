// panel_name.h
//
// Edit SoundPanel Panel Name
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

#ifndef PANEL_NAME_H
#define PANEL_NAME_H

#include <QDialog>
#include <QLineEdit>
#include <QString>

class PanelName : public QDialog
{
  Q_OBJECT
  public:
   PanelName(QString *panel,QWidget *parent=0,const char *name=0);
   ~PanelName();
   QSize sizeHint() const;
   QSizePolicy sizePolicy() const;

  private slots:
   void okData();
   void cancelData();

  private:
   QLineEdit *panel_panel_edit;
   QString *panel_panel;
};


#endif

