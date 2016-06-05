// record_audio.cpp
//
// Record Audio to a SoundPanel button.
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

#include "audiosettingsdialog.h"
#include "conf.h"
#include "globals.h"
#include "record_audio.h"

RecordAudio::RecordAudio(QString *legend,QString *wavefile,
			 QString *path,QString play_dev,QString rec_dev,
			 int playcard,
			 SoundCard::SourceNode type,int reccard,
			 int recport,int rec_level,AudioSettings *settings,
			 SoundCard *card,
			 QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  rec_legend=legend;
  rec_wavefile=wavefile;
  rec_path=path;
  rec_settings=*settings;
  rec_play_card=playcard;
  rec_rec_card=reccard;
  rec_rec_port=recport;
  rec_input_type=type;
  rec_soundcard=card;

  setCaption("Record Audio");

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
  QFont timer_font=QFont("Helvetica",18,QFont::Bold);
  timer_font.setPixelSize(18);

  //
  // Play Device
  //
  rec_play=new PlayStream(card,this,"rec_play");
  rec_play->setCard(rec_play_card);
  connect(rec_play,SIGNAL(played()),this,SLOT(playedData()));
  connect(rec_play,SIGNAL(stopped()),this,SLOT(playStoppedData()));
  connect(rec_play,SIGNAL(position(int)),this,SLOT(positionData(int)));

  //
  // Record Device
  //
  rec_record=new RecordStream(card,this,"rec_record");
  rec_record->setCard(rec_rec_card);
  rec_record->setStream(rec_rec_port);
  rec_soundcard->setInputLevel(rec_rec_card,rec_rec_port,
			       rec_level+1600);
  connect(rec_soundcard,SIGNAL(inputPortError(int,int)),
	  this,SLOT(inputPortErrorData(int,int)));
  connect(rec_record,SIGNAL(ready()),this,SLOT(recordReadyData()));
  connect(rec_record,SIGNAL(recording()),this,SLOT(recordedData()));
  connect(rec_record,SIGNAL(stopped()),this,SLOT(recordStoppedData()));
  connect(rec_record,SIGNAL(position(int)),this,SLOT(positionData(int)));

  //
  // Build File Filter
  //
  rec_file_filter="Audio Files (*.wav";
  if(rec_record->formatSupported(WaveFile::Vorbis)) {
    rec_file_filter+=" *.ogg";
  }
  rec_file_filter+=")";

  //
  // Legend
  //
  rec_legend_edit=new QLineEdit(this,"rec_legend_edit");
  rec_legend_edit->setGeometry(80,10,sizeHint().width()-90,20);
  rec_legend_edit->setFont(font);
  rec_legend_edit->setText(QString(*rec_legend));
  QLabel *label=new QLabel(rec_legend_edit,"&Legend:",
			   this,"rec_legend_label");
  label->setGeometry(10,12,65,20);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight);

  //
  // Wavefile
  //
  rec_wavefile_edit=new QLineEdit(this,"rec_wavefile_edit");
  rec_wavefile_edit->setGeometry(80,34,300,20);
  rec_wavefile_edit->setFont(font);
  rec_wavefile_edit->setText(*rec_wavefile);
  label=new QLabel(rec_wavefile_edit,"&Wave File:",
			   this,"rec_wavefile_label");
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
  // Settings Button
  //
  button=new QPushButton(this,"settings_button");
  button->setGeometry(10,sizeHint().height()-125,80,50);
  button->setText("Audio\nSettings");
  button->setFont(font);
  connect(button,SIGNAL(clicked()),this,SLOT(settingsData()));

  //
  // Time Counter
  //
  rec_time_label=new QLabel("00:00:00",this,"rec_time_label");
  rec_time_label->setGeometry(90,sizeHint().height()-110,100,20);
  rec_time_label->setFont(timer_font);
  rec_time_label->setAlignment(Qt::AlignCenter);

  //
  // Record Button
  //
  rec_record_button=new TransportButton(TransportButton::Record,
					 this,"settings_button");
  rec_record_button->setGeometry(190,sizeHint().height()-125,80,50);
  rec_record_button->off();
  connect(rec_record_button,SIGNAL(clicked()),this,SLOT(recordData()));

  //
  // Play Button
  //
  rec_play_button=new TransportButton(TransportButton::Play,
					 this,"settings_button");
  rec_play_button->setGeometry(280,sizeHint().height()-125,80,50);
  rec_play_button->setFont(QFont("Helvetica",14,QFont::Bold));
  rec_play_button->off();
  connect(rec_play_button,SIGNAL(clicked()),this,SLOT(playData()));

  //
  // Stop Button
  //
  rec_stop_button=new TransportButton(TransportButton::Stop,
					 this,"settings_button");
  rec_stop_button->setGeometry(370,sizeHint().height()-125,80,50);
  rec_stop_button->setFont(QFont("Helvetica",14,QFont::Bold));
  rec_stop_button->on();
  connect(rec_stop_button,SIGNAL(clicked()),this,SLOT(stopData()));

  //
  // Audio Meter
  //
  rec_meter=new StereoMeter(this,"meter");
  rec_meter->
    setGeometry(65,65,rec_meter->sizeHint().width(),
		rec_meter->sizeHint().height());
  rec_meter->setReference(0);
  rec_meter->setMode(SegMeter::Peak);
  rec_meter_timer=new QTimer(this,"rec_meter_timer");
  connect(rec_meter_timer,SIGNAL(timeout()),this,SLOT(recordMeterData()));
  play_meter_timer=new QTimer(this,"play_meter_timer");
  connect(play_meter_timer,SIGNAL(timeout()),this,SLOT(playMeterData()));

  //
  // Sync Error Indicator
  //
  rec_sync_label=new QLabel("No Input Sync!",this,"rec_sync_label");
  rec_sync_label->setGeometry(40,sizeHint().height()-50,200,30);
  rec_sync_label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
  rec_sync_label->setFont(QFont("Helvetica",20,QFont::Bold));
  rec_sync_label->setPalette(QColor(Qt::red));
  rec_sync_label->hide();

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
  // Populate Data
  //
  rec_legend_edit->setText(*rec_legend);
  rec_wavefile_edit->setText(*rec_wavefile);
  QFile *file=new QFile(*rec_wavefile);
  if(file->exists()) {
    WaveFile *wave=new WaveFile(*rec_wavefile);
    wave->openWave();
    rec_time_label->
      setText(GetTimeLength(wave->getExtTimeLength(),true,false));
    wave->closeWave();
    delete wave;
  }
  delete file;
  inputPortErrorData(rec_rec_card,rec_rec_port);
}


