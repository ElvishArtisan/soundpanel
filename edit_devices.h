// edit_devices.h
//
// Edit SoundPanel Audio Device Configuration.
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

#ifndef EDIT_DEVICES_H
#define EDIT_DEVICES_H

#include <QtGui>

#include "cardselector.h"
#include "soundcard.h"

class EditDevices : public QDialog
{
  Q_OBJECT
  public:
  EditDevices(QString *playdev,QString *recdev,
	      int *playcard,int *playport,
	      SoundCard::ClockSource *clk_src,
	      SoundCard::SourceNode *type,int *reccard,
	      int *recport,int *inlevel,int *outlevel,
	      QWidget *parent=0,const char *name=0);
   ~EditDevices();
   QSize sizeHint() const;
   QSizePolicy sizePolicy() const;

  private slots:
   void cardChangedData(int);
   void inputTypeActivated(int);
   void okData();
   void cancelData();

 protected:
   void closeEvent(QCloseEvent *e);

  private:
   QLineEdit *snd_playdev_edit;
   QLineEdit *snd_recdev_edit;
   QString *snd_playdev;
   QString *snd_recdev;
   int *snd_playcard;
   int *snd_playport;
   int *snd_playlevel;
   int *snd_reccard;
   int *snd_recport;
   int *snd_reclevel;
   CardSelector *snd_playdev_selector;
   CardSelector *snd_recdev_selector;
   QLabel *snd_inlevel_label;
   QSpinBox *snd_inlevel_box;
   QSpinBox *snd_outlevel_box;
   QLabel *snd_type_label;
   QComboBox *snd_type_box;
   QLabel *snd_clock_label;
   QComboBox *snd_clock_box;
   SoundCard::SourceNode *snd_type;
   SoundCard::ClockSource *snd_clock_source;
};


#endif

