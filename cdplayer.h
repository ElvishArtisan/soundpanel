// cdplayer.h
//
// Abstract a Linux CDROM Device.
//
//   (C) Copyright 2002-2003,2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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

#ifndef CDPLAYER_H
#define CDPLAYER_H

#include <QtGui>

#include <linux/cdrom.h>

#include "cddbrecord.h"

//
// Driver Settings
//
#define CDPLAYER_CLOCK_INTERVAL 100
#define CDPLAYER_BUTTON_DELAY 10

class CdPlayer : public QObject
{
  Q_OBJECT
  public:
   enum Status {NoStatusInfo=CDS_NO_INFO,NoDriveDisc=CDS_NO_DISC,
		    TrayOpen=CDS_TRAY_OPEN,NotReady=CDS_DRIVE_NOT_READY,
		    Ok=CDS_DISC_OK};
   enum Medium {NoMediumInfo=CDS_NO_INFO,NoMediumLoaded=CDS_NO_DISC,
		AudioDisc=CDS_AUDIO,Data1=CDS_DATA_1,Data2=CDS_DATA_2,
		Xa21=CDS_XA_2_1,Xa22=CDS_XA_2_2,Mixed=CDS_MIXED};
   enum State {NoStateInfo=0,Stopped=1,Playing=2,Paused=3};
   enum PlayMode {Single=0,Continuous=1};
   CdPlayer(QWidget *parent=0,const char *name=0);
   ~CdPlayer();
   QString device() const;
   void setDevice(QString device);
   bool open();
   void close();
   CdPlayer::Status status();
   CdPlayer::Medium medium();
   int tracks() const;
   bool isAudio(int track) const;
   int trackLength(int track) const;
   unsigned trackOffset(int track) const;
   CdPlayer::State state() const;
   int leftVolume();
   int rightVolume();
   CdPlayer::PlayMode playMode() const;
   void setPlayMode(CdPlayer::PlayMode mode);
   void setCddbRecord(CddbRecord *);

  public slots:
   void eject();
   void play(int track);
   void pause();
   void stop();
   void setLeftVolume(int vol);
   void setRightVolume(int vol);

  signals:
   void ejected();
   void mediaChanged();
   void played(int track);
   void paused();
   void stopped();
   void leftVolumeChanged(int vol);
   void rightVolumeChanged(int vol);

  private slots:
   void buttonTimerData();
   void clockData();

  private:
   enum ButtonOp {Play=0,Pause=1,Resume=2,Stop=3,Eject=4};
   void ReadToc();
   unsigned GetCddbSum(int);
   unsigned GetCddbDiscId();
   QString cdrom_device;
   int cdrom_fd;
   QTimer *cdrom_clock;
   QTimer *cdrom_button_timer;
   CdPlayer::ButtonOp cdrom_pending_op;
   int cdrom_pending_track;
   CdPlayer::State cdrom_state;
   int cdrom_track;
   int cdrom_track_count;
   union cdrom_addr *cdrom_track_start;
   bool *cdrom_audio_track;
   CdPlayer::PlayMode cdrom_play_mode;
   union cdrom_addr cdrom_rip_ptr;
   union cdrom_addr cdrom_rip_end;
   bool cdrom_old_state;
   int cdrom_audiostatus;
   unsigned cdrom_disc_id;
};


#endif  // CDPLAYER_H
