// search_array.cpp
//
// Get a Search String.
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

#include "search_array.h"

SearchArray::SearchArray(QString *search,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  search_search=search;

  setCaption("Search");

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
  // Search String
  //
  search_search_edit=new QLineEdit(this,"search_search_edit");
  search_search_edit->setGeometry(45,10,sizeHint().width()-55,20);
  search_search_edit->setFont(font);
  search_search_edit->setText(QString(*search_search));
  QLabel *label=new QLabel(search_search_edit,"Cut:",
			   this,"search_search_label");
  label->setGeometry(10,10,30,20);
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
  search_search_edit->setText(*search_search);
}


SearchArray::~SearchArray()
{
   delete search_search_edit;
}


QSize SearchArray::sizeHint() const
{
  return QSize(190,108);
}


QSizePolicy SearchArray::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void SearchArray::okData()
{
  *search_search=search_search_edit->text();
  done(0);
}


void SearchArray::cancelData()
{
  done(1);
}


void SearchArray::closeEvent(QCloseEvent *e)
{
  cancelData();
}
