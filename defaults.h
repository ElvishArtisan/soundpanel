// defaults.h
//
// Default Settings for SoundPanel.
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


#ifndef DEFAULTS_H
#define DEFAULTS_H

//
// Application Settings
//
#define BUTTON_PANELS 1
#define BUTTON_COLUMNS 9
#define BUTTON_ROWS 10
#define X_MARGIN 10
#define Y_MARGIN 55
#define PLAYING_BUTTON_COLOR Qt::red
#define DECK_LABEL_COLOR Qt::white
#define CHAIN_FLASH_COLOR Qt::blue
#define MAX_HPI_CARDS 8
#define MAX_HPI_STREAMS 8
#define MAX_HPI_PORTS 8
#define MAX_LAST_ARRAYS 5
#define RIPPER_BAR_INTERVAL 500
#define RIPPER_CDDB_USER "soundpanel"
#define MAX_LINE 512

//
// Application Defaults
//
#define DEFAULT_POSITION_X 0
#define DEFAULT_POSITION_Y 0
#define DEFAULT_SIZE_X 800
#define DEFAULT_SIZE_Y 610
#define DEFAULT_CONFIG_FILE ".soundpanelrc"
#define DEFAULT_ARRAY_TITLE "untitled"
#define DEFAULT_ALSA_DEVICE "plughw:0,0"
#define DEFAULT_INPUT_TYPE "Analog"
#define DEFAULT_HPI_CARD 0
#define DEFAULT_HPI_STREAM 0
#define DEFAULT_HPI_PORT 0
#define DEFAULT_HPI_LEVEL 400
#define DEFAULT_RIPPER_DEVICE "/dev/cdrom"
#define DEFAULT_AUDIO_FORMAT 0
#define DEFAULT_AUDIO_CHANNELS 2
#define DEFAULT_AUDIO_SAMPRATE 44100
#define DEFAULT_AUDIO_BITRATE 192000
#define DEFAULT_PARANOIA_LEVEL 0
#define DEFAULT_CDDB_SERVER "freedb.freedb.org"
#define DEFAULT_NORMAL_LEVEL -1600
#define DEFAULT_EDITOR_COMMAND ""
#define DEFAULT_NORMALIZE true
#define DEFAULT_CHAIN_ACTIVE false
#define DEFAULT_ARRAY_STARTUP_MODE "None"
#define DEFAULT_CLOCK_SOURCE SoundCard::Internal


#endif 
