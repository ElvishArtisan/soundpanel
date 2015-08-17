// ripper_dialog.cpp
//
// CD Ripper Dialog for SoundPanel.
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
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <linux/cdrom.h>

#include <conf.h>
#include <audiosettingsdialog.h>

#include <ripper_dialog.h>


RipperDialog::RipperDialog(QString *legend,QString *wavefile,
			   QString *path,QString ripdev,int paranoia_level,
			   QString cddb_serv,bool *apply_cddb_title,
			   bool *normalize,int normalize_level,
			   AudioSettings *settings,
			   QWidget *parent,const char *name) 
  : QDialog(parent,name)
{
  rip_legend=legend;
  rip_wavefile=wavefile;
  rip_path=path;
  rip_ripdev=ripdev;
  rip_paranoia_level=paranoia_level;
  rip_cddb_server=cddb_serv;
  rip_apply_cddb_title=apply_cddb_title;
  rip_complete=false;
  rip_normalize=normalize;
  rip_settings=*settings;

  setCaption("Rip CD");

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
  QFont track_font=QFont("Helvetica",12,QFont::Bold);
  track_font.setPixelSize(12);
  QFont button_font=QFont("Helvetica",12,QFont::Bold);
  button_font.setPixelSize(12);

  //
  // The CDROM Drive
  //
  rip_cdrom=new CdPlayer(this,"rip_cdrom");
  connect(rip_cdrom,SIGNAL(ejected()),this,SLOT(ejectedData()));
  connect(rip_cdrom,SIGNAL(mediaChanged()),this,SLOT(mediaChangedData()));
  connect(rip_cdrom,SIGNAL(played(int)),this,SLOT(playedData(int)));
  connect(rip_cdrom,SIGNAL(stopped()),this,SLOT(stoppedData()));
  rip_cdrom->setDevice(rip_ripdev);
  rip_cdrom->open();

  //
  // CDDB Stuff
  //
  rip_cddb_lookup=new CddbLookup(this,"rip_cddb_lookup");
  connect(rip_cddb_lookup,SIGNAL(done(CddbLookup::Result)),
	  this,SLOT(cddbDoneData(CddbLookup::Result)));

  //
  // Legend
  //
  rip_legend_edit=new QLineEdit(this,"rip_legend_edit");
  rip_legend_edit->setGeometry(90,10,sizeHint().width()-100,20);
  rip_legend_edit->setFont(font);
  rip_legend_edit->setText(QString(*rip_legend));
  QLabel *label=new QLabel(rip_legend_edit,"&Legend:",
			   this,"rip_legend_label");
  label->setGeometry(10,10,75,20);
  label->setFont(track_font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // Wavefile
  //
  rip_wavefile_edit=new QLineEdit(this,"rip_wavefile_edit");
  rip_wavefile_edit->setGeometry(90,34,290,20);
  rip_wavefile_edit->setFont(font);
  rip_wavefile_edit->setText(*rip_wavefile);
  label=new QLabel(rip_wavefile_edit,"&Wave File:",
			   this,"rip_wavefile_label");
  label->setGeometry(10,34,75,20);
  label->setFont(track_font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // CDDB Album
  //
  rip_album_edit=new QLineEdit(this,"rip_album_edit");
  rip_album_edit->setGeometry(90,58,sizeHint().width()-100,20);
  rip_album_edit->setFont(font);
  rip_album_edit->setReadOnly(true);
  label=new QLabel(rip_album_edit,"Album:",
			   this,"rip_album_label");
  label->setGeometry(10,58,75,20);
  label->setFont(track_font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // CDDB Artist
  //
  rip_artist_edit=new QLineEdit(this,"rip_artist_edit");
  rip_artist_edit->setGeometry(90,82,sizeHint().width()-100,20);
  rip_artist_edit->setFont(font);
  rip_artist_edit->setReadOnly(true);
  label=new QLabel(rip_artist_edit,"Artist:",
			   this,"rip_artist_label");
  label->setGeometry(10,82,75,20);
  label->setFont(track_font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // CDDB Other Field
  //
  rip_other_edit=new QLineEdit(this,"rip_other_edit");
  rip_other_edit->setGeometry(90,106,sizeHint().width()-100,20);
  rip_other_edit->setFont(font);
  rip_other_edit->setReadOnly(true);
  label=new QLabel(rip_other_edit,"Other:",
			   this,"rip_other_label");
  label->setGeometry(10,106,75,20);
  label->setFont(track_font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // Apply CDDB Title Checkbox
  //
  rip_apply_box=new QCheckBox(this,"rip_apply_box");
  rip_apply_box->setGeometry(90,130,15,15);
  rip_apply_box->setFont(font);
  rip_apply_box->setDisabled(true);
  rip_apply_label=new QLabel(rip_apply_box,"Apply Track Title to Legend",
			   this,"rip_apply_box");
  rip_apply_label->setGeometry(112,128,170,20);
  rip_apply_label->setFont(track_font);
  rip_apply_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  rip_apply_label->setDisabled(true);
  if(*apply_cddb_title) {
    rip_apply_box->setChecked(true);
  }

  //
  // Browse Button
  //
  rip_browse_button=new QPushButton("&Browse",this,"browse_button");
  rip_browse_button->setGeometry(395,30,60,28);
  rip_browse_button->setFont(font);
  connect(rip_browse_button,SIGNAL(clicked()),this,SLOT(browseData()));

  //
  // Track List
  //
  rip_track_list=new QTreeWidget(this);
  rip_track_list->setGeometry(10,172,sizeHint().width()-110,
			      sizeHint().height()-236);
  rip_track_list->setFont(font);
  rip_track_list->setAllColumnsShowFocus(true);
  label=new QLabel(rip_track_list,"Tracks",this,"name_label");
  label->setGeometry(15,156,100,14);
  label->setFont(button_font);
  rip_track_list->setColumnCount(5);
  QStringList hdrs;
  hdrs.push_back(tr("Track"));
  hdrs.push_back(tr("Length"));
  hdrs.push_back(tr("Title"));
  hdrs.push_back(tr("Other"));
  hdrs.push_back(tr("Type"));
  rip_track_list->setHeaderLabels(hdrs);
  for(int i=0;i<5;i++) {
    rip_track_list->resizeColumnToContents(i);
  }

  //
  // Progress Bar
  //
  rip_bar=new QProgressBar(this);
  rip_bar->setGeometry(10,sizeHint().height()-54,sizeHint().width()-110,20);

  //
  // Progress Bar Timer
  //
  rip_bar_timer=new QTimer(this,"rip_bar_timer");
  connect(rip_bar_timer,SIGNAL(timeout()),this,SLOT(barTimerData()));

  //
  // Normalize Check Box
  //
  rip_normal_checkbox=new QCheckBox(this,"normalize_check_box");
  rip_normal_checkbox->setGeometry(10,sizeHint().height()-23,15,15);
  label=new QLabel(rip_normal_checkbox,"Normalize",this,"normalize_check_label");
  label->setGeometry(30,sizeHint().height()-27,100,24);
  label->setFont(track_font);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  rip_normal_checkbox->setChecked(*normalize);

  //
  // Normalize Level
  //
  rip_normal_box=new QSpinBox(this,"rip_normal_box");
  rip_normal_box->setGeometry(245,sizeHint().height()-27,40,24);
  rip_normal_box->setRange(-30,0);
  rip_normal_box->setValue(normalize_level/100);
  connect(rip_normal_checkbox,SIGNAL(toggled(bool)),
	  rip_normal_box,SLOT(setEnabled(bool)));
  rip_normal_box->setEnabled(*normalize);
  rip_normal_label=new QLabel(rip_normal_box,"Normalize Level:",
			      this,"rip_normal_level");
  rip_normal_label->setGeometry(115,sizeHint().height()-27,125,24);
  rip_normal_label->setFont(track_font);
  rip_normal_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  rip_normal_label->setEnabled(*normalize);
  rip_normal_unit=new QLabel(rip_normal_box,"dbFS",
			      this,"rip_normal_level");
  connect(rip_normal_checkbox,SIGNAL(toggled(bool)),
	  rip_normal_label,SLOT(setEnabled(bool)));
  rip_normal_unit->setGeometry(290,sizeHint().height()-27,60,24);
  rip_normal_unit->setFont(font);
  rip_normal_unit->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  connect(rip_normal_checkbox,SIGNAL(toggled(bool)),
	  rip_normal_unit,SLOT(setEnabled(bool)));
  rip_normal_unit->setEnabled(*normalize);

  //
  // Eject Button
  //
  rip_eject_button=new TransportButton(TransportButton::Eject,
					this,"close_button");
  rip_eject_button->setGeometry(sizeHint().width()-90,148,80,50);
  connect(rip_eject_button,SIGNAL(clicked()),this,SLOT(ejectButtonData()));
  
  //
  // Play Button
  //
  rip_play_button=new TransportButton(TransportButton::Play,
					this,"close_button");
  rip_play_button->setGeometry(sizeHint().width()-90,208,80,50);
  connect(rip_play_button,SIGNAL(clicked()),this,SLOT(playButtonData()));
  
  //
  // Stop Button
  //
  rip_stop_button=new TransportButton(TransportButton::Stop,
					this,"close_button");
  rip_stop_button->setGeometry(sizeHint().width()-90,268,80,50);
  rip_stop_button->on();
  connect(rip_stop_button,SIGNAL(clicked()),this,SLOT(stopButtonData()));
  
  //
  // Rip Track Button
  //
  rip_rip_button=new QPushButton("&Rip\nTrack",this,"rip_track_button");
  rip_rip_button->setGeometry(sizeHint().width()-90,342,80,50);
  rip_rip_button->setFont(button_font);
  connect(rip_rip_button,SIGNAL(clicked()),this,SLOT(ripTrackButtonData()));

  //
  // Settings Button
  //
  rip_settings_button=new QPushButton("Audio\n&Settings",
				      this,"rip_settings_button");
  rip_settings_button->setGeometry(sizeHint().width()-90,422,80,50);
  rip_settings_button->setFont(button_font);
  connect(rip_settings_button,SIGNAL(clicked()),
	  this,SLOT(settingsButtonData()));

  //
  // Close Button
  //
  rip_close_button=new QPushButton("&Close",this,"close_button");
  rip_close_button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  rip_close_button->setFont(button_font);
  rip_close_button->setDefault(true);
  connect(rip_close_button,SIGNAL(clicked()),this,SLOT(closeData()));

  //
  // Populate Data
  //
  rip_legend_edit->setText(*rip_legend);
  rip_wavefile_edit->setText(*rip_wavefile);
}


RipperDialog::~RipperDialog()
{
  rip_cdrom->close();
  delete rip_cdrom;
  delete rip_track_list;
  delete rip_browse_button;
  delete rip_rip_button;
  delete rip_close_button;
  delete rip_eject_button;
  delete rip_play_button;
  delete rip_stop_button;
  delete rip_legend_edit;
  delete rip_wavefile_edit;
  delete rip_bar;
  delete rip_bar_timer;
}


QSize RipperDialog::sizeHint() const
{
  return QSize(470,572);
}


QSizePolicy RipperDialog::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void RipperDialog::browseData()
{
  QString filename;

  if(rip_wavefile_edit->text().isEmpty()) {
    filename=QFileDialog::getSaveFileName(*rip_path,
					  "WAV Files (*.wav)",this);
  }
  else {
    filename=QFileDialog::getOpenFileName(rip_wavefile_edit->text(),
					  "WAV Files (*.wav)",this);
  }
  if(!filename.isEmpty()) {
    if(filename.right(4).lower()!=QString(".wav")) {
      filename+=QString(".wav");
    }
    rip_wavefile_edit->setText(filename);
    *rip_path=GetPathPart(rip_wavefile_edit->text());
  }
}


void RipperDialog::ejectButtonData()
{
  rip_cdrom->eject();
}


void RipperDialog::playButtonData()
{
  if(rip_track_list->currentItem()!=NULL) {
    rip_cdrom->play(rip_track_list->currentItem()->text(0).toInt());
    rip_play_button->on();
    rip_stop_button->off();
  }
}


void RipperDialog::stopButtonData()
{
  rip_cdrom->stop();
  rip_play_button->off();
  rip_stop_button->on();
}


void RipperDialog::ripTrackButtonData()
{
  QString cmd;
  QString paranoia_flag;

  if(!ripper_running) {
    rip_rip_aborted=false;
    if(rip_wavefile_edit->text().isEmpty()) {
      browseData();
      if(rip_wavefile_edit->text().isEmpty()) {
	return;
      }
    }
    bar_file=new QFile(rip_wavefile_edit->text());
    if(bar_file->exists()) {
      if(!QMessageBox::warning(this,"File Exists",
		  "This file already exists.\nDo you want to overwrite it?",
		  "OK","Cancel",0,0,1)==0) {
	delete bar_file;
	return;
      }
    }
    if(!bar_file->open(QIODevice::WriteOnly)) {
      QMessageBox::warning(this,"No Access",
			   "Unable to open the file for writing!",
			   "OK");
      delete bar_file;
      return;
    }
    bar_file->close();
    rip_complete=false;
    rip_browse_button->setDisabled(true);
    rip_eject_button->setDisabled(true);
    rip_play_button->setDisabled(true);
    rip_stop_button->setDisabled(true);
    rip_settings_button->setDisabled(true);
    rip_rip_button->setDisabled(true);
    rip_close_button->setDisabled(true);
    rip_finished_length=
      (int)((double)rip_cdrom->
	    trackLength(rip_track_list->currentItem()->text(0).toInt())*176.4);
    switch(rip_paranoia_level) {
	case 1:
	  paranoia_flag=" -Y";
	  break;
	case 2:
	  paranoia_flag=" -Z";
	  break;
    }
    rip_wavefile_name=rip_wavefile_edit->text();
    if(rip_normal_checkbox->isChecked()) {
      rip_working_wav=
	QString().sprintf("%s.wav",
       tempnam((const char *)GetBasePart(rip_wavefile_edit->text()),"sndpl"));
      rip_working_level=
	QString().sprintf("%s.lvl",
       tempnam((const char *)GetBasePart(rip_wavefile_edit->text()),"sndpl"));

      cmd=QString().
	sprintf("cdparanoia -q%s -d %s %d - | sox -t wav - %s stat -v 2> %s",
		(const char *)paranoia_flag,
		(const char *)rip_ripdev,
		rip_track_list->currentItem()->text(0).toInt(),
		(const char *)rip_working_wav,
		(const char *)rip_working_level);
      bar_file->setName(rip_working_wav);
      rip_stage=1;
    }
    else {
      cmd=QString().
	sprintf("cdparanoia -q%s -d %s %d - | sox -t wav - -r %d -c %d %s",
		(const char *)paranoia_flag,
		(const char *)rip_ripdev,
		rip_track_list->currentItem()->text(0).toInt(),
		rip_settings.sampleRate(),
		rip_settings.channels(),
		(const char *)rip_wavefile_name);
      if(rip_settings.sampleRate()!=44100) {
	cmd+=" resample -q";
      }
      rip_stage=0;
      rip_finished_length=(int)(((double)rip_finished_length/2.0)*
				((double)rip_settings.channels())*
				((double)rip_settings.sampleRate()/44100.0));
    }
    ripper_running=true;
    if((rip_ripper_pid=fork())==0) {
      if(system(cmd)!=0) {
	bar_file->remove();
      }
      exit(0);
    }
//    rip_bar->setProgress(0);
//    rip_bar->setPercentageVisible(true);
    rip_bar_timer->start(RIPPER_BAR_INTERVAL,true);
  }
  else {
    rip_rip_aborted=true;
    bar_file->remove();
    ::kill(rip_ripper_pid,SIGTERM);
  }
}


void RipperDialog::settingsButtonData()
{
  AudioSettingsDialog *settings=
    new AudioSettingsDialog(&rip_settings,false,this,"settings");
  settings->exec();
  delete settings;
}


void RipperDialog::ejectedData()
{
  rip_track_list->clear();
}


void RipperDialog::mediaChangedData()
{
  QTreeWidgetItem *l;
  QString cdda_dir=tempnam("/tmp","cdda");

  mkdir(cdda_dir,0700);
  rip_track_list->clear();
  for(int i=rip_cdrom->tracks();i>0;i--) {
    l=new QTreeWidgetItem(rip_track_list);
    l->setText(0,QString().sprintf("%d",i));
    l->setText(1,GetTimeLength(rip_cdrom->trackLength(i)));
    if(rip_cdrom->isAudio(i)) {
      l->setText(4,"Audio Track");
    }
    else {
      l->setText(4,"Data Track");
    }
  }
  rip_track_list->resizeColumnToContents(0);
  rip_track_list->resizeColumnToContents(1);
  rip_track_list->resizeColumnToContents(4);
  rip_cddb_record.clear();
  rip_cdrom->setCddbRecord(&rip_cddb_record);
  rip_cddb_lookup->setCddbRecord(&rip_cddb_record);
  rip_cddb_lookup->lookupRecord(cdda_dir,rip_ripdev,rip_cddb_server,8880,
				RIPPER_CDDB_USER,PACKAGE_NAME,VERSION);
  system(QString().sprintf("rm %s/*",(const char *)cdda_dir));
  system(QString().sprintf("rmdir %s",(const char *)cdda_dir));
}


void RipperDialog::playedData(int track)
{
  rip_play_button->on();
  rip_stop_button->off();
}


void RipperDialog::stoppedData()
{
  rip_play_button->off();
  rip_stop_button->on();
}


void RipperDialog::barTimerData()
{
  QString cmd;
  double ratio=0.0;
  FILE *fh;

  if(ripper_running) {
    switch(rip_stage) {
	case 0:
	  rip_bar->setValue((int)(100.0*(double)bar_file->size()/
				  (double)rip_finished_length));
	  rip_bar_timer->start(RIPPER_BAR_INTERVAL,true);
	  break;

	case 1:
	  rip_bar->setValue((int)(50.0*(double)bar_file->size()/
				  (double)rip_finished_length));
	  rip_bar_timer->start(RIPPER_BAR_INTERVAL,true);
	  break;

	case 2:
	  rip_bar->setValue(50+(int)(50.0*(double)bar_file->size()/
				     (double)rip_finished_length));
	  rip_bar_timer->start(RIPPER_BAR_INTERVAL,true);
	  break;
    }
  }
  else {
    if(!bar_file->exists()) {
      if(rip_rip_aborted) {
	QMessageBox::information(this,"Rip Aborted",
				 "The rip has been aborted.");
      }
      else {
	QMessageBox::warning(this,"Rip Failed",
			     "The ripper encountered an error.\n\
Please check your ripper configuration and try again.");
      }
      delete bar_file;
      bar_file=NULL;
      rip_stage=0;
      rip_bar->setTextVisible(false);
      rip_bar->reset();
      rip_browse_button->setEnabled(true);
      rip_eject_button->setEnabled(true);
      rip_play_button->setEnabled(true);
      rip_stop_button->setEnabled(true);
      rip_settings_button->setEnabled(true);
      rip_rip_button->setEnabled(true);
      rip_close_button->setEnabled(true);
      return;
    }
    switch(rip_stage) {
	case 1:
	  if((fh=fopen((const char *)rip_working_level,"r"))==NULL) {
	    bar_file->setName(rip_working_wav);
	    bar_file->remove();
	    bar_file->setName(rip_working_level);
	    bar_file->remove();
	    delete bar_file;
	    bar_file=NULL;
	    rip_stage=0;
	    return;
	  }
	  fscanf(fh,"%lf",&ratio);
	  fclose(fh);
	  rip_finished_length=(int)(((double)rip_finished_length/2.0)*
	    ((double)rip_settings.channels())*
	    ((double)rip_settings.sampleRate()/44100.0));
	  cmd=QString().sprintf("sox -v %6.4lf %s -c %d -r %d %s",
			pow(10.0,(double)rip_normal_box->value()/20.0)/ratio,
				(const char *)rip_working_wav,	
				rip_settings.channels(),
				rip_settings.sampleRate(),
				(const char *)rip_wavefile_name);
	  if(rip_settings.sampleRate()!=44100) {
	    cmd+=" resample -q";
	  }
	  rip_stage=2;
	  bar_file->setName(rip_wavefile_name);
	  ripper_running=true;
	  rip_bar_timer->start(RIPPER_BAR_INTERVAL,true);
	  if((rip_ripper_pid=fork())==0) {
	    if(system(cmd)!=0) {
	      bar_file->remove();
	    }
	    exit(0);
	  }
	  break;

	case 2:
	  bar_file->setName(rip_working_wav);
	  bar_file->remove();
	  bar_file->setName(rip_working_level);
	  bar_file->remove();
	case 0:
	  rip_bar->setTextVisible(false);
	  rip_bar->reset();
	  rip_browse_button->setEnabled(true);
	  rip_eject_button->setEnabled(true);
	  rip_play_button->setEnabled(true);
	  rip_stop_button->setEnabled(true);
	  rip_settings_button->setEnabled(true);
	  rip_rip_button->setEnabled(true);
	  rip_close_button->setEnabled(true);
	  QMessageBox::information(this,"Rip Complete",
				   "Rip complete!");
	  rip_complete=true;
	  delete bar_file;
	  bar_file=NULL;
	  rip_stage=0;
    }
  }
}


void RipperDialog::cddbDoneData(CddbLookup::Result result)
{
  QList<QTreeWidgetItem *> items;
  switch(result) {
      case CddbLookup::ExactMatch:
	if(rip_cdrom->status()!=CdPlayer::Ok) {
	  return;
	}
	rip_artist_edit->setText(rip_cddb_record.discArtist());
	rip_album_edit->setText(rip_cddb_record.discAlbum());
	rip_other_edit->setText(rip_cddb_record.discExtended());

	for(int i=0;i<rip_cddb_record.tracks();i++) {
	  items=rip_track_list->
	    findItems(QString().sprintf("%d",i+1),Qt::MatchExactly);
	  items[0]->setText(2,rip_cddb_record.trackTitle(i).stripWhiteSpace());
	  items[0]->
	    setText(3,rip_cddb_record.trackExtended(i).stripWhiteSpace());
	}
	rip_track_list->resizeColumnToContents(2);
	rip_track_list->resizeColumnToContents(3);
	rip_apply_box->setEnabled(true);
	rip_apply_label->setEnabled(true);
	break;
      case CddbLookup::PartialMatch:
	printf("Partial Match!\n");
	break;
      default:
	printf("No Match!\n");
	break;
  }
}


void RipperDialog::closeData()
{
  *rip_normalize=rip_normal_checkbox->isChecked();
  if(rip_complete) {
    if(rip_apply_box->isChecked()) {
      *rip_legend=rip_track_list->currentItem()->text(2);
    }
    else {
      *rip_legend=rip_legend_edit->text();
    }
    *rip_wavefile=rip_wavefile_edit->text();
    *rip_apply_cddb_title=rip_apply_box->isChecked();
    done(0);
  }
  else {
    done(1);
  }
}


void RipperDialog::closeEvent(QCloseEvent *e)
{
  if(!ripper_running) {
    closeData();
  }
}
