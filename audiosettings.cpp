// audio_settings.cpp
//
// Common Audio Settings
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: audiosettings.cpp,v 1.1 2007/12/19 20:22:23 fredg Exp $
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

#include <audiosettings.h>


AudioSettings::AudioSettings()
{
}


AudioSettings::Format AudioSettings::format() const
{
  return set_format;
}


void AudioSettings::setFormat(AudioSettings::Format format)
{
  set_format=format;
}


unsigned AudioSettings::channels() const
{
  return set_channels;
}


void AudioSettings::setChannels(unsigned channels)
{
  set_channels=channels;
}


unsigned AudioSettings::sampleRate() const
{
  return set_sample_rate;
}


void AudioSettings::setSampleRate(unsigned rate)
{
  set_sample_rate=rate;
}


unsigned AudioSettings::bitRate() const
{
  return set_bit_rate;
}


void AudioSettings::setBitRate(unsigned rate)
{
  set_bit_rate=rate;
}
