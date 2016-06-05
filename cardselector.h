// cardselector.h
//
// Audio Card Selector Widget
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

#ifndef CARDSELECTOR_H
#define CARDSELECTOR_H

#include <QtGui>

class CardSelector : public QWidget
{
  Q_OBJECT
  public:
   CardSelector(QWidget *parent=0,const char *name=0);
   ~CardSelector();
   QSize sizeHint() const;
   QSizePolicy sizePolicy() const;
   QString title() const;
   void setTitle(QString title);
   int card() const;
   void setCard(int card);
   int port() const;
   void setPort(int port);

  signals:
   void cardChanged(int card);
   void portChanged(int port);

  private slots:
   void cardData(int);
   void portData(int);

  private:
   QLabel *card_card_label;
   QSpinBox *card_card_box;
   QLabel *card_port_label;
   QSpinBox *card_port_box;
   QLabel *card_title;
   int yoffset;
};


#endif  // CARDSELECTOR_H
