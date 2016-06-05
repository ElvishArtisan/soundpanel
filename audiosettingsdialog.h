// audio_settings_dialog.h
//
// Edit Rivendell Audio Settings
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

#ifndef AUDIO_SETTINGS_DIALOG_H
#define AUDIO_SETTINGS_DIALOG_H

#include <QComboBox>
#include <QDialog>

#include "audiosettings.h"

class AudioSettingsDialog : public QDialog
{
  Q_OBJECT
  public:
  /**
   * Create an AudioSettingsDialog object
   * @param settings = The AudioSettings object from which to take the 
   * default settings, and in which to return the response.
   * @param mpeg = Set true if MPEG formats should be offered, otherwise false.
   **/
   AudioSettingsDialog(AudioSettings *settings,bool mpeg=true,
			QWidget *parent=0,const char *name=0);

  /**
   * Destroys an AudioSettingsDialog object
   **/
   ~AudioSettingsDialog();

  /**
   * Reimplemented from QWidget, the API is not affected.
   **/
   QSize sizeHint() const;

  /**
   * Reimplemented from QWidget, the API is not affected.
   **/
   QSizePolicy sizePolicy() const;

  private slots:
   void formatData(int);
   void okData();
   void cancelData();

  private:
   void ShowBitRates(AudioSettings::Format fmt,int rate);
   AudioSettings::Format ReadFormat();
   AudioSettings *lib_lib;
   QComboBox *lib_format_box;
   QComboBox *lib_channels_box;
   QComboBox *lib_bitrate_box;
   QComboBox *lib_samprate_box;
};


#endif

