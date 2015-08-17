// soundpanel.cpp
//
// A Cart Wall Utility
//
//   (C) Copyright 2003-2004 Fred Gleason <fredg@paravelsystems.com>
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
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include <audiosettingsdialog.h>
#include <playmeter.h>

#include <soundpanel.h>
#include <edit_button.h>
#include <snd_properties.h>
#include <edit_devices.h>
#include <record_audio.h>
#include <ripper_dialog.h>
#include <edit_ripper.h>
#include <edit_editor.h>
#include <search_array.h>
#include <panel_name.h>
#include <about.h>
#include <conf.h>

//
// Icons
//
#include "soundpanel-22x22.xpm"

//
// Global Variables
//
bool ripper_running=false;
QString array_file_filter;

//
// Prototypes
//
void SigHandler(int signo);


MainWidget::MainWidget(QWidget *parent,const char *name)
  :QMainWindow(parent,name)
{
  array_panel_quantity=BUTTON_PANELS;
  array_panel=0;
  array_focus_x=0;
  array_focus_y=0;
  array_chain_active=false;

  array_panels.
    insert(array_panels.begin(),BUTTON_PANELS,Panel());
  array_panels.back().initColor(backgroundColor());
  array_looping=false;
  array_stopping=false;

  //
  // Process Command Line Arguments
  //
  for(int i=1;i<qApp->argc();i++) {
    bool argfound=false;
    if(!strcmp(qApp->argv()[i],"-c")) {  // Configuration File
      if(i==(qApp->argc()-1)) {
	printf("soundpanel: missing argument\n");
	exit(1);
      }
      array_configname=qApp->argv()[++i];
      argfound=true;
    }
    if(!argfound) {
      array_filename=qApp->argv()[i];
    }
  }

  //
  // Load Configuration
  //
  if(array_configname.isEmpty()) {
    if(getenv("HOME")!=NULL) {
      array_configname=getenv("HOME");
    }
    else {
      array_configname="/";
    }
    array_configname=array_configname+QString("/")+
      QString(DEFAULT_CONFIG_FILE);
  }
  LoadConfig();

  //
  // Fix the Window Size
  //
  setMinimumWidth(DEFAULT_SIZE_X);
  setMinimumHeight(DEFAULT_SIZE_Y);

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont button_font=QFont("Helvetica",14,QFont::Bold);
  button_font.setPixelSize(14);
  QFont display_font=QFont("Helvetica",16,QFont::Bold);
  display_font.setPixelSize(16);

  //
  // Create And Set Icon
  //
  soundpanel_map=new QPixmap(soundpanel_xpm);
  setIcon(*soundpanel_map);

  //
  // Sound Card Mixer
  //
  array_card=new SoundCard(this,"array_card");

  //
  // Play Device
  //
  array_play=new PlayStream(array_card,this,"array_play");
  array_play->setCard(array_play_card);
  array_card->setInputPortMux(array_rec_card,array_rec_port,array_input_type);
  connect(array_play,SIGNAL(played()),this,SLOT(playedData()));
  connect(array_play,SIGNAL(paused()),this,SLOT(pausedData()));
  connect(array_play,SIGNAL(stopped()),this,SLOT(stoppedData()));
  connect(array_play,SIGNAL(position(int)),this,SLOT(positionData(int)));

  //
  // The Panel Selector
  //
  array_panel_box=new PanelBox(this,"array_panel_box");
  array_panel_box->setGeometry(10,29,200,26);
  array_panel_box->setFont(font);
  for(int i=0;i<BUTTON_PANELS;i++) {
    array_panels[i].setName(QString().sprintf("Panel %d",i+1));
    array_panel_box->insertItem(array_panels[i].name());
  }
  connect(array_panel_box,SIGNAL(activated(int)),
	  this,SLOT(panelActivatedData(int)));
  connect(array_panel_box,SIGNAL(rightPressed(QPoint)),
	  this,SLOT(rightBoxData(QPoint)));

  //
  // The Play Meter
  //
  left_meter=new PlayMeter(SegMeter::Right,this,"left_meter");
  left_meter->setGeometry(sizeHint().width()-310,32,300,10);
  left_meter->setLabel("L");
  left_meter->setRange(-4600,0);
  left_meter->setHighThreshold(-1600);
  left_meter->setClipThreshold(-1100);
  left_meter->setMode(SegMeter::Peak);
  right_meter=new PlayMeter(SegMeter::Right,this,"right_meter");
  right_meter->setGeometry(sizeHint().width()-310,42,300,10);
  right_meter->setLabel("R");
  right_meter->setRange(-4600,0);
  right_meter->setHighThreshold(-1600);
  right_meter->setClipThreshold(-1100);
  right_meter->setMode(SegMeter::Peak);
  meter_timer=new QTimer(this,"meter_timer");
  connect(meter_timer,SIGNAL(timeout()),this,SLOT(meterData()));

  //
  // Audio Buttons
  //
  QSignalMapper *mapper=new QSignalMapper(this,"mapper");
  connect(mapper,SIGNAL(mapped(int)),
	  this,SLOT(leftButtonMapperData(int)));
  QSignalMapper *focus_mapper=new QSignalMapper(this,"focus_mapper");
  connect(focus_mapper,SIGNAL(mapped(int)),this,SLOT(focusedData(int)));
  for(int i=0;i<BUTTON_ROWS;i++) {
    for(int j=0;j<BUTTON_COLUMNS;j++) {
      array_button[i][j]=new ArrayButton(this);
      array_button[i][j]->setFont(font);
      mapper->setMapping(array_button[i][j],i*BUTTON_COLUMNS+j);
      focus_mapper->setMapping(array_button[i][j],i*BUTTON_COLUMNS+j);
      connect(array_button[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
      connect(array_button[i][j],SIGNAL(focused()),focus_mapper,SLOT(map()));
    }
  }
  array_button[array_focus_x][array_focus_y]->setFocus();

  //
  // Deck Label
  //
  array_deck_label=new QLabel("<none>",this,"array_deck_label");
  array_deck_label->setGeometry(10,sizeHint().height()-51,240,26);
  array_deck_label->setBackgroundColor(QColor(DECK_LABEL_COLOR));
  array_deck_label->setAlignment(Qt::AlignCenter);
  array_deck_label->setFont(display_font);
  array_deck_label->setLineWidth(1);
  array_deck_label->setMidLineWidth(1);
  array_deck_label->setFrameStyle(QFrame::Box|QFrame::Raised);

  //
  // Time Label
  //
  array_time_label=new QLabel("-:--:--",this,"array_time_label");
  array_time_label->setGeometry(10,sizeHint().height()-26,240,26);
  array_time_label->setBackgroundColor(QColor(DECK_LABEL_COLOR));
  array_time_label->setAlignment(Qt::AlignCenter);
  array_time_label->setFont(display_font);
  array_time_label->setLineWidth(1);
  array_time_label->setMidLineWidth(1);
  array_time_label->setFrameStyle(QFrame::Box|QFrame::Raised);

  //
  // Play Button
  //
  array_play_button=new TransportButton(TransportButton::Play,
					 this,"array_play_button");
  array_play_button->setGeometry(260,sizeHint().height()-50,
				 80,50);
  array_play_button->off();
  array_play_button->setDisabled(true);
  connect(array_play_button,SIGNAL(clicked()),this,SLOT(playButtonData()));

  //
  // Stop Button
  //
  array_stop_button=new TransportButton(TransportButton::Stop,
					 this,"array_stop_button");
  array_stop_button->setGeometry(350,sizeHint().height()-50,80,50);
  array_stop_button->on();
  connect(array_stop_button,SIGNAL(clicked()),this,SLOT(stopButtonData()));

  //
  // Search Button
  //
  array_search_button=new QPushButton("&Search",this,"array_search_button");
  array_search_button->setGeometry(440,sizeHint().height()-50,80,50);
  array_search_button->setFont(button_font);
  connect(array_search_button,SIGNAL(clicked()),this,SLOT(searchButtonData()));

  //
  // Chain Button
  //
  array_chain_button=new PushButton("&Chain",this,"array_chain_button");
  array_chain_button->setGeometry(530,sizeHint().height()-50,80,50);
  array_chain_button->setFont(button_font);
  array_chain_button->setFlashColor(CHAIN_FLASH_COLOR);
  array_chain_button->setFlashingEnabled(array_chain_active);
  connect(array_chain_button,SIGNAL(clicked()),this,SLOT(chainButtonData()));

  //
  // Pause Button
  //
  array_pause_button=new TransportButton(TransportButton::Pause,
					 this,"array_pause_button");
  array_pause_button->
    setGeometry(sizeHint().width()-180,sizeHint().height()-50,80,50);
  array_pause_button->off();
  connect(array_pause_button,SIGNAL(clicked()),this,SLOT(pauseButtonData()));

  //
  // Loop Button
  //
  array_loop_button=new TransportButton(TransportButton::Loop,
					 this,"array_loop_button");
  array_loop_button->setGeometry(sizeHint().width()-90,sizeHint().height()-50,
				 80,50);
  array_loop_button->off();
  connect(array_loop_button,SIGNAL(clicked()),this,SLOT(loopButtonData()));

  //
  // The Main Menu
  //
  array_file_menu=new QMenu(this);
  menuBar()->insertItem("&File",array_file_menu);
  connect(array_file_menu,SIGNAL(activated(int)),
	  this,SLOT(activatedFileMenuData(int)));
  connect(array_file_menu,SIGNAL(aboutToShow()),
	  this,SLOT(updateFileMenuData()));

  array_settings_menu=new QMenu(this);
  menuBar()->insertItem("&Settings",array_settings_menu);
  connect(array_settings_menu,SIGNAL(aboutToShow()),
	  this,SLOT(updateSettingsMenuData()));

  array_help_menu=new QMenu(this);
  menuBar()->insertItem("&Help",array_help_menu);
  connect(array_help_menu,SIGNAL(aboutToShow()),
	  this,SLOT(updateHelpMenuData()));

  //
  // The File Menu
  //
  array_file_menu->insertItem("&New",this,SLOT(newMenuData()),0,0);
  array_file_menu->insertItem("&Open",this,SLOT(openMenuData()),0,1);
  array_file_menu->insertSeparator();
  array_file_menu->insertItem("&Save",this,SLOT(saveMenuData()),0,2);
  array_file_menu->insertItem("Save &As",this,SLOT(saveAsMenuData()),0,3);
  array_file_menu->insertSeparator();
  array_file_menu->insertItem("&Properties",
			      this,SLOT(propertiesMenuData()),0,4);
  array_file_menu->insertSeparator();
  array_file_menu->insertItem("&Quit",this,SLOT(quitMainWidget()),0,5);
  array_file_menu->insertSeparator();
  array_last_items=-1;
  UpdateLastList();

  //
  // The Settings Menu
  //
  array_settings_menu->insertItem("&General",
				  this,SLOT(generalSettingsMenuData()),0,0);
  array_settings_menu->insertItem("&Audio Devices",
				  this,SLOT(devicesMenuData()),0,1);
  array_settings_menu->insertItem("&Recorder",
				  this,SLOT(recordSettingsMenuData()),0,2);
  array_settings_menu->insertItem("&CD Ripper",
				  this,SLOT(ripperSettingsMenuData()),0,3);
  array_settings_menu->insertItem("Wave &Editor",
				  this,SLOT(editorSettingsMenuData()),0,4);

  //
  // The Help Menu
  //
  array_help_menu->insertItem("&About",this,SLOT(aboutHelpMenuData()),0,0);

  //
  // The Right-Button Menu
  //
  array_menu=new QMenu(this);
  connect(array_menu,SIGNAL(aboutToShow()),this,SLOT(updateButtonMenuData()));
  array_menu->insertItem("Cut",this,SLOT(cutButtonMenuData()),0,0);
  array_menu->insertItem("Copy",this,SLOT(copyButtonMenuData()),0,1);
  array_menu->insertItem("Paste",this,SLOT(pasteButtonMenuData()),0,2);
  array_menu->insertItem("Clear",this,SLOT(clearButtonMenuData()),0,3);
  array_menu->insertItem("Properties",
			 this,SLOT(propertiesButtonMenuData()),0,4);
  array_menu->insertSeparator();
  array_menu->insertItem("Record Audio",this,SLOT(recordButtonMenuData()),0,5);
  array_menu->insertItem("Rip CD",this,SLOT(ripButtonMenuData()),0,6);
  array_menu->insertItem("Edit Audio",this,SLOT(editButtonMenuData()),0,7);
  array_menu->insertItem("Delete Audio",this,SLOT(deleteButtonMenuData()),0,8);

  //
  // The Right Panel Box Menu
  //
  array_panel_menu=new QMenu(this);
  connect(array_panel_menu,SIGNAL(aboutToShow()),
	  this,SLOT(updatePanelMenuData()));
  array_panel_menu->insertItem("&Insert Panel",
			       this,SLOT(insertPanelMenuData()),0,0);
  array_panel_menu->insertItem("&Delete Panel",
			       this,SLOT(deletePanelMenuData()),0,1);
  array_panel_menu->insertItem("&Rename Panel",
			       this,SLOT(renamePanelMenuData()),0,2);

  //
  // Load/Initialize Array
  //
  if(array_filename.isEmpty()) {
    array_blank=true;
    array_title=QString(DEFAULT_ARRAY_TITLE);
    if(getenv("HOME")!=NULL) {
      array_path=getenv("HOME");
    }
    else {
      array_path="/";
    }
  }
  else {
    array_blank=false;
    LoadFile();
    AssignButtons();
    array_path=GetPathPart(array_filename);
  }
  array_panel=0;
  array_modified=false;
  SetCaption();

  // 
  // Setup signal handling 
  //
  ::signal(SIGCHLD,SigHandler);

  //
  // Load Default Array
  //
  switch(array_startup_mode) {
      case EditGeneral::Previous:
      case EditGeneral::Specified:
	array_filename=array_startup_filename;
	if(!LoadFile()) {
	  break;
	}
	array_panel=0;
	SetCaption();
	AssignButtons();
	array_path=GetPathPart(array_filename);
	AddLastList(array_filename);
	break;

      case EditGeneral::None:
	break;
  }
  SetHPIMixer();
}


QSize MainWidget::sizeHint() const
{
  return QSize(size_x,size_y);
}


QSizePolicy MainWidget::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void MainWidget::focusedData(int id)
{
  array_focus_y=id/BUTTON_COLUMNS;
  array_focus_x=id-array_focus_y*BUTTON_COLUMNS;
}


void MainWidget::leftButtonMapperData(int id)
{
  int row=id/BUTTON_COLUMNS;
  int col=id-row*BUTTON_COLUMNS;
  PlayStream::Error error;

  if(array_panels[array_panel].wavefile(col,row).isEmpty()) {
    return;
  }
  if(array_play->getState()!=PlayStream::Stopped) {
    array_play->stop();
  }
  current_row=row;
  current_col=col;
  current_panel=array_panel;
  current_id=id;
  current_offset=current_panel*BUTTON_ROWS*BUTTON_COLUMNS+id;
    
  if((error=array_play->openWave(array_panels[array_panel].
				 wavefile(current_col,current_row)))!=0) {
    QMessageBox::warning(this,"SoundPanel",array_play->errorString(error));
    return;
  }
  if(!array_play->getFormatChunk()) {
    array_play->closeWave();
    QMessageBox::warning(this,"SoundPanel","Unsupported audio file format");
    return;
  }
  current_samplerate=(double)array_play->getSamplesPerSec();
  current_samplelength=(double)array_play->getSampleLength();

  array_deck_label->setText(array_panels[current_panel].
			    legend(current_col,current_row));
  array_deck_wavefile=array_panels[current_panel].
    wavefile(current_col,current_row);
  array_deck_length=array_panels[current_panel].
		       length(current_col,current_row);
  array_deck_col=current_col;
  array_deck_row=current_row;
  array_deck_panel=current_panel;
  array_deck_offset=current_offset;
  array_play_button->setEnabled(true);
  for(int i=0;i<MAX_HPI_PORTS;i++) {
    array_card->
      setOutputVolume(array_play_card,array_play->getStream(),i,-10000);
  }
  array_card->setOutputVolume(array_play_card,array_play->getStream(),
			      array_play_port,0);
  if(!array_play->play()) {
    array_play->closeWave();
    QMessageBox::warning(this,"SoundPanel","Audio Device Error");
  }
}


void MainWidget::rightButtonMapperData(int xpos,int ypos)
{
  right_col=(xpos-X_MARGIN)/((geometry().width()-2*X_MARGIN)/BUTTON_COLUMNS);
  right_row=(ypos-Y_MARGIN)/((geometry().height()-Y_MARGIN-50)/BUTTON_ROWS);
  right_offset=array_panel*BUTTON_ROWS*BUTTON_COLUMNS+right_row*BUTTON_COLUMNS+
    right_col;
  if((right_row==current_row)&&(right_col==current_col)&&
     (array_play->getState()==PlayStream::Playing)) {
    return;
  }
  array_menu->setGeometry(geometry().x()+xpos,geometry().y()+ypos,
			 array_menu->sizeHint().width(),
			 array_menu->sizeHint().height());
  array_menu->exec();
}


void MainWidget::activatedFileMenuData(int id)
{
  if(id<10) {
    return;
  }
  if(!PromptSave()) {
    return;
  }
  array_filename=array_last_filename[id-11];
  LoadFile();
  array_panel=0;
  SetCaption();
  AssignButtons();
  array_path=GetPathPart(array_filename);
}


void MainWidget::updateFileMenuData()
{
  if(array_play->getState()==PlayStream::Stopped) {
    array_file_menu->setItemEnabled(0,true);
    array_file_menu->setItemEnabled(1,true);
    array_file_menu->setItemEnabled(2,array_modified);
    array_file_menu->setItemEnabled(3,array_modified||(!array_blank));
    array_file_menu->setItemEnabled(4,true);
  }
  else {
    array_file_menu->setItemEnabled(0,false);
    array_file_menu->setItemEnabled(1,false);
    array_file_menu->setItemEnabled(2,false);
    array_file_menu->setItemEnabled(3,false);
    array_file_menu->setItemEnabled(4,false);
  }
}


void MainWidget::updateSettingsMenuData()
{
  if(array_play->getState()==PlayStream::Stopped) {
    array_settings_menu->setItemEnabled(0,true);
    array_settings_menu->setItemEnabled(1,true);
    array_settings_menu->setItemEnabled(2,true);
    array_settings_menu->setItemEnabled(3,true);
  }
  else {
    array_settings_menu->setItemEnabled(0,false);
    array_settings_menu->setItemEnabled(1,false);
    array_settings_menu->setItemEnabled(2,false);
    array_settings_menu->setItemEnabled(3,false);
  }
}


void MainWidget::updateHelpMenuData()
{
  if(array_play->getState()==PlayStream::Stopped) {
    array_help_menu->setItemEnabled(0,true);
  }
  else {
    array_help_menu->setItemEnabled(0,false);
  }
}


void MainWidget::newMenuData()
{
  if(!PromptSave()) {
    return;
  }
  NewFile();
}


void MainWidget::openMenuData()
{
  if(!PromptSave()) {
    return;
  }
  array_filename=QFileDialog::getOpenFileName(array_path,
					      "Array Files (*.aar)",this);
  LoadFile();
  array_panel=0;
  SetCaption();
  AssignButtons();
  array_path=GetPathPart(array_filename);
  AddLastList(array_filename);
}


void MainWidget::saveMenuData()
{
  if(array_filename.isEmpty()) {
    saveAsMenuData();
    return;
  }
  SaveFile();
}


void MainWidget::saveAsMenuData()
{
  QString filename;

  if(!array_filename.isEmpty()) {
    filename=QFileDialog::getSaveFileName(array_filename,
					  "Array Files (*.aar)",this);
  }
  else {
    filename=QFileDialog::getSaveFileName(array_path,
					  "Array Files (*.aar)",this);
  }
  if(filename.right(4)!=QString(".aar")) {
    filename+=QString(".aar");
  }
  QFile *file=new QFile(filename);
  if(file->exists()) {
    if(!QMessageBox::warning(this,"File Exists",
	     "This file already exists.\nDo you want to overwrite it?",
	     "OK","Cancel",0,0,1)==0) {
      delete file;
      return;
    }
  }
  array_filename=filename;
  SaveFile();
  array_path=GetPathPart(array_filename);
  SetCaption();
}


void MainWidget::propertiesMenuData()
{
  SndProperties *prop=new SndProperties(&array_title,this,"prop");
  if(prop->exec()==0) {
    SetCaption();
    array_blank=false;
    array_modified=true;
  }
  delete prop;
}


void MainWidget::generalSettingsMenuData()
{
  EditGeneral *edit=
    new EditGeneral(&array_startup_mode,&array_startup_filename,&array_path,
		    this,"rip");
  if(edit->exec()==0) {
    SaveConfig();
  }
  delete edit;
}


void MainWidget::devicesMenuData()
{
  EditDevices *edit=new EditDevices(&array_alsa_playdev,
				    &array_alsa_recdev,
				    &array_play_card,
				    &array_play_port,
				    &clock_source,
				    &array_input_type,
				    &array_rec_card,
				    &array_rec_port,
				    &array_rec_level,&array_play_level,
				    this,"edit");
  if(edit->exec()==0) {
    array_play->setCard(array_play_card);
    SetHPIMixer();
    BuildFileFilter();
    SaveConfig();
  }
  delete edit;
}


void MainWidget::recordSettingsMenuData()
{
  AudioSettingsDialog *settings=
    new AudioSettingsDialog(&array_audio_settings,true,this,"settings");
  settings->setCaption("Edit Default Settings");
  if(settings->exec()==0) {
    SaveConfig();
  }
  delete settings;
}


void MainWidget::ripperSettingsMenuData()
{
  EditRipper *rip=new EditRipper(&array_ripdev,&array_paranoia_level,
				 &array_cddb_server,&array_normal_level,
				 this,"rip");
  if(rip->exec()==0) {
    SaveConfig();
  }
  delete rip;
}


void MainWidget::editorSettingsMenuData()
{
  EditEditor *edit=new EditEditor(&array_editor,this,"rip");
  if(edit->exec()==0) {
    SaveConfig();
  }
  delete edit;
}


void MainWidget::aboutHelpMenuData()
{
  About *about=new About(this,"about");
  about->exec();
  delete about;
}


void MainWidget::updateButtonMenuData()
{
  if(array_play->getState()==PlayStream::Stopped) {
    if((array_panels[array_panel].wavefile(right_col,right_row)
		 .isEmpty()&&
	       array_panels[array_panel].legend(right_col,right_row).
	isEmpty())) {
      array_menu->setItemEnabled(0,false);
      array_menu->setItemEnabled(1,false);
    }
    else {
      array_menu->setItemEnabled(0,true);
      array_menu->setItemEnabled(1,true);
    }
    array_menu->setItemEnabled(2,
	       !(array_clip_wavefile.isEmpty()&&array_clip_legend.isEmpty()));
    array_menu->setItemEnabled(3,
	       !(array_panels[array_panel].wavefile(right_col,right_row).
		 isEmpty()&&
		 array_panels[array_panel].legend(right_col,right_row).
		 isEmpty()&&
		 array_panels[array_panel].color(right_col,right_row)==
		 backgroundColor()));
    array_menu->setItemEnabled(4,true);
    array_menu->setItemEnabled(5,true);
    array_menu->setItemEnabled(6,true);
    if((!array_editor.isEmpty())&&
       (!array_panels[array_panel].wavefile(right_col,right_row).isEmpty())) {
      array_menu->setItemEnabled(7,true);
    }
    else {
      array_menu->setItemEnabled(7,false);
    }
    if(QFile::exists(array_panels[array_panel].
		     wavefile(right_col,right_row))) {
      array_menu->setItemEnabled(8,true);
    }
    else {
      array_menu->setItemEnabled(8,false);
    }
  }
  else {
    array_menu->setItemEnabled(0,false);
    array_menu->setItemEnabled(1,false);
    array_menu->setItemEnabled(2,false);
    array_menu->setItemEnabled(3,false);
    array_menu->setItemEnabled(4,false);
    array_menu->setItemEnabled(5,false);
    array_menu->setItemEnabled(6,false);
    array_menu->setItemEnabled(7,false);
    array_menu->setItemEnabled(8,false);
  }
}


void MainWidget::cutButtonMenuData()
{
  copyButtonMenuData();
  clearButtonMenuData();
}


void MainWidget::copyButtonMenuData()
{
  array_clip_legend=array_panels[array_panel].legend(right_col,right_row);
  array_clip_wavefile=array_panels[array_panel].wavefile(right_col,right_row);
  array_clip_color=array_panels[array_panel].color(right_col,right_row);
}


void MainWidget::pasteButtonMenuData()
{
  array_panels[array_panel].setLegend(right_col,right_row,array_clip_legend);
  array_panels[array_panel].
    setWavefile(right_col,right_row,array_clip_wavefile);
  array_panels[array_panel].
    setColor(right_col,right_row,array_clip_color);
  UpdateButton(right_col,right_row);
  array_blank=false;
  array_modified=true;
}



void MainWidget::clearButtonMenuData()
{
  array_panels[array_panel].setLegend(right_col,right_row,"");
  array_panels[array_panel].setWavefile(right_col,right_row,"");
  array_panels[array_panel].setColor(right_col,right_row,backgroundColor());
  UpdateButton(right_col,right_row);
  array_modified=true;
}


void MainWidget::propertiesButtonMenuData()
{
  QString leg=array_panels[array_panel].legend(right_col,right_row);
  QString wav=array_panels[array_panel].wavefile(right_col,right_row);
  QColor color=array_panels[array_panel].color(right_col,right_row);

  EditButton *edit_button=
    new EditButton(&leg,&wav,
		   &array_path,&color,&array_cc,
		   this,"list_users");
  if(edit_button->exec()==0) {
    array_panels[array_panel].setLegend(right_col,right_row,leg);
    array_panels[array_panel].setWavefile(right_col,right_row,wav);
    array_panels[array_panel].setColor(right_col,right_row,color);
    UpdateButton(right_col,right_row); 
    array_modified=true;
  }
  delete edit_button;
}


void MainWidget::recordButtonMenuData()
{
  QString leg;
  QString wav;

  leg=array_panels[array_panel].legend(right_col,right_row);
  wav=array_panels[array_panel].wavefile(right_col,right_row);
  RecordAudio *record=
    new RecordAudio(&leg,&wav,
		    &array_path,array_alsa_playdev,
		    array_alsa_recdev,array_play_card,
		    array_input_type,
		    array_rec_card,
		    array_rec_port,array_rec_level,&array_audio_settings,
		    array_card,this,"list_users");
  if(record->exec()==0) {
    array_panels[array_panel].setLegend(right_col,right_row,leg);
    array_panels[array_panel].setWavefile(right_col,right_row,wav);
    UpdateButton(right_col,right_row); 
    array_modified=true;
  }
  delete record;
}


void MainWidget::ripButtonMenuData()
{
  static bool apply_cddb_title=false;
  QString leg;
  QString wav;

  leg=array_panels[array_panel].legend(right_col,right_row);
  wav=array_panels[array_panel].wavefile(right_col,right_row);
  RipperDialog *rip=
    new RipperDialog(&leg,&wav,
		     &array_path,array_ripdev,
		     array_paranoia_level,array_cddb_server,
		     &apply_cddb_title,&array_normalize,array_normal_level,
		     &array_audio_settings,this,"rip");
  if(rip->exec()==0) {
    array_panels[array_panel].setLegend(right_col,right_row,leg);
    array_panels[array_panel].setWavefile(right_col,right_row,wav);
    UpdateButton(right_col,right_row); 
    array_modified=true;
    SaveConfig();
  }
  delete rip;
}


void MainWidget::editButtonMenuData()
{
  if(system((const char *)(array_editor+QString(" ")+
		   array_panels[array_panel].wavefile(right_col,right_row)))
     !=0) {
    QMessageBox::warning(this,"No Wave Editor","Unable to start Wave Editor!");
    return;
  }
  UpdateButton(right_col,right_row); 
}


void MainWidget::deleteButtonMenuData()
{
  if(!QMessageBox::warning(this,"Delete Audio",
			   "Are you sure you want to delete the audio?",
			   "OK","Cancel",0,0,1)==0) {
    return;
  }
  QFile::remove(array_panels[array_panel].wavefile(right_col,right_row));
  clearButtonMenuData();
}


void MainWidget::rightBoxData(QPoint p)
{
  array_panel_menu->setGeometry(p.x(),p.y(),
				array_panel_menu->sizeHint().width(),
				array_panel_menu->sizeHint().height());
  array_panel_menu->exec();
}


void MainWidget::updatePanelMenuData()
{
  if(array_play->getState()==PlayStream::Stopped) {
    array_panel_menu->setItemEnabled(0,true);
    if(array_panel_quantity>1) {
      array_panel_menu->setItemEnabled(1,true);
    }
    else {
      array_panel_menu->setItemEnabled(1,false);
    }
    array_panel_menu->setItemEnabled(2,true);
  }
  else {
    array_panel_menu->setItemEnabled(0,false);
    array_panel_menu->setItemEnabled(1,false);
    array_panel_menu->setItemEnabled(2,false);
  }
}


void MainWidget::insertPanelMenuData()
{
  array_panels.insert(array_panels.begin()+array_panel,1,Panel());
  array_panels[array_panel].initColor(backgroundColor());
  array_panel_box->
    insertItem(array_panel_box->text(array_panel_box->count()-1));
  for(int i=array_panel_quantity;i>array_panel;i--) {
    array_panel_box->changeItem(array_panel_box->text(i-1),i);
  }
  array_panel_box->changeItem("New Panel",array_panel);
  array_panel_quantity++;
  array_modified=true;
  panelActivatedData(array_panel);
}


void MainWidget::deletePanelMenuData()
{
  if(!QMessageBox::warning(this,"Delete Panel",
			   "Are you sure you want to delete the panel?",
			   "OK","Cancel",0,0,1)==0) {
    return;
  }
  array_panels.erase(array_panels.begin()+array_panel,
		     array_panels.begin()+array_panel+1);
  for(int i=array_panel;i<(array_panel_quantity-1);i++) {
    array_panel_box->changeItem(array_panel_box->text(i+1),i);
  }
  array_panel_box->removeItem(array_panel_box->count()-1);
  array_panel_quantity--;
  array_modified=true;
  panelActivatedData(array_panel);
}


void MainWidget::renamePanelMenuData()
{
  QString panel_name;

  panel_name=array_panels[array_panel].name();
  PanelName *panel=new PanelName(&panel_name,this,"panel");
  if(panel->exec()==0) {
    array_panels[array_panel].setName(panel_name);
    array_panel_box->changeItem(array_panels[array_panel].name(),array_panel);
    array_modified=true;
  }
  delete panel;
}


void MainWidget::pauseButtonData()
{
  if(array_play->getState()==PlayStream::Playing) {
    array_play->pause();
  }
}


void MainWidget::stopButtonData()
{
  if(array_play->getState()!=PlayStream::Stopped) {
    array_stopping=true;
    array_play->stop();
  }
}


void MainWidget::loopButtonData()
{
  if(array_looping) {
    array_loop_button->off();
    array_looping=false;
  }
  else {
    array_loop_button->on();
    array_looping=true;
  }
}


void MainWidget::playButtonData()
{
  PlayStream::Error error;

  if(array_deck_wavefile.isEmpty()) {
    return;
  }
  if(array_play->getState()!=PlayStream::Paused) {
    if(array_play->getState()!=PlayStream::Stopped) {
      array_play->stop();
    }
    current_row=array_deck_row;
    current_col=array_deck_col;
    current_panel=array_deck_panel;
    current_id=array_deck_id;
    current_offset=array_deck_offset;
    
    if((error=array_play->openWave(array_deck_wavefile))!=0) {
      QMessageBox::warning(this,"SoundPanel",array_play->errorString(error));
      return;
    }
    if(!array_play->getFormatChunk()) {
      array_play->closeWave();
      QMessageBox::warning(this,"SoundPanel","Unsupported audio file format");
      return;
    }
    current_samplerate=(double)array_play->getSamplesPerSec();
    current_samplelength=(double)array_play->getSampleLength();
  }
  if(!array_play->play()) {
    array_play->closeWave();
    QMessageBox::warning(this,"SoundPanel","Audio Device Error");
  }
}


void MainWidget::searchButtonData()
{
  QString search_string;

  SearchArray *search=new SearchArray(&search_string,this,"search");
  if(search->exec()==0) {
    LoadCut(search_string);
  }
  delete search;
}


void MainWidget::chainButtonData()
{
  array_chain_active=!array_chain_active;
  array_chain_button->setFlashingEnabled(array_chain_active);
}


void MainWidget::playedData()
{
  array_play_button->on();
  array_pause_button->off();
  array_stop_button->off();
  array_search_button->setDisabled(true);
  if(current_panel==array_panel) {
    array_button[current_row][current_col]->
      setPalette(QPalette(QColor(PLAYING_BUTTON_COLOR),backgroundColor()));
  }
  meter_timer->start(METER_UPDATE_INTERVAL);
}


void MainWidget::pausedData()
{
  array_play_button->flash();
  array_pause_button->on();
  array_stop_button->off();
  meter_timer->stop();
  left_meter->setPeakBar(-10000);
  right_meter->setPeakBar(-10000);
}


void MainWidget::stoppedData()
{
  array_play->closeWave();
  array_play_button->off();
  array_pause_button->off();
  array_stop_button->on();
  array_search_button->setEnabled(true);
  array_button[current_row][current_col]->
    setColor(array_panels[array_panel].color(current_col,current_row));
  if(array_looping) {
    if(!array_stopping) {
      leftButtonMapperData(current_id);
    }
  }
  else {
    if(array_chain_active&&(current_col<(BUTTON_COLUMNS-1))&&
       (!array_stopping)) {
      leftButtonMapperData(current_row*BUTTON_COLUMNS+current_col+1);
    }
  }
  array_stopping=false;
  meter_timer->stop();
  left_meter->setPeakBar(-10000);
  right_meter->setPeakBar(-10000);
}


void MainWidget::positionData(int pos)
{
  static QString lastval;

  int time_left=(int)(1000.0*(current_samplelength-(double)pos)/
		      current_samplerate);
  lastval=GetTimeLength(time_left,true,false);
  array_time_label->setText(lastval);
  if(current_panel!=array_panel) {
    return;
  }
  array_button[current_row][current_col]->
    setLength(time_left);
}


void MainWidget::meterData()
{
  short level[2];
  array_card->outputStreamMeter(array_play_card,array_play->getStream(),level);
  left_meter->setPeakBar(level[0]);
  right_meter->setPeakBar(level[1]);
}


void MainWidget::quitMainWidget()
{
  if(!PromptSave()) {
    return;
  }
  array_play->stop();
  qApp->quit();
}


void MainWidget::mousePressEvent(QMouseEvent *e)
{
  switch(e->button()) {
      case Qt::RightButton:
	if((e->pos().x()<X_MARGIN)||e->pos().x()>(size().width()-X_MARGIN)||
	   (e->pos().y()<Y_MARGIN)||e->pos().y()>(size().height()-55)) {
	  e->ignore();
	  return;
	}
	rightButtonMapperData(e->pos().x(),e->pos().y());
	break;

      default:
	e->ignore();
	break;
  }
}


void MainWidget::keyPressEvent(QKeyEvent *e)
{
  switch(e->key()) {
      case Qt::Key_Up:
	if(array_focus_y>0) {
	  array_button[--array_focus_y][array_focus_x]->setFocus();
	  e->accept();
	}
	break;
      case Qt::Key_Down:
	if(array_focus_y<(BUTTON_ROWS-1)) {
	  array_button[++array_focus_y][array_focus_x]->setFocus();
	  e->accept();
	}
	break;
      case Qt::Key_Left:
	if(array_focus_x>0) {
	  array_button[array_focus_y][--array_focus_x]->setFocus();
	  e->accept();
	}
	break;
      case Qt::Key_Right:
	if(array_focus_x<(BUTTON_COLUMNS-1)) {
	  array_button[array_focus_y][++array_focus_x]->setFocus();
	  e->accept();
	}
	break;
      case Qt::Key_Home:
	playButtonData();
	e->accept();
	break;
      case Qt::Key_End:
	stopButtonData();
	e->accept();
	break;
      default:
	break;
  }
}


void MainWidget::panelActivatedData(int)
{
  if(array_play->getState()!=PlayStream::Stopped) {
    if(array_panel==current_panel) {
      array_button[current_row][current_col]->setPalette(palette());
    }
    if(array_panel_box->currentItem()==current_panel) {
      array_button[current_row][current_col]->
	setPalette(QPalette(QColor(PLAYING_BUTTON_COLOR),backgroundColor()));
    }
  }
  array_panel=array_panel_box->currentItem();
  AssignButtons();
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  if(left_meter!=NULL) {
    left_meter->setGeometry(e->size().width()-310,32,300,10);
    right_meter->setGeometry(e->size().width()-310,42,300,10);
  }
  int bx=(geometry().width()-2*X_MARGIN)/BUTTON_COLUMNS;
  int by=(geometry().height()-Y_MARGIN-50)/BUTTON_ROWS;
  for(int i=0;i<BUTTON_ROWS;i++) {
    for(int j=0;j<BUTTON_COLUMNS;j++) {
      array_button[i][j]->setGeometry(X_MARGIN+j*bx,Y_MARGIN+i*by,bx,by);
    }
  }
  array_deck_label->setGeometry(10,e->size().height()-51,240,26);
  array_time_label->setGeometry(10,e->size().height()-26,240,26);
  array_play_button->setGeometry(260,e->size().height()-50,
				 80,50);
  array_stop_button->setGeometry(350,e->size().height()-50,80,50);
  array_search_button->setGeometry(440,e->size().height()-50,80,50);
  array_chain_button->
    setGeometry(e->size().width()-270,e->size().height()-50,80,50);
  array_pause_button->
    setGeometry(e->size().width()-180,e->size().height()-50,80,50);
  array_loop_button->setGeometry(e->size().width()-90,e->size().height()-50,
				 80,50);
  QWidget::resizeEvent(e);
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  if(!PromptSave()) {
    return;
  }
  array_play->stop();
  SaveConfig();
  e->accept();
}


void MainWidget::AddLastList(QString filename)
{
/*
  for(int i=0;i<MAX_LAST_ARRAYS;i++) {
    if(filename==array_last_filename[i]) {
      return;
    }
  }

  for(int i=MAX_LAST_ARRAYS-1;i>0;i--) {
    array_last_filename[i]=array_last_filename[i-1];
  }
  array_last_filename[0]=filename;
  UpdateLastList();
  SaveConfig();
*/
}


void MainWidget::UpdateLastList()
{
/*
  for(int i=0;i<MAX_LAST_ARRAYS;i++) {
    if(array_last_filename[i].isEmpty()) {
      i=MAX_LAST_ARRAYS;
      array_last_items=i;
    }
    else {
      if(array_file_menu->idAt(i+10)<0) {
	printf("Inserted: %d\n",i);
	array_file_menu->insertItem(array_last_filename[i],0,i+10);
      }
      else {
	printf("Changed: %d\n",i);
	array_file_menu->changeItem(array_last_filename[i],i+10);
      }
    }
  }
  if(array_last_items==0) {
    array_last_items=MAX_LAST_ARRAYS;
  }
*/
}


void MainWidget::SetHPIMixer()
{
  array_card->setClockSource(array_play_card,clock_source);
  array_card->
    setOutputLevel(array_play_card,array_play_port,
		   array_play_level-DEFAULT_NORMAL_LEVEL);
  BuildFileFilter();
  array_card->setInputPortMux(array_rec_card,array_rec_port,array_input_type);
}


void MainWidget::UpdateButton(int col,int row)
{
  if(array_panels[array_panel].legend(col,row).isEmpty()) {
    array_panels[array_panel].setLegend(col,row,
		 GetBasePart(array_panels[array_panel].wavefile(col,row)));
  }
  if(!array_panels[array_panel].legend(col,row).isEmpty()) {
    array_button[row][col]->
      setLabel(array_panels[array_panel].legend(col,row));
    array_button[row][col]->
      setLength(array_panels[array_panel].length(col,row));
  }
  else {
    array_button[row][col]->clear();
  }
  array_button[row][col]->
    setColor(array_panels[array_panel].color(col,row));
}


void MainWidget::SetCaption()
{
  if(array_filename.isEmpty()) {
    setCaption(QString().sprintf("SoundPanel - %s",
				 (const char *)array_title));
  }
  else {
    setCaption(QString().sprintf("SoundPanel - %s [%s]",
				 (const char *)array_title,
				 (const char *)GetBasePart(array_filename)));
  }
}


void MainWidget::AssignButtons()
{
  for(int i=0;i<BUTTON_ROWS;i++) {
    for(int j=0;j<BUTTON_COLUMNS;j++) {
      UpdateButton(j,i);
    }
  }
}


bool MainWidget::LoadCut(QString search)
{
  for(int i=0;i<array_panel_quantity;i++) {
    for(int j=0;j<BUTTON_ROWS;j++) {
      for(int k=0;k<BUTTON_COLUMNS;k++) {
	if(array_panels[i].legend(k,j).contains(search,false)>0) {
	  array_deck_row=j;
	  array_deck_col=k;
	  array_deck_label->setText(array_panels[i].legend(k,j));
	  array_time_label->setText(GetTimeLength(array_panels[i].length(k,j),
						   true,false));
	  array_deck_wavefile=array_panels[i].wavefile(k,j);
	  array_deck_length=array_panels[i].length(k,j);
	  array_deck_panel=i;
	  array_play_button->setEnabled(true);
	  return true;
	}
      }
    }
  }
  array_deck_label->setText("<none>");
  array_time_label->setText("-:--:--");
  array_play_button->setDisabled(true);
  return false;
}


bool MainWidget::PromptSave()
{
  if(array_modified) {
    switch(QMessageBox::warning(this,"File Modified",
		   "The array has been modified.\nWould you like to save it?",
		   "&Yes","&No","&Cancel",0)) {
	case 0:
	  saveMenuData();
	  return true;
	  break;
	case 1:
	  return true;
	  break;
	case 2:
	  return false;
	  break;
    }
  }
  return true;
}


void MainWidget::NewFile()
{
  array_filename="";
  array_title=DEFAULT_ARRAY_TITLE;
  setCaption(QString().sprintf("SoundPanel - %s",(const char *)array_title));
  array_panels.resize(0);
  array_panels.insert(array_panels.begin(),BUTTON_PANELS,Panel());
  array_panels.back().initColor(backgroundColor());
  array_panel_quantity=1;
  array_panels[0].setName("Panel 1");
  array_panel_box->clear();
  for(int i=0;i<BUTTON_PANELS;i++) {
    array_panel_box->insertItem(QString().sprintf("Panel %d",i+1),i);
  }
  array_panel=0;
  AssignButtons();
  array_blank=true;
  array_modified=false;
}


bool MainWidget::LoadFile()
{
  char buffer[MAX_LINE];
  int n;
  int panel=-1;
  char lhs[MAX_LINE];
  char rhs[MAX_LINE];
  int line;

  QFile *file=new QFile(array_filename);
  if(!file->exists()) {
    delete file;
    return false;
  }
  if(!file->open(QIODevice::ReadOnly)) {
    delete file;
    return false;
  }
  array_panel_quantity=BUTTON_PANELS;
  while((n=file->readLine(buffer,MAX_LINE-1))>=0) {
    buffer[n]=0;
    if(buffer[0]=='[') {  // Block Tag
      if(sscanf(buffer,"[%s",lhs)==1) {
	if(!strcasecmp(lhs,"global]")) {
	  panel=-1;	
	}
	else {
	  sscanf(lhs+5,"%d",&panel);
	}
      }
    }
    else {
      if(panel<array_panel_quantity) {
	for(int i=0;i<n;i++) {
	  if(buffer[i]!='=') {
	    lhs[i]=buffer[i];
	  }
	  else {
	    lhs[i]=0;
	    strcpy(rhs,buffer+i+1);
	    rhs[strlen(rhs)-1]=0;
	    i=n;
	  }
	}
	if(!strncasecmp(lhs,"panels",6)) {   // Panel Quantity Field
	  if(panel==-1) {
	    sscanf(rhs,"%d",&array_panel_quantity);
	    array_panels.resize(0);
	    array_panels.insert(array_panels.begin(),array_panel_quantity,
				Panel());
	    array_panels[0].initColor(backgroundColor());
	    array_panel_box->clear();
	    for(int i=0;i<array_panel_quantity;i++) {
	      array_panel_box->
		insertItem(QString().sprintf("Panel %d",i+1),i);
	    }
	  }
	}
	if(!strncasecmp(lhs,"title",5)) {    // Title Field
	  if(panel==-1) {
	    array_title=QString(rhs);
	  }
	  else {
	    array_panels[panel].setName(QString(rhs));
	    array_panel_box->changeItem(array_panels[panel].name(),panel);
	  }
	}
	if(!strncasecmp(lhs,"legend",6)) {    // Legend Field
	  sscanf(lhs+6,"%d",&line);
	  array_panels[panel].setLegend(line,QString(rhs));
	}
	if(!strncasecmp(lhs,"wavefile",8)) {  // Wavefile Field
	  sscanf(lhs+8,"%d",&line);
	  array_panels[panel].setWavefile(line,QString(rhs));
	}
	if(!strncasecmp(lhs,"color",5)) {  // Color Field
	  sscanf(lhs+5,"%d",&line);
	  array_panels[panel].setColor(line,QColor(rhs));
	}
      }
    }
  }
  file->close();
  delete file;
  array_blank=false;
  array_modified=false;
  array_panel=0;
  array_panel_box->setCurrentItem(array_panel);
  return true;
}


bool MainWidget::SaveFile()
{
  FILE *filename;
  int n;

  if((filename=fopen((const char *)array_filename,"w"))==NULL) {
    return false;
  }
  fprintf(filename,"[Global]\n");
  fprintf(filename,"Title=%s\n",(const char *)array_title);
  fprintf(filename,"Panels=%d\n",array_panel_quantity);
  fprintf(filename,"\n");
  for(int i=0;i<array_panel_quantity;i++) {
    fprintf(filename,"[Panel%d]\n",i);
    fprintf(filename,"Title=%s\n",(const char *)array_panels[i].name());
    QColor background_color=backgroundColor();
    for(int j=0;j<BUTTON_ROWS;j++) {
      for(int k=0;k<BUTTON_COLUMNS;k++) {
	n=j*BUTTON_COLUMNS+k;
	if(!array_panels[i].legend(k,j).isEmpty()) {
	  fprintf(filename,"Legend%d=%s\n",n,
		  (const char *)array_panels[i].legend(k,j));
	}
	if(!array_panels[i].wavefile(k,j).isEmpty()) {
	  fprintf(filename,"Wavefile%d=%s\n",n,
		  (const char *)array_panels[i].wavefile(k,j));
	}
	if(array_panels[i].color(k,j)!=background_color) {
	  fprintf(filename,"Color%d=%s\n",n,
		  (const char *)array_panels[i].color(k,j).name());
	}
      }
    }  
  }
  fclose(filename);
  array_blank=false;
  array_modified=false;

  return true;
}


void MainWidget::LoadConfig()
{
  char temp[256];
  char section[256];

  array_startup_mode=EditGeneral::None;
  GetPrivateProfileString(array_configname,"Startup","ArrayMode",temp,
			  DEFAULT_ARRAY_STARTUP_MODE,255);
  if(!strcasecmp(temp,"previous")) {
    array_startup_mode=EditGeneral::Previous;
  }
  if(!strcasecmp(temp,"specified")) {
    array_startup_mode=EditGeneral::Specified;
  }
  GetPrivateProfileString(array_configname,"Startup","Array",temp,"",255);
  array_startup_filename=QString(temp);

  size_x=GetPrivateProfileInt(array_configname,"Geometry",
			     "SizeX",DEFAULT_SIZE_X);
  size_y=GetPrivateProfileInt(array_configname,"Geometry",
			     "SizeY",DEFAULT_SIZE_Y);
  GetPrivateProfileString(array_configname,"Devices","InputType",temp,
			  DEFAULT_INPUT_TYPE,255);
  array_input_type=SoundCard::LineIn;
  if(!strcasecmp(temp,"analog")) {
    array_input_type=SoundCard::LineIn;
  }
  if(!strcasecmp(temp,"digital")) {
    array_input_type=SoundCard::AesEbuIn;
  }
  clock_source=
    (SoundCard::ClockSource)GetPrivateProfileInt(array_configname,
						     "Devices",
						     "HPIClockSource",
						     DEFAULT_CLOCK_SOURCE);
  array_rec_card=GetPrivateProfileInt(array_configname,"Devices",
				      "HPIRecordCard",DEFAULT_HPI_CARD);
  array_rec_port=GetPrivateProfileInt(array_configname,"Devices",
				      "HPIRecordPort",DEFAULT_HPI_PORT);
  array_rec_level=GetPrivateProfileInt(array_configname,"Devices",
				      "HPIRecordLevel",DEFAULT_HPI_LEVEL);
  array_play_card=GetPrivateProfileInt(array_configname,"Devices",
				      "HPIPlayCard",DEFAULT_HPI_CARD);
  array_play_port=GetPrivateProfileInt(array_configname,"Devices",
				      "HPIPlayPort",DEFAULT_HPI_PORT);
  array_play_level=GetPrivateProfileInt(array_configname,"Devices",
				      "HPIPlayLevel",DEFAULT_HPI_LEVEL);
  GetPrivateProfileString(array_configname,"Devices","RipperDevice",temp,
			  DEFAULT_RIPPER_DEVICE,255);
  array_ripdev=QString(temp);
  array_paranoia_level=GetPrivateProfileInt(array_configname,"Devices",
					    "ParanoiaLevel",
					    DEFAULT_PARANOIA_LEVEL);
  GetPrivateProfileString(array_configname,"Devices","CDDBServer",temp,
			  DEFAULT_CDDB_SERVER,255);
  array_cddb_server=QString(temp);
  array_normal_level=GetPrivateProfileInt(array_configname,"Devices",
					  "NormalLevel",DEFAULT_NORMAL_LEVEL);
  array_normalize=GetPrivateProfileBool(array_configname,"Devices",
					"Normalize",DEFAULT_NORMALIZE);
  GetPrivateProfileString(array_configname,"Devices","EditorCommand",temp,
			  DEFAULT_EDITOR_COMMAND,255);
  array_editor=QString(temp);
  array_audio_settings.
    setFormat((AudioSettings::Format)
	      GetPrivateProfileInt(array_configname,"Settings",
				   "Format",DEFAULT_AUDIO_FORMAT));
  array_audio_settings.
    setChannels(GetPrivateProfileInt(array_configname,"Settings",
				     "Channels",DEFAULT_AUDIO_CHANNELS));
  array_audio_settings.
    setSampleRate(GetPrivateProfileInt(array_configname,"Settings",
				     "SampleRate",DEFAULT_AUDIO_SAMPRATE));
  array_audio_settings.
    setBitRate(GetPrivateProfileInt(array_configname,"Settings",
				     "BitRate",DEFAULT_AUDIO_BITRATE));
  array_chain_active=GetPrivateProfileBool(array_configname,"Settings",
					   "ChainActive",DEFAULT_CHAIN_ACTIVE);
  for(int i=0;i<MAX_LAST_ARRAYS;i++) {
    sprintf(section,"Array%d",i);
    GetPrivateProfileString(array_configname,"LastLoaded",section,temp,"",255);
    array_last_filename[i]=QString(temp);
  }
}


void MainWidget::SaveConfig()
{
  FILE *filename;
  char temp[256];

  if((filename=fopen((const char *)array_configname,"w"))==NULL) {
    return;
  }
  fprintf(filename,"[Startup]\n");
  switch(array_startup_mode) {
      case EditGeneral::None:
	fprintf(filename,"ArrayMode=None\n");
	break;

      case EditGeneral::Previous:
	fprintf(filename,"ArrayMode=Previous\n");
	fprintf(filename,"Array=%s\n",(const char *)array_filename);
	break;

      case EditGeneral::Specified:
	fprintf(filename,"ArrayMode=Specified\n");
	fprintf(filename,"Array=%s\n",(const char *)array_startup_filename);
	break;
  }

  fprintf(filename,"[Geometry]\n");
  fprintf(filename,"SizeX=%d\n",geometry().width());
  fprintf(filename,"SizeY=%d\n",geometry().height());
  fprintf(filename,"\n");
  fprintf(filename,"[Devices]\n");
  switch(array_input_type) {
      case SoundCard::LineIn:
	fprintf(filename,"InputType=Analog\n");
	break;
      case SoundCard::AesEbuIn:
	fprintf(filename,"InputType=Digital\n");
	break;
      default:
	break;
  }
  fprintf(filename,"HPIClockSource=%d\n",clock_source);
  fprintf(filename,"HPIRecordCard=%d\n",array_rec_card);
  fprintf(filename,"HPIRecordPort=%d\n",array_rec_port);
  fprintf(filename,"HPIRecordLevel=%d\n",array_rec_level);
  fprintf(filename,"HPIPlayCard=%d\n",array_play_card);
  fprintf(filename,"HPIPlayPort=%d\n",array_play_port);
  fprintf(filename,"HPIPlayLevel=%d\n",array_play_level);
  fprintf(filename,"RipperDevice=%s\n",(const char *)array_ripdev);
  fprintf(filename,"ParanoiaLevel=%d\n",(int)array_paranoia_level);
  fprintf(filename,"CDDBServer=%s\n",(const char *)array_cddb_server);
  fprintf(filename,"NormalLevel=%d\n",array_normal_level);
  if(array_normalize) {
    fprintf(filename,"Normalize=Yes\n");
  }
  else {
    fprintf(filename,"Normalize=No\n");
  }
  fprintf(filename,"EditorCommand=%s\n",(const char *)array_editor);
  fprintf(filename,"\n");
  fprintf(filename,"[Settings]\n");
  fprintf(filename,"Format=%d\n",(int)array_audio_settings.format());
  fprintf(filename,"Channels=%d\n",array_audio_settings.channels());
  fprintf(filename,"SampleRate=%d\n",array_audio_settings.sampleRate());
  fprintf(filename,"BitRate=%d\n",array_audio_settings.bitRate());
  if(array_chain_active) {
    fprintf(filename,"ChainActive=Yes\n");
  }
  else {
    fprintf(filename,"ChainActive=No\n");
  }
  fprintf(filename,"\n");
  fprintf(filename,"[LastLoaded]\n");
  for(int i=0;i<MAX_LAST_ARRAYS;i++) {
    sprintf(temp,"Array%d",i);
    fprintf(filename,"%s=%s\n",temp,(const char *)array_last_filename[i]);
  }
  fprintf(filename,"\n");
  fclose(filename);
}


void SigHandler(int signo)
{
  pid_t pLocalPid;

  switch(signo) {
  case SIGCHLD:
    pLocalPid=waitpid(-1,NULL,WNOHANG);
    while(pLocalPid>0) {
      pLocalPid=waitpid(-1,NULL,WNOHANG);
    }
    ripper_running=false;
    signal(SIGCHLD,SigHandler);
    return;
  }
}


void MainWidget::BuildFileFilter()
{
  array_file_filter="Audio Files (*.wav";
  if((array_card->getCardInfo(array_play_card)->modelNumber()&0xF000)==
     0x6000) {
    array_file_filter+=" *.mp1";
    array_file_filter+=" *.mp2";
    array_file_filter+=" *.mp3";
  }
  if(array_play->formatSupported(WaveFile::Vorbis)) {
    array_file_filter+=" *.ogg";
  }
  array_file_filter+=")";
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  
  MainWidget *w=new MainWidget(NULL,"main");
  a.setMainWidget(w);
  w->setGeometry(QRect(QPoint(0,0),w->sizeHint()));
  w->show();
  return a.exec();
}


