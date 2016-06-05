// panel_name.cpp
//
// Edit SoundPanel Panel Name.
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

#include <stdlib.h>

#include <QLabel>
#include <QPushButton>

#include "panel_name.h"

PanelName::PanelName(QString *panel,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  panel_panel=panel;

  setCaption("Set Panel Name");

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont button_font=QFont("Helvetica",14,QFont::Bold);
  button_font.setPixelSize(14);

  //
  // Panel Name
  //
  panel_panel_edit=new QLineEdit(this,"panel_panel_edit");
  panel_panel_edit->setGeometry(95,10,130,20);
  panel_panel_edit->setFont(font);
  panel_panel_edit->setText(QString(*panel_panel));
  QLabel *label=new QLabel(panel_panel_edit,"Panel Name:",
			   this,"panel_panel_label");
  label->setGeometry(10,14,80,20);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight);  
  
  //
  // OK Button
  //
  QPushButton *button=new QPushButton("&OK",this,"ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(button_font);
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  button=new QPushButton("&Cancel",this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Populate Data
  //
  panel_panel_edit->setText(*panel_panel);
}


PanelName::~PanelName()
{
   delete panel_panel_edit;
}


QSize PanelName::sizeHint() const
{
  return QSize(240,108);
}


QSizePolicy PanelName::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void PanelName::okData()
{
  *panel_panel=panel_panel_edit->text();
  done(0);
}


void PanelName::cancelData()
{
  done(1);
}
