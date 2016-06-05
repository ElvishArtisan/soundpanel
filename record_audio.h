// record_audio.h
//
// Record Audio to a SoundPanel button.
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

#ifndef RECORD_AUDIO_H
#define RECORD_AUDIO_H

#include <QLabel>
#include <QTimer>
#include <QWidget>

#include "transportbutton.h"
#include "playstream.h"
#include "recordstream.h"
#include "audiosettings.h"
#include "stereometer.h"
#include "soundcard.h"

//
// Meter Settings
//
#define METER_UPDATE_INTERVAL 50


class RecordAudio : public QDialog
{
  Q_OBJECT
  public:
  RecordAudio(QString *legend,QString *wavefile,QString *path,
	      QString rec_dev,QString play_dev,int playcard,
	      SoundCard::SourceNode type,int reccard,
	      int recport,int rec_level,AudioSettings *settings,
	      SoundCard *card,
	      QWidget *parent=0,const char *name=0);
   ~RecordAudio();
   QSize sizeHint() const;
   QSizePolicy sizePolicy() const;

  private slots:
   void browseData();
   void settingsData();
   void recordData();
   void playData();
   void stopData();
   void playedData();
   void playStoppedData();
   void positionData(int);
   void recordReadyData();
   void recordedData();
   void recordStoppedData();
   void inputPortErrorData(int,int);
   void playMeterData();
   void recordMeterData();
   void okData();
   void cancelData();

 protected:
   void closeEvent(QCloseEvent *e);

  private:
   SoundCard *rec_soundcard;
   StereoMeter *rec_meter;
   QTimer *play_meter_timer;
   QTimer *rec_meter_timer;
   QLineEdit *rec_legend_edit;
   QLineEdit *rec_wavefile_edit;
   QString *rec_legend;
   QString *rec_wavefile;
   QString *rec_path;
   PlayStream *rec_play;
   RecordStream *rec_record;
   TransportButton *rec_record_button;
   TransportButton *rec_play_button;
   TransportButton *rec_stop_button;
   AudioSettings rec_settings;
   QLabel *rec_time_label;
   QLabel *rec_sync_label;
   int rec_play_card;
   SoundCard::SourceNode rec_input_type;
   int rec_rec_card;
   int rec_rec_port;
   QString rec_file_filter;
};


#endif

