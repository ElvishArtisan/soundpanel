// snd_properties.cpp
//
// Edit SoundPanel array Properties.
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

#include "snd_properties.h"

SndProperties::SndProperties(QString *title,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  snd_title=title;

  setCaption("Array Properties");

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont button_font=QFont("Helvetica",12,QFont::Bold);
  button_font.setPixelSize(12);

  //
  // Array Title
  //
  snd_title_edit=new QLineEdit(this,"snd_title_edit");
  snd_title_edit->setGeometry(55,10,200,20);
  snd_title_edit->setText(QString(*snd_title));
  QLabel *label=new QLabel(snd_title_edit,"&Title:",
			   this,"snd_title_label");
  label->setGeometry(10,10,40,20);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  
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
  snd_title_edit->setText(*snd_title);
}


SndProperties::~SndProperties()
{
}


QSize SndProperties::sizeHint() const
{
  return QSize(265,105);
}


QSizePolicy SndProperties::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void SndProperties::okData()
{
  *snd_title=snd_title_edit->text();
  done(0);
}


void SndProperties::cancelData()
{
  done(1);
}


void SndProperties::closeEvent(QCloseEvent *e)
{
  cancelData();
}

