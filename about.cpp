// about.cpp
//
// Information Screen for SoundPanel.
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

#include <QLabel>
#include <QPushButton>

#include "about.h"

About::About(QWidget *parent,const char *name) : QDialog(parent,name)
{
  setCaption("About SoundPanel");

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
  QFont big_font=QFont("Helvetica",18,QFont::Bold);
  big_font.setPixelSize(18);
  QFont demi_font=QFont("Helvetica",14,QFont::DemiBold);
  demi_font.setPixelSize(14);
  QFont small_font=QFont("Helvetica",12,QFont::Normal);
  small_font.setPixelSize(12);
  QFont bold_font=QFont("Helvetica",12,QFont::Bold);
  bold_font.setPixelSize(12);

  QLabel *label=new QLabel("SoundPanel",this,"name_label");
  label->setGeometry(10,10,120,24);
  label->setFont(big_font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  
  label=new QLabel(QString().sprintf("Version %s",VERSION),this,"name_label");
  label->setGeometry(140,10,130,24);
  label->setFont(demi_font);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  
  label=new QLabel(
    QString("(C) Copyright 2003-2007 Fred Gleason <fredg@paravelsystems.com>"),
		   this,"name_label");
  label->setGeometry(10,32,sizeHint().width()-20,22);
  label->setFont(small_font);

  label=new QLabel(QString("Audio Driver:"),
		   this,"driver_label");
  label->setGeometry(10,56,100,14);
  label->setFont(bold_font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  
  label=new QLabel(QString("AudioScience HPI"),
		   this,"driver_label");
  label->setGeometry(115,56,sizeHint().width()-215,14);
  label->setFont(small_font);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  label=new QLabel(QString("This program is free software, and comes with \
absolutely\nno warranty.  See the GNU Public License for details."),
		   this,"name_label");
  label->setGeometry(10,74,sizeHint().width()-110,46);
  label->setFont(small_font);

  //
  // Close Button
  //
  QPushButton *button=new QPushButton("&Close",this,"close_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(demi_font);
  connect(button,SIGNAL(clicked()),this,SLOT(closeData()));
}


About::~About()
{
}


QSize About::sizeHint() const
{
  return QSize(500,130);
}


QSizePolicy About::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void About::closeData()
{
  done(0);
}
