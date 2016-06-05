// edit_ripper.cpp
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

#include <stdlib.h>

#include "edit_ripper.h"
#include "wavefile.h"

EditRipper::EditRipper(QString *ripdev,int *paranoia_level,QString *cddb_serv,
		       int *normal_level,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  snd_ripdev=ripdev;
  snd_paranoia_level=paranoia_level;
  snd_cddb_server=cddb_serv;
  snd_normal_level=normal_level;

  setCaption("CD Ripper");

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  //
  // Generate Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont button_font=QFont("Helvetica",12,QFont::Bold);
  button_font.setPixelSize(12);

  //
  // Ripper Device
  //
  snd_ripdev_edit=new QLineEdit(this,"snd_ripdev_edit");
  snd_ripdev_edit->setGeometry(125,10,100,20);
  snd_ripdev_edit->setFont(font);
  snd_ripdev_edit->setText(QString(*snd_ripdev));
  QLabel *label=new QLabel(snd_ripdev_edit,"CD Ripper Device:",
			   this,"snd_ripdev_label");
  label->setGeometry(10,10,110,20);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);  
  
  //
  // Paranoia Level
  //
  snd_paranoia_box=new QComboBox(this,"snd_paranoia_box");
  snd_paranoia_box->setGeometry(125,36,100,20);
  snd_paranoia_box->setFont(font);
  label=new QLabel(snd_paranoia_box,"Paranoia Level:",
			   this,"snd_ripdev_label");
  label->setGeometry(10,36,110,20);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);  
  
  //
  // CDDB Server
  //
  snd_cddb_edit=new QLineEdit(this,"snd_cddb_edit");
  snd_cddb_edit->setGeometry(125,62,120,20);
  snd_cddb_edit->setFont(font);
  snd_cddb_edit->setText(QString(*snd_cddb_server));
  label=new QLabel(snd_cddb_edit,"FreeDB Server:",
		   this,"snd_cddb_label");
  label->setGeometry(10,62,110,20);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);  
  
  //
  // Normalization Level
  //
  snd_normal_box=new QSpinBox(this,"snd_normal_box");
  snd_normal_box->setGeometry(125,88,40,20);
  snd_normal_box->setFont(font);
  snd_normal_box->setRange(-30,0);
  snd_normal_box->setValue(*snd_normal_level/100);
  label=new QLabel(snd_normal_box,"Normal Level:",
		   this,"snd_cddb_label");
  label->setGeometry(10,88,110,20);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);  
  label=new QLabel(snd_normal_box,"dBFS",
		   this,"snd_unit_label");
  label->setGeometry(170,88,50,20);
  label->setFont(font);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  
  //
  // OK Button
  //
  QPushButton *button=new QPushButton("&OK",this,"ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setFont(button_font);
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
  snd_ripdev_edit->setText(*snd_ripdev);
  snd_paranoia_box->insertItem("Normal");
  snd_paranoia_box->insertItem("Low");
  snd_paranoia_box->insertItem("None");
  snd_paranoia_box->setCurrentItem(*snd_paranoia_level);
  snd_cddb_edit->setText(*snd_cddb_server);
}


EditRipper::~EditRipper()
{
   delete snd_ripdev_edit;
   delete snd_paranoia_box;
}


QSize EditRipper::sizeHint() const
{
  return QSize(260,182);
}


QSizePolicy EditRipper::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditRipper::okData()
{
  *snd_ripdev=snd_ripdev_edit->text();
  *snd_paranoia_level=snd_paranoia_box->currentItem();
  *snd_cddb_server=snd_cddb_edit->text();
  *snd_normal_level=100*snd_normal_box->value();
  done(0);
}


void EditRipper::cancelData()
{
  done(1);
}


void EditRipper::closeEvent(QCloseEvent *e)
{
  cancelData();
}


