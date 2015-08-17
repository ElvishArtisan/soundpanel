// edit_button.cpp
//
// Edit a SoundPanel Audio Button.
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

#include <conf.h>

#include <edit_button.h>
#include <globals.h>

EditButton::EditButton(QString *legend,QString *wavefile,
		       QString *path,QColor *color,bool *cc,
		       QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_legend=legend;
  edit_wavefile=wavefile;
  edit_path=path;
  edit_color=color;
  edit_current_color=*color;
  edit_cc=cc;

  setCaption("Button Properties");

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
  // Legend
  //
  edit_legend_edit=new QLineEdit(this,"edit_legend_edit");
  edit_legend_edit->setGeometry(80,10,sizeHint().width()-90,20);
  edit_legend_edit->setFont(font);
  edit_legend_edit->setText(QString(*edit_legend));
  QLabel *label=new QLabel(edit_legend_edit,"&Legend:",
			   this,"edit_legend_label");
  label->setGeometry(10,12,65,20);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight);

  //
  // Wavefile
  //
  edit_wavefile_edit=new QLineEdit(this,"edit_wavefile_edit");
  edit_wavefile_edit->setGeometry(80,34,300,20);
  edit_wavefile_edit->setFont(font);
  edit_wavefile_edit->setText(*edit_wavefile);
  label=new QLabel(edit_wavefile_edit,"&Wave File:",
			   this,"edit_wavefile_label");
  label->setGeometry(10,36,65,20);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight);

  //
  // Browse Button
  //
  QPushButton *button=new QPushButton("&Browse",this,"browse_button");
  button->setGeometry(395,30,60,28);
  button->setFont(font);
  connect(button,SIGNAL(clicked()),this,SLOT(browseData()));

  //
  // Color Button
  //
  edit_color_button=new QPushButton("Co&lor",this,"edit_color_button");
  edit_color_button->
    setGeometry(10,sizeHint().height()-60,80,50);
  edit_color_button->setDefault(true);
  edit_color_button->setFont(button_font);
  connect(edit_color_button,SIGNAL(clicked()),this,SLOT(colorData()));

  //
  // OK Button
  //
  button=new QPushButton("&OK",this,"ok_button");
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
  // Replicate Data
  //
  edit_legend_edit->setText(*edit_legend);
  edit_color_button->setPalette(QPalette(*edit_color,backgroundColor()));
}


EditButton::~EditButton()
{
}


QSize EditButton::sizeHint() const
{
  return QSize(475,130);
}


QSizePolicy EditButton::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditButton::browseData()
{
  QString filename;

  if(edit_wavefile_edit->text().isEmpty()) {
    filename=
      QFileDialog::getOpenFileName(*edit_path,
				   array_file_filter,this);
  }
  else {
    filename=
      QFileDialog::getOpenFileName(edit_wavefile_edit->text(),
				   array_file_filter,this);
  }
  if(!filename.isEmpty()) {
    edit_wavefile_edit->setText(filename);
    *edit_path=GetPathPart(edit_wavefile_edit->text());
  }
}


void EditButton::colorData()
{
  edit_current_color=QColorDialog::getColor(edit_current_color,this);
  edit_color_button->
    setPalette(QPalette(edit_current_color,backgroundColor()));
}


void EditButton::okData()
{
  if(edit_legend_edit->text().isEmpty()) {
    *edit_legend=GetBasePart(edit_wavefile_edit->text());
  }
  else {
    *edit_legend=edit_legend_edit->text();
  }
  *edit_wavefile=edit_wavefile_edit->text();
  *edit_color=edit_current_color;

  done(0);
}


void EditButton::cancelData()
{
  done(1);
}


void EditButton::closeEvent(QCloseEvent *e)
{
  cancelData();
}
