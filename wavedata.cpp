//   wavedata.cpp
//
//   A Container Class for Audio Meta Data.
//
//   (C) Copyright 2002-2016 Fred Gleason <fredg@paravelsystems.com>
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
//

#include "wavedata.h"

WaveData::WaveData()
{
  clear();
}


bool WaveData::metadataFound() const
{
  return data_metadata_found;
}


void WaveData::setMetadataFound(bool state)
{
  data_metadata_found=state;
}


QString WaveData::title() const
{
  return data_title;
}


void WaveData::setTitle(const QString &str)
{
  data_title=str;
}


QString WaveData::artist() const
{
  return data_artist;
}


void WaveData::setArtist(const QString &str)
{
  data_artist=str;
}


QString WaveData::album() const
{
  return data_album;
}


void WaveData::setAlbum(const QString &str)
{
  data_album=str;
}


QString WaveData::label() const
{
  return data_label;
}


void WaveData::setLabel(const QString &str)
{
  data_label=str;
}


int WaveData::releaseYear() const
{
  return data_release_year;
}


void WaveData::setReleaseYear(int year)
{
  data_release_year=year;
}


QString WaveData::client() const
{
  return data_client;
}


void WaveData::setClient(const QString &str)
{
  data_client=str;
}


QString WaveData::agency() const
{
  return data_agency;
}


void WaveData::setAgency(const QString &str)
{
  data_agency=str;
}


QString WaveData::composer() const
{
  return data_composer;
}


void WaveData::setComposer(const QString &str)
{
  data_composer=str;
}


QString WaveData::publisher() const
{
  return data_publisher;
}


void WaveData::setPublisher(const QString &str)
{
  data_publisher=str;
}


QString WaveData::licensingOrganization() const
{
  return data_licensing_organization;
}


void WaveData::setLicensingOrganization(const QString &str)
{
  data_licensing_organization=str;
}


QString WaveData::copyrightNotice() const
{
  return data_copyright_notice;
}


void WaveData::setCopyrightNotice(const QString &str)
{
  data_copyright_notice=str;
}


QString WaveData::cutId() const
{
  return data_cut_id;
}


void WaveData::setCutId(const QString &str)
{
  data_cut_id=str;
}


QString WaveData::classification() const
{
  return data_classification;
}


void WaveData::setClassification(const QString &str)
{
  data_classification=str;
}


QString WaveData::category() const
{
  return data_category;
}


void WaveData::setCategory(const QString &str)
{
  data_category=str;
}


QString WaveData::url() const
{
  return data_url;
}


void WaveData::setUrl(const QString &str)
{
  data_url=str;
}


QString WaveData::tagText() const
{
  return data_tag_text;
}


void WaveData::setTagText(const QString &str)
{
  data_tag_text=str;
}


QString WaveData::description() const
{
  return data_description;
}


void WaveData::setDescription(const QString &str)
{
  data_description=str;
}


QString WaveData::originator() const
{
  return data_originator;
}


void WaveData::setOriginator(const QString &str)
{
  data_originator=str;
}


QString WaveData::originatorReference() const
{
  return data_originator_reference;
}


void WaveData::setOriginatorReference(const QString &str)
{
  data_originator_reference=str;
}


QString WaveData::codingHistory() const
{
  return data_coding_history;
}


void WaveData::setCodingHistory(const QString &str)
{
  data_coding_history=str;
}


QString WaveData::userDefined() const
{
  return data_user_defined;
}


void WaveData::setUserDefined(const QString &str)
{
  data_user_defined=str;
}


QString WaveData::isrc() const
{
  return data_isrc;
}


void WaveData::setIsrc(const QString &str)
{
  data_isrc=str;
}


QString WaveData::mcn() const
{
  return data_mcn;
}


void WaveData::setMcn(const QString &str)
{
  data_mcn=str;
}


QString WaveData::outCue() const
{
  return data_out_cue;
}


void WaveData::setOutCue(const QString &str)
{
  data_out_cue=str;
}


WaveData::EndType WaveData::endType() const
{
  return data_end_type;
}


void WaveData::setEndType(WaveData::EndType type)
{
  data_end_type=type;
}


int WaveData::introLength() const
{
  return data_intro_length;
}


void WaveData::setIntroLength(int len)
{
  data_intro_length=len;
}


int WaveData::segueLength() const
{
  return data_segue_length;
}


void WaveData::setSegueLength(int len)
{
  data_segue_length=len;
}


int WaveData::fullLength() const
{
  return data_full_length;
}


void WaveData::setFullLength(int len)
{
  data_full_length=len;
}


int WaveData::beatsPerMinute() const
{
  return data_beats_per_minute;
}


void WaveData::setBeatsPerMinute(int bpm)
{
  data_beats_per_minute=bpm;
}


QString WaveData::tmciSongId() const
{
  return data_tmci_song_id;
}


void WaveData::setTmciSongId(const QString &str)
{
  data_tmci_song_id=str;
}


QDate WaveData::startDate() const
{
  return data_start_date;
}


void WaveData::setStartDate(const QDate &date)
{
  data_start_date=date;
}


QTime WaveData::startTime() const
{
  return data_start_time;
}


void WaveData::setStartTime(const QTime &time)
{
  data_start_time=time;
}


QDate WaveData::originationDate() const
{
  return data_origination_date;
}


void WaveData::setOriginationDate(const QDate &date)
{
  data_origination_date=date;
}


QTime WaveData::originationTime() const
{
  return data_origination_time;
}


void WaveData::setOriginationTime(const QTime &time)
{
  data_origination_time=time;
}


QDate WaveData::endDate() const
{
  return data_end_date;
}


void WaveData::setEndDate(const QDate &date)
{
  data_end_date=date;
}


QTime WaveData::endTime() const
{
  return data_end_time;
}


void WaveData::setEndTime(const QTime &time)
{
  data_end_time=time;
}


void WaveData::clear()
{
  data_metadata_found=false;
  data_title="";
  data_artist="";
  data_album="";
  data_label="";
  data_release_year=0;
  data_client="";
  data_agency="";
  data_composer="";
  data_publisher="";
  data_licensing_organization="";
  data_copyright_notice="";
  data_user_defined="";
  data_cut_id="";
  data_classification="";
  data_category="";
  data_url="";
  data_tag_text="";
  data_description="";
  data_originator="";
  data_originator_reference="";
  data_isrc="";
  data_mcn="";
  data_out_cue="";
  data_end_type=WaveData::UnknownEnd;
  data_intro_length=0;
  data_segue_length=0;
  data_full_length=0;
  data_beats_per_minute=0;
  data_tmci_song_id="";
  data_origination_date=QDate();
  data_origination_time=QTime();
  data_start_date=QDate();
  data_start_time=QTime();
  data_end_date=QDate();
  data_end_time=QTime();
}
