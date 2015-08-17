// ripper_dialog.h
//
// Information Screen for SoundPanel.
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

#ifndef RIPPER_DIALOG_H
#define RIPPER_DIALOG_H

#include <QtGui/QtGui>

#include <transportbutton.h>
#include <cddblookup.h>
#include <cdplayer.h>

#include <soundpanel.h>


class RipperDialog : public QDialog
{
  Q_OBJECT
  public:
   RipperDialog(QString *legend,QString *wavefile,
		QString *path,QString ripdev,int paranoia_level,
		QString cddb_serv,bool *apply_cddb_title,
		bool *normalize,int normalize_level,
		AudioSettings *settings,
		QWidget *parent=0,const char *name=0);
   ~RipperDialog();
   QSize sizeHint() const;
   QSizePolicy sizePolicy() const;

  private slots:
   void browseData();
   void ejectButtonData();
   void playButtonData();
   void stopButtonData();
   void ripTrackButtonData();
   void settingsButtonData();
   void ejectedData();
   void mediaChangedData();
   void playedData(int);
   void stoppedData();
   void barTimerData();
   void cddbDoneData(CddbLookup::Result result);
   void closeData();

 protected:
   void closeEvent(QCloseEvent *e);

  private:
   CdPlayer *rip_cdrom;
   CddbRecord rip_cddb_record;
   CddbLookup *rip_cddb_lookup;
   AudioSettings rip_settings;
   QLineEdit *rip_album_edit;
   QLineEdit *rip_artist_edit;
   QLineEdit *rip_other_edit;
   QCheckBox *rip_apply_box;
   QLabel *rip_apply_label;
   QString rip_cddb_server;
   QTreeWidget *rip_track_list;
   QPushButton *rip_browse_button;
   QPushButton *rip_rip_button;
   QPushButton *rip_settings_button;
   bool rip_rip_aborted;
   QPushButton *rip_close_button;
   TransportButton *rip_eject_button;
   TransportButton *rip_play_button;
   TransportButton *rip_stop_button;
   QLineEdit *rip_legend_edit;
   QString *rip_legend;
   QLineEdit *rip_wavefile_edit;
   QString *rip_wavefile;
   QString *rip_path;
   QString rip_ripdev;
   QProgressBar *rip_bar;
   QLabel *rip_normal_label;
   QLabel *rip_normal_unit;
   QCheckBox *rip_normal_checkbox;
   QSpinBox *rip_normal_box;
   QTimer *rip_bar_timer;
   QFile *bar_file;
   int rip_finished_length;
   pid_t rip_ripper_pid;
   int rip_paranoia_level;
   bool *rip_apply_cddb_title;
   bool rip_complete;
   bool *rip_normalize;
   int rip_stage;
   QString rip_wavefile_name;
   QString rip_working_wav;
   QString rip_working_level;
};


#endif

