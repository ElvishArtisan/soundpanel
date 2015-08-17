// soundpanel.h
//
// A Cart Wall Utility
//
//   (C) Copyright 2003 Fred Gleason <fredg@paravelsystems.com>
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


#ifndef SNDARRAY_H
#define SNDARRAY_H

#include <QtCore/QtCore>
#include <QtGui/QtGui>

#include <vector>

#include <soundcard.h>
#include <playstream.h>
#include <playmeter.h>
#include <transportbutton.h>
#include <pushbutton.h>
#include <audiosettings.h>
#include <panelbox.h>
#include <panel.h>
#include <defaults.h>
#include <array_button.h>
#include <edit_general.h>

//
// Global Variables
//
extern bool ripper_running;


class MainWidget : public QMainWindow
{
  Q_OBJECT
  public:
   MainWidget(QWidget *parent=0,const char *name=0);
   QSize sizeHint() const;
   QSizePolicy sizePolicy() const;

  private slots:
   void focusedData(int);
   void leftButtonMapperData(int);
   void activatedFileMenuData(int);
   void updateFileMenuData();
   void updateSettingsMenuData();
   void updateHelpMenuData();
   void newMenuData();
   void openMenuData();
   void saveMenuData();
   void saveAsMenuData();
   void propertiesMenuData();
   void generalSettingsMenuData();
   void devicesMenuData();
   void recordSettingsMenuData();
   void ripperSettingsMenuData();
   void editorSettingsMenuData();
   void aboutHelpMenuData();
   void updateButtonMenuData();
   void cutButtonMenuData();
   void copyButtonMenuData();
   void pasteButtonMenuData();
   void clearButtonMenuData();
   void propertiesButtonMenuData();
   void recordButtonMenuData();
   void ripButtonMenuData();
   void editButtonMenuData();
   void deleteButtonMenuData();
   void rightBoxData(QPoint);
   void updatePanelMenuData();
   void insertPanelMenuData();
   void deletePanelMenuData();
   void renamePanelMenuData();
   void pauseButtonData();
   void stopButtonData();
   void loopButtonData();
   void playButtonData();
   void searchButtonData();
   void chainButtonData();
   void playedData();
   void pausedData();
   void stoppedData();
   void positionData(int);
   void panelActivatedData(int);
   void meterData();
   void quitMainWidget();

  protected:
   void mousePressEvent(QMouseEvent *e);
   void keyPressEvent(QKeyEvent *e);
   void resizeEvent(QResizeEvent *e);
   void closeEvent(QCloseEvent *e);

  private:
   void rightButtonMapperData(int,int);
   void AddLastList(QString);
   void UpdateLastList();
   void SetHPIMixer();
   void UpdateButton(int,int);
   void SetCaption();
   void AssignButtons();
   bool LoadCut(QString search);
   bool PromptSave();
   void NewFile();
   bool LoadFile();
   bool SaveFile();
   void LoadConfig();
   void SaveConfig();
   void BuildFileFilter();
   std::vector<Panel> array_panels;
   SoundCard *array_card;
   PlayStream *array_play;
   PlayMeter *left_meter;
   PlayMeter *right_meter;
   QTimer *meter_timer;
   TransportButton *array_stop_button;
   TransportButton *array_loop_button;
   TransportButton *array_play_button;
   TransportButton *array_pause_button;
   QPushButton *array_search_button;
   PushButton *array_chain_button;
   AudioSettings array_audio_settings;
   QMenu *array_file_menu;
   QMenu *array_settings_menu;
   QMenu *array_help_menu;
   QMenu *array_menu;
   QMenu *array_panel_menu;
   PanelBox *array_panel_box;
   QString array_filename;
   QString array_last_filename[5];
   int array_last_items;
   QString array_configname;
   QString array_title;
   QString array_alsa_playdev;
   QString array_alsa_recdev;
   QString array_path;
   QString array_ripdev;
   QString array_editor;
   int array_panel;
   int array_panel_quantity;
   ArrayButton *array_button[BUTTON_ROWS][BUTTON_COLUMNS];
   QLabel *array_deck_label;
   QLabel *array_time_label;
   int array_deck_length;
   QString array_deck_wavefile;
   int array_deck_col;
   int array_deck_row;
   int array_deck_panel;
   int array_deck_id;
   int array_deck_offset;
   bool array_cc;
   int current_row;
   int current_col;
   int current_panel;
   int current_id;
   int current_offset;
   int right_row;
   int right_col;
   int right_offset;
   double current_samplerate;
   double current_samplelength;
   QString array_clip_legend;
   QString array_clip_wavefile;
   int array_clip_length;
   QColor array_clip_color;
   bool array_modified;
   bool array_blank;
   SoundCard::SourceNode array_input_type;
   int array_rec_card;
   int array_rec_port;
   int array_play_card;
   int array_play_port;
   int array_paranoia_level;
   QString array_cddb_server;
   bool array_normalize;
   int array_normal_level;
   bool array_looping;
   bool array_stopping;
   int array_focus_x;
   int array_focus_y;
   int array_rec_level;
   int array_play_level;
   int size_x;
   int size_y;
   bool array_chain_active;
   QString array_startup_filename;
   EditGeneral::StartupArray array_startup_mode;
   SoundCard::ClockSource clock_source;
   QPixmap *soundpanel_map;
};


#endif 
