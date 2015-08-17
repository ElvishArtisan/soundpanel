// edit_editor.cpp
//
// Edit SoundPanel Wave Editor Configuration.
//
//   (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
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

#include <edit_editor.h>


EditEditor::EditEditor(QString *editor,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_editor=editor;

  setCaption("Wave Editor");

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
  // Wave Editor
  //
  edit_editor_edit=new QLineEdit(this,"edit_editor_edit");
  edit_editor_edit->setGeometry(135,10,130,20);
  edit_editor_edit->setFont(font);
  edit_editor_edit->setText(QString(*edit_editor));
  QLabel *label=new QLabel(edit_editor_edit,"Editor Command:",
			   this,"edit_editor_label");
  label->setGeometry(10,10,120,20);
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
  edit_editor_edit->setText(*edit_editor);
}


EditEditor::~EditEditor()
{
   delete edit_editor_edit;
}


QSize EditEditor::sizeHint() const
{
  return QSize(280,108);
}


QSizePolicy EditEditor::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditEditor::okData()
{
  *edit_editor=edit_editor_edit->text();
  done(0);
}


void EditEditor::cancelData()
{
  done(1);
}


void EditEditor::closeEvent(QCloseEvent *e)
{
  cancelData();
}

