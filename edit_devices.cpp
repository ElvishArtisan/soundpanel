// edit_devices.cpp
//
// Edit SoundPanel Audio Device Configuration.
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

#include "defaults.h"
#include "edit_devices.h"

EditDevices::EditDevices(QString *playdev,QString *recdev,
			 int *playcard,int *playport,
			 SoundCard::ClockSource *clk_src,
			 SoundCard::SourceNode *type,int *reccard,
			 int *recport,
			 int *reclevel,int *playlevel,
			 QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  snd_playdev=playdev;
  snd_recdev=recdev;
  snd_clock_source=clk_src;
  snd_playcard=playcard;
  snd_playport=playport;
  snd_playlevel=playlevel;
  snd_reccard=reccard;
  snd_recport=recport;
  snd_reclevel=reclevel;
  snd_type=type;

  setCaption("Audio Devices");

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
  // HPI Array Play Device
  //
  snd_playdev_selector=new CardSelector(this, "snd_playdev_selector");
  snd_playdev_selector->setTitle("Play Device");
  snd_playdev_selector->setGeometry(35,10,
				    snd_playdev_selector->sizeHint().width(),
				    snd_playdev_selector->sizeHint().height());

  //
  // HPI Play Level
  //
  snd_outlevel_box=new QSpinBox(this,"snd_outlevel_box");
  snd_outlevel_box->setGeometry(95,75,50,19);
  snd_outlevel_box->
    setRange((SOUNDCARD_HPI_MIN_LEVEL+DEFAULT_NORMAL_LEVEL)/100,
	     (SOUNDCARD_HPI_MAX_LEVEL+DEFAULT_NORMAL_LEVEL)/100);
  QLabel *label=
    new QLabel(snd_outlevel_box,"Level (dBu):",this,"snd_outlevel_label");
  label->setGeometry(10,75,80,19);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // HPI Array Record Device
  //
  snd_recdev_selector=new CardSelector(this, "snd_recdev_selector");
  snd_recdev_selector->setTitle("Record Device");
  snd_recdev_selector->setGeometry(180,10,
				    snd_recdev_selector->sizeHint().width(),
				    snd_recdev_selector->sizeHint().height());
  connect(snd_recdev_selector,SIGNAL(cardChanged(int)),
	  this,SLOT(cardChangedData(int)));

  //
  // HPI Record Level
  //
  snd_inlevel_box=new QSpinBox(this,"snd_inlevel_box");
  snd_inlevel_box->setGeometry(240,75,50,19);
  snd_inlevel_box->
    setRange((SOUNDCARD_HPI_MIN_LEVEL+DEFAULT_NORMAL_LEVEL)/100,
	     (SOUNDCARD_HPI_MAX_LEVEL+DEFAULT_NORMAL_LEVEL)/100);
  snd_inlevel_label=
    new QLabel(snd_inlevel_box,"Level (dBu):",this,"snd_inlevel_label");
  snd_inlevel_label->setGeometry(155,75,80,19);
  snd_inlevel_label->setFont(font);
  snd_inlevel_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // HPI Input Type
  //
  snd_type_box=new QComboBox(this,"snd_type_box");
  snd_type_box->setGeometry(125,101,100,26);
  snd_type_label=new QLabel(snd_type_box,"Input Type",this,"snd_type_label");
  snd_type_label->setGeometry(15,104,105,20);
  snd_type_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  connect(snd_type_box,SIGNAL(activated(int)),
	  this,SLOT(inputTypeActivated(int)));
  
  //
  // HPI Clock Source
  //
  snd_clock_box=new QComboBox(this,"snd_clock_box");
  snd_clock_box->setGeometry(125,126,115,26);
  snd_clock_label=new QLabel(snd_clock_box,"Clock Source",
			     this,"snd_clock_label");
  snd_clock_label->setGeometry(15,129,105,20);
  snd_clock_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  
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
  snd_playdev_selector->setCard(*playcard);
  snd_playdev_selector->setPort(*playport);
  snd_outlevel_box->setValue(*snd_playlevel/100);
  snd_recdev_selector->setCard(*reccard);
  snd_recdev_selector->setPort(*recport);
  snd_inlevel_box->setValue(*snd_reclevel/100);
  snd_type_box->insertItem("Analog");
  snd_type_box->insertItem("Digital");
  switch(*snd_type) {
      case SoundCard::LineIn:
	snd_type_box->setCurrentItem(0);
	inputTypeActivated(0);
	break;
      case SoundCard::AesEbuIn:
	snd_type_box->setCurrentItem(1);
	inputTypeActivated(1);
	break;
      default:
	snd_type_box->setCurrentItem(0);
	inputTypeActivated(0);
	break;
  }
  snd_clock_box->insertItem("Internal");
  snd_clock_box->insertItem("AES3");
  snd_clock_box->insertItem("Word Clock");
  switch(*snd_clock_source) {
      case SoundCard::Internal:
	snd_clock_box->setCurrentItem(0);
	break;

      case SoundCard::AesEbu:
      case SoundCard::SpDiff:
	snd_clock_box->setCurrentItem(1);
	break;

      case SoundCard::WordClock:
	snd_clock_box->setCurrentItem(2);
	break;
  }
  cardChangedData(*reccard);
}


EditDevices::~EditDevices()
{
}


QSize EditDevices::sizeHint() const
{
  return QSize(310,221);
}


QSizePolicy EditDevices::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditDevices::cardChangedData(int card)
{
  if(card<0) {
    snd_type_label->setDisabled(true);
    snd_type_box->setDisabled(true);
  }
  else {
    snd_type_label->setEnabled(true);
    snd_type_box->setEnabled(true);
  }
}


void EditDevices::inputTypeActivated(int id)
{
  switch(id) {
      case 0:
	snd_inlevel_label->setEnabled(true);
	snd_inlevel_box->setEnabled(true);
	break;

      case 1:
	snd_inlevel_label->setDisabled(true);
	snd_inlevel_box->setDisabled(true);
	break;
  }
}


void EditDevices::okData()
{
  *snd_playcard=snd_playdev_selector->card();
  *snd_playport=snd_playdev_selector->port();
  *snd_playlevel=100*snd_outlevel_box->value();
  *snd_reccard=snd_recdev_selector->card();
  *snd_recport=snd_recdev_selector->port();
  *snd_reclevel=100*snd_inlevel_box->value();
  switch(snd_type_box->currentItem()) {
      case 0:
	*snd_type=SoundCard::LineIn;
	break;
      case 1:
	*snd_type=SoundCard::AesEbuIn;
	break;
  }
  switch(snd_clock_box->currentItem()) {
      case 0:
	*snd_clock_source=SoundCard::Internal;
	break;

      case 1:
	*snd_clock_source=SoundCard::AesEbu;
	break;

      case 2:
	*snd_clock_source=SoundCard::WordClock;
	break;
  }
  done(0);
}


void EditDevices::cancelData()
{
  done(1);
}


void EditDevices::closeEvent(QCloseEvent *e)
{
  cancelData();
}

