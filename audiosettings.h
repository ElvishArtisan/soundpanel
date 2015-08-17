// audio_settings.h
//
// A container class for audio settings.
//
//   (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: audiosettings.h,v 1.1 2007/12/19 20:22:23 fredg Exp $
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

#ifndef AUDIO_SETTINGS_H
#define AUDIO_SETTINGS_H


/**
 * @short A container class for common audio format settings.
 * @author Fred Gleason <fredg@paravelsystems.com>
 *
 * AudioSettings provides a container class for commonly-encountered
 * audio format settings.
 **/   
class AudioSettings
{
  public:
 /**
  * The audio encoding format.
  * Pcm16 = Sixteen bit, little-endian, PCM
  * Layer1 = MPEG-1 Layer 1
  * Layer2 = MPEG-1 Layer 2
  * Layer3 = MPEG-1 Layer 3
  **/
  enum Format {Pcm16=0,Layer1=1,Layer2=2,Layer3=3,OggVorbis=4};

 /**
  * Create a AudioSettings object.
  **/
  AudioSettings();

 /** 
  * Returns the encoding format
  **/
  AudioSettings::Format format() const;

 /**
  * Set the recording format.
  * @param format = The format.
  **/
  void setFormat(AudioSettings::Format format);

 /**
  * Returns the number of audio channels.
  **/
  unsigned channels() const;

 /**
  * Set the number of audio channels.
  * @param channels = The number of channels.
  **/
  void setChannels(unsigned channels);

 /**
  * Returns the sampling rate, in samples per second.
  **/
  unsigned sampleRate() const;

 /**
  * Set the sampling rate.
  * @param rate = The sample rate, in samples per second.
  **/
  void setSampleRate(unsigned rate);

 /**
  * Returns the bit rate, in bits per second.
  **/
  unsigned bitRate() const;

 /**
  * Set the bit rate.
  * @param rate = The bit rate, in bits per second.
  **/
  void setBitRate(unsigned rate);

  private:
   AudioSettings::Format set_format;
   unsigned set_channels;
   unsigned set_sample_rate;
   unsigned set_bit_rate;
};


#endif