RecordAudio::~RecordAudio()
{
}


QSize RecordAudio::sizeHint() const
{
  return QSize(470,255);
}


QSizePolicy RecordAudio::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void RecordAudio::browseData()
{
  QString filename;

  if(rec_wavefile_edit->text().isEmpty()) {
    filename=QFileDialog::getSaveFileName(*rec_path,rec_file_filter,this);
  }
  else {
    filename=QFileDialog::getSaveFileName(rec_wavefile_edit->text(),
					  rec_file_filter,this);
  }
  if(!filename.isEmpty()) {
    if(filename.right(4).left(1)!=QString(".")) {
      switch(rec_settings.format()) {
	  case AudioSettings::Pcm16:
	  case AudioSettings::Layer1:
	  case AudioSettings::Layer2:
	  case AudioSettings::Layer3:
	    filename+=QString(".wav");
	    break;

	  case AudioSettings::OggVorbis:
	    filename+=QString(".ogg");
	    break;
      }
    }
    rec_wavefile_edit->setText(filename);
    *rec_path=GetPathPart(rec_wavefile_edit->text());
  }
}


void RecordAudio::settingsData()
{
  AudioSettingsDialog *settings=
    new AudioSettingsDialog(&rec_settings,false,this,"settings");
  settings->exec();
  delete settings;
}


