//   recordstream.h
//
//   A class for recording Microsoft WAV files.
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
//

#ifndef RECORDSTREAM_H
#define RECORDSTREAM_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <QTimer>
#include <QWidget>

#include "wavefile.h"
#include "soundcard.h"

#define DEBUG_VAR "_RECORDSTREAM"
#define XRUN_VAR "_RSOUND_XRUN"

#include <asihpi/hpi.h>
#define AUDIO_SIZE 32768
#define RECORDSTREAM_CLOCK_INTERVAL 100

class RecordStream : public QObject,public WaveFile
{
  Q_OBJECT

 public:
  enum RecordState {Recording=0,RecordReady=1,Paused=2,Stopped=3,
		    RecordStarted=4};
  enum Error {Ok=0,NoFile=1,NoStream=2,AlreadyOpen=3};
  RecordStream(SoundCard *card,QWidget *parent=0,const char *name=0);
  ~RecordStream();
  QString errorString(RecordStream::Error err);
  RecordStream::Error createWave();
  void closeWave();
  RecordStream::Error createWave(QString filename);
  bool formatSupported(WaveFile::Format format);
  bool formatSupported();
  int getCard() const;
  int getStream() const;
  bool haveInputVOX() const;
  RecordStream::RecordState getState();
  int getPosition() const;
  
 signals:
  void isStopped(bool state);
  void ready();
  void recording();
  void recordStart();
  void paused();
  void stopped();
  void position(int samples);
  void stateChanged(int card,int stream,int state);
  
 public slots:
  void setCard(int card);
  void setStream(int stream);
  bool recordReady();
  void record();
  void pause();
  void stop();
  void setInputVOX(int gain);
  void setRecordLength(int length);

 private slots:
  void tickClock();

 private:
  bool GetStream();
  void FreeStream();
  bool HPICall(uint16_t hpi_err) const;
  SoundCard *sound_card;
  bool debug;
  bool xrun;
  QTimer *clock;
  int card_number;
  int stream_number;
  bool is_recording;
  bool is_ready;
  bool is_paused;
  bool stopping;
  bool record_started;
  QTimer *length_timer;
  int record_length;
  unsigned audio_ptr;
  unsigned char abuf[AUDIO_SIZE];
  unsigned left_to_write;
  hpi_handle_t hpi_stream;
  uint16_t state;
  uint32_t buffer_size;
  uint32_t data_recorded;
  uint32_t samples_recorded;
  uint32_t reserved;
  uint32_t fragment_size;
  int fragment_time;
  uint8_t *pdata;
#if HPI_VER < 0x030a00
  HPI_FORMAT format;
#else
  struct hpi_format format;
#endif
#if HPI_VER < 0x00030500
  HPI_DATA hpi_data;
#endif
  bool is_open;
};


#endif  // RECORDSTREAM_H
