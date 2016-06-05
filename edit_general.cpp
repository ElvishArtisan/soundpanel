// edit_general.cpp
//
// Edit SoundPanel General Configuration.
//
//   (C) Copyright 2002-2004,2016 Fred Gleason <fredg@paravelsystems.com>
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

#include "conf.h"
#include "edit_general.h"

EditGeneral::EditGeneral(EditGeneral::StartupArray *array_mode,QString *array,
			 QString *path,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_array=array;
  edit_startup_mode=array_mode;
  edit_path=path;

  setCaption("General Settings");

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
  // Array Startup Mode
  //
  edit_mode_box=new QComboBox(this,"edit_mode_box");
  edit_mode_box->setGeometry(105,10,130,20);
  edit_mode_box->setFont(font);
  edit_mode_box->insertItem("Blank");
  edit_mode_box->insertItem("Last Loaded");
  edit_mode_box->insertItem("Specified");
  QLabel *label=new QLabel(edit_mode_box,"Default Array:",
			   this,"edit_editor_label");
  label->setGeometry(10,10,90,20);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);  
  connect(edit_mode_box,SIGNAL(activated(int)),
	  this,SLOT(modeActivatedData(int)));

  //
  // Startup Array
  //
  edit_array_edit=new QLineEdit(this,"edit_array_edit");
  edit_array_edit->setGeometry(105,40,310,20);
  edit_array_edit->setFont(font);
  edit_array_label=new QLabel(edit_array_edit,"Startup Array:",
			   this,"edit_array_label");
  edit_array_label->setGeometry(10,40,90,20);
  edit_array_label->setFont(font);
  edit_array_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);  

  //
  // Browse Button
  //
  edit_browse_button=new QPushButton("&Browse",this,"browse_button");
  edit_browse_button->setGeometry(425,33,60,28);
  edit_browse_button->setFont(font);
  connect(edit_browse_button,SIGNAL(clicked()),this,SLOT(browseData()));
  
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
  edit_mode_box->setCurrentItem(*edit_startup_mode);
  modeActivatedData(*edit_startup_mode);
  edit_array_edit->setText(QString(*edit_array));
}


EditGeneral::~EditGeneral()
{
//   delete edit_editor_edit;
}


QSize EditGeneral::sizeHint() const
{
  return QSize(500,138);
}


QSizePolicy EditGeneral::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditGeneral::modeActivatedData(int index)
{
  switch((EditGeneral::StartupArray)index) {
      case EditGeneral::None:
      case EditGeneral::Previous:
	edit_array_label->setDisabled(true);
	edit_array_edit->setDisabled(true);
	edit_browse_button->setDisabled(true);
	break;

      case EditGeneral::Specified:
	edit_array_label->setEnabled(true);
	edit_array_edit->setEnabled(true);
	edit_browse_button->setEnabled(true);
	break;
  }
}


void EditGeneral::browseData()
{
  QString filename;

  if(edit_array_edit->text().isEmpty()) {
    filename=
      QFileDialog::getOpenFileName(*edit_path,
				   "Array Files (*.aar)",this);
  }
  else {
    filename=
      QFileDialog::getOpenFileName(edit_array_edit->text(),
				   "Array Files (*.aar)",this);
  }
  if(!filename.isEmpty()) {
    edit_array_edit->setText(filename);
    *edit_path=GetPathPart(edit_array_edit->text());
  }
}


void EditGeneral::okData()
{
  *edit_startup_mode=(EditGeneral::StartupArray)edit_mode_box->currentItem();
  *edit_array=edit_array_edit->text();
  done(0);
}


void EditGeneral::cancelData()
{
  done(1);
}


void EditGeneral::closeEvent(QCloseEvent *e)
{
  cancelData();
}