void RecordAudio::recordData()
{
  QFile *file;
  RecordStream::Error error;

  if(rec_wavefile_edit->text().isEmpty()) {
    browseData();
    if(rec_wavefile_edit->text().isEmpty()) {
      return;
    }
  }
  if(rec_record->getState()==RecordStream::Stopped) {
    file=new QFile(rec_wavefile_edit->text());
    if(file->exists()) {
      if(!QMessageBox::warning(this,"File Exists",
		   "This file already exists.\nDo you want to overwrite it?",
		   "OK","Cancel",0,0,1)==0) {
	delete file;
	return;
      }
    }
    delete file;
    rec_record->nameWave(rec_wavefile_edit->text());
    switch(rec_settings.format()) {
	case AudioSettings::Pcm16:
	  rec_record->setFormatTag(WAVE_FORMAT_PCM);
	  rec_record->setChannels(rec_settings.channels());
	  rec_record->setSamplesPerSec(rec_settings.sampleRate());
	  rec_record->setBitsPerSample(16);
	  rec_record->setBextChunk(true);
	  break;

	case AudioSettings::OggVorbis:
	  rec_record->setFormatTag(WAVE_FORMAT_VORBIS);
	  rec_record->setChannels(rec_settings.channels());
	  rec_record->setSamplesPerSec(rec_settings.sampleRate());
	  rec_record->setBitsPerSample(16);
	  break;

	case AudioSettings::Layer1:
	  rec_record->setFormatTag(WAVE_FORMAT_MPEG);
	  rec_record->setChannels(rec_settings.channels());
	  rec_record->setSamplesPerSec(rec_settings.sampleRate());
	  rec_record->setBitsPerSample(16);
	  rec_record->setHeadLayer(1);
	  rec_record->setHeadBitRate(rec_settings.bitRate());
	  switch(rec_settings.channels()) {
	      case 1:
		rec_record->setHeadMode(ACM_MPEG_SINGLECHANNEL);
		break;
	      case 2:
		rec_record->setHeadMode(ACM_MPEG_STEREO);
	  }
	  rec_record->setHeadFlags(ACM_MPEG_ID_MPEG1);
	  rec_record->setBextChunk(true);
	  rec_record->setMextChunk(true);
	  break;

	case AudioSettings::Layer2:
	  rec_record->setFormatTag(WAVE_FORMAT_MPEG);
	  rec_record->setChannels(rec_settings.channels());
	  rec_record->setSamplesPerSec(rec_settings.sampleRate());
	  rec_record->setBitsPerSample(16);
	  rec_record->setHeadLayer(2);
	  rec_record->setHeadBitRate(rec_settings.bitRate());
	  switch(rec_settings.channels()) {
	      case 1:
		rec_record->setHeadMode(ACM_MPEG_SINGLECHANNEL);
		break;
	      case 2:
		rec_record->setHeadMode(ACM_MPEG_STEREO);
	  }
	  rec_record->setHeadFlags(ACM_MPEG_ID_MPEG1);
	  rec_record->setBextChunk(true);
	  rec_record->setMextChunk(true);
	  break;

	case AudioSettings::Layer3:
	  rec_record->setFormatTag(WAVE_FORMAT_MPEG);
	  rec_record->setChannels(rec_settings.channels());
	  rec_record->setSamplesPerSec(rec_settings.sampleRate());
	  rec_record->setBitsPerSample(16);
	  rec_record->setHeadLayer(3);
	  rec_record->setHeadBitRate(rec_settings.bitRate());
	  switch(rec_settings.channels()) {
	      case 1:
		rec_record->setHeadMode(ACM_MPEG_SINGLECHANNEL);
		break;
	      case 2:
		rec_record->setHeadMode(ACM_MPEG_STEREO);
	  }
	  rec_record->setHeadFlags(ACM_MPEG_ID_MPEG1);
	  rec_record->setBextChunk(true);
	  rec_record->setMextChunk(true);
	  break;
    }
    if((error=rec_record->createWave())!=0) {
      QMessageBox::warning(this,"SoundPanel",rec_record->errorString(error));
      return;
    }
    if(!rec_record->recordReady()) {
      QMessageBox::warning(this,"SoundPanel","Audio Device Error");
    }
    return;
  }
}


