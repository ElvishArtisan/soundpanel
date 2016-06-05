// cardselector.cpp
//
// Audio card selector widget for Rivendell
//
//   (C) Copyright 2002-2007,2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
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

#include "cardselector.h"
#include "soundcard.h"

CardSelector::CardSelector(QWidget *parent,const char *name)
  : QWidget(parent,name)
{
  QFont font;

  yoffset=0;

  //
  // Generate Font
  //
  font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);

  //
  // Title
  //
  card_title=new QLabel(this,"card_title");
  card_title->setGeometry(0,0,geometry().width(),19);
  card_title->setFont(font);
  card_title->setAlignment(Qt::AlignHCenter);
  card_title->hide();

  //
  // Card
  //
  card_card_box=new QSpinBox(this,"card_card_box");
  card_card_box->setGeometry(60,yoffset,50,19);
  card_card_box->setSpecialValueText("None");
  card_card_box->setMinValue(-1);
  card_card_box->setMaxValue(HPI_MAX_ADAPTERS-1);
  card_card_box->setValue(-1);
  connect(card_card_box,SIGNAL(valueChanged(int)),this,SLOT(cardData(int)));
  card_card_label=new QLabel(card_card_box,"Card:",this,
				       "card_card_label");
  card_card_label->setGeometry(0,yoffset+2,55,19);
  card_card_label->setAlignment(Qt::AlignRight|Qt::TextShowMnemonic);

  //
  // Port
  //
  card_port_box=new QSpinBox(this,"card_port_box");
  card_port_box->setGeometry(60,yoffset+22,50,19);
  card_port_box->setMinValue(0);
  card_port_box->setMaxValue(HPI_MAX_NODES-1);
  card_port_box->setDisabled(true);
  connect(card_port_box,SIGNAL(valueChanged(int)),this,SLOT(portData(int)));
  card_port_label=new QLabel(card_port_box,"Port:",this,
				       "card_port_label");
  card_port_label->setGeometry(0,yoffset+24,55,19);
  card_port_label->setAlignment(Qt::AlignRight|Qt::TextShowMnemonic);
}


CardSelector::~CardSelector()
{
  delete card_title;
  delete card_card_box;
  delete card_port_box;
}


QSize CardSelector::sizeHint() const
{
  return QSize(110,43+yoffset);
} 


QSizePolicy CardSelector::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


QString CardSelector::title() const
{
  return card_title->text();
}


void CardSelector::setTitle(QString title)
{
  card_title->setText(title);
  if(title.isEmpty()) {
    yoffset=0;
    card_title->hide();
  }
  else {
    yoffset=22;
    card_title->show();
  }
  card_card_box->setGeometry(60,yoffset,50,19);
  card_card_label->setGeometry(0,yoffset+2,55,19);
  card_port_box->setGeometry(60,yoffset+22,50,19);
  card_port_label->setGeometry(0,yoffset+24,55,19);
}


int CardSelector::card() const
{
  return card_card_box->value();
}


void CardSelector::setCard(int card)
{
  card_card_box->setValue(card);
}


int CardSelector::port() const
{
  return card_port_box->value();
}


void CardSelector::setPort(int port)
{
  card_port_box->setValue(port);
}


void CardSelector::cardData(int card)
{
  if(card>=0) {
    card_port_box->setEnabled(true);
  }
  else {
    card_port_box->setDisabled(true);
  }
  emit cardChanged(card);
}


void CardSelector::portData(int port)
{
  emit portChanged(port);
}