void RecordAudio::playData()
{
  if((rec_play->getState()==PlayStream::Stopped)&&
     (rec_record->getState()==RecordStream::Stopped)) {
    rec_play->openWave(rec_wavefile_edit->text());
    if(!rec_play->play()) {
      rec_play->closeWave();
    }
    return;
  }
  if(rec_record->getState()==RecordStream::RecordReady) {
    rec_record->record();
    return;
  }
  return;
}


void RecordAudio::stopData()
{
  if(rec_play->getState()==PlayStream::Playing) {
    rec_play->stop();
  }
  if((rec_record->getState()==RecordStream::RecordReady)||
     (rec_record->getState()==RecordStream::Recording)||
     (rec_record->getState()==RecordStream::RecordStarted)) {
    rec_record->stop();
  }
}


void RecordAudio::playedData()
{
  rec_play_button->on();
  rec_record_button->off();
  rec_stop_button->off();
  play_meter_timer->start(METER_UPDATE_INTERVAL);
}


void RecordAudio::playStoppedData()
{
  rec_play->closeWave();
  rec_meter->setLeftPeakBar(-10000);
  rec_meter->setRightPeakBar(-10000);
  rec_play_button->off();
  rec_record_button->off();
  rec_stop_button->on();
  rec_time_label->
    setText(GetTimeLength(rec_play->getExtTimeLength(),true,false));
  play_meter_timer->stop();
  rec_meter->setLeftPeakBar(-10000);
  rec_meter->setRightPeakBar(-10000);
}


void RecordAudio::positionData(int pos)
{
  if(rec_play->getState()==PlayStream::Playing) {
    rec_time_label->
      setText(GetTimeLength(int(1000.0*(double)pos/
				 ((double)rec_play->getSamplesPerSec())),
			     true,false));
  }
  if(rec_record->getState()==RecordStream::RecordStarted) {
    rec_time_label->
      setText(GetTimeLength(int(1000.0*(double)pos/
				 ((double)rec_record->getSamplesPerSec())),
			     true,false));
  }
}


void RecordAudio::recordedData()
{
  rec_play_button->on();
  rec_record_button->on();
  rec_stop_button->off();
}


void RecordAudio::recordReadyData()
{
  rec_play_button->off();
  rec_record_button->flash();
  rec_stop_button->off();  
  rec_time_label->setText("0:00:00");
  rec_meter_timer->start(METER_UPDATE_INTERVAL);
}
 

void RecordAudio::recordStoppedData()
{
  rec_meter->setLeftPeakBar(-10000);
  rec_meter->setRightPeakBar(-10000);
  rec_record->closeWave();
  rec_play_button->off();
  rec_record_button->off();
  rec_stop_button->on();
  rec_meter_timer->stop();
  rec_meter->setLeftPeakBar(-10000);
  rec_meter->setRightPeakBar(-10000);
}


void RecordAudio::inputPortErrorData(int card,int port)
{
  if((card!=rec_rec_card)||(port!=rec_rec_port)) {
    return;
  }
  if(rec_input_type!=SoundCard::LineIn) {
    if(rec_soundcard->getInputPortError(card,port)==0) {
      rec_sync_label->hide();
    }
    else {
      rec_sync_label->show();
    }
  }
}


void RecordAudio::playMeterData()
{
  short level[2];
  rec_soundcard->outputStreamMeter(rec_play_card,rec_play->getStream(),level);
  rec_meter->setLeftPeakBar(level[0]);
  rec_meter->setRightPeakBar(level[1]);
}


void RecordAudio::recordMeterData()
{
  short level[2];
  rec_soundcard->inputStreamMeter(rec_rec_card,rec_rec_port,level);
  rec_meter->setLeftPeakBar(level[0]);
  rec_meter->setRightPeakBar(level[1]);
}


void RecordAudio::okData()
{
  rec_record->stop();
  rec_play->stop();
  if(rec_legend_edit->text().isEmpty()) {
    *rec_legend=GetBasePart(rec_wavefile_edit->text());
  }
  else {
    *rec_legend=rec_legend_edit->text();
  }
  *rec_wavefile=rec_wavefile_edit->text();

  done(0);
}


void RecordAudio::cancelData()
{
  rec_record->stop();
  rec_play->stop();
  done(1);
}


void RecordAudio::closeEvent(QCloseEvent *e)
{
  cancelData();
}
