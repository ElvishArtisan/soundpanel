//   recordstream.cpp
//
//   A class for recording Microsoft WAV files.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: recordstream.cpp,v 1.3 2012/09/10 11:57:30 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <recordstream.h>


RecordStream::RecordStream(SoundCard *card,
			     QWidget *parent,const char *name) 
  :QObject(parent,name),WaveFile()
{ 
  if(getenv(DEBUG_VAR)==NULL) {
    debug=false;
  }
  else {
    debug=true;
    printf("RecordStream: debugging enabled\n");
  }
  if(getenv(XRUN_VAR)==NULL) {
    xrun=false;
  }
  else {
    xrun=true;
    printf("RecordStream: xrun notification enabled\n");
  }

  sound_card=card;

  card_number=-1;
  stream_number=-1;
  is_ready=false;
  is_recording=false;
  is_paused=false;
  stopping=false;
  record_started=false;
  record_length=0;
  is_open=false;
  pdata=NULL;

  clock=new QTimer(this,"clock");
  connect(clock,SIGNAL(timeout()),this,SLOT(tickClock()));

  length_timer=new QTimer(this,"length_timer");
  connect(length_timer,SIGNAL(timeout()),this,SLOT(pause()));
}


RecordStream::~RecordStream()
{
  if(pdata!=NULL) {
    delete pdata;
  }
} 


QString RecordStream::errorString(RecordStream::Error err)
{
  QString str;

  switch(err) {
      case RecordStream::Ok:
	return QString(tr("Ok"));
	break;

      case RecordStream::NoFile:
	return QString(tr("Unable to create/open file"));
	break;

      case RecordStream::NoStream:
	return QString(tr("Input stream unavailable"));
	break;

      case RecordStream::AlreadyOpen:
	return QString(tr("Stream is already open"));
	break;

      default:
	str=QString(tr("Unknown Error:"));
	return QString().sprintf("%s %d\n",(const char *)str,err);
	break;
  }
}


RecordStream::Error RecordStream::createWave()
{
  if(is_open) {
    return RecordStream::AlreadyOpen;
  }
  if(!WaveFile::createWave()) {
    return RecordStream::NoFile;
  }
  if(!GetStream()) {
    closeWave();
    return RecordStream::NoStream;
  }
  is_open=true;
  return RecordStream::Ok;
}


RecordStream::Error RecordStream::createWave(QString filename)
{
  if(is_open) {
    return RecordStream::AlreadyOpen;
  }
  setName(filename);
  return createWave();
}


void RecordStream::closeWave()
{
  if(!is_open) {
    return;
  }
  if(getState()!=RecordStream::Stopped) {
    stop();
  }
  WaveFile::closeWave(samples_recorded);
  FreeStream();
  is_open=false;
}


bool RecordStream::formatSupported(WaveFile::Format format)
{
#if HPI_VER < HPI_VERSION_CONSTRUCTOR(3L,10,0)
  HPI_FORMAT hformat;
#else
  struct hpi_format hformat;
#endif
  hpi_handle_t histream;
  bool found=false;

  if(card_number<0) {
    return false;
  }
  if(format==WaveFile::Vorbis) {
#ifdef HAVE_VORBIS
    return true;
#endif  // HAVE_VORBIS
    return false;
  }
  if(!is_open) {
    for(int i=0;i<sound_card->getCardInputStreams(card_number);i++) {
      if(HPI_InStreamOpen(NULL,card_number,i,&histream)==0) {
	found=true;
	break;
      }
    }
    if(!found) {
      return false;
    }
  }
  else {
    histream=hpi_stream;
  }
  switch(format) {
      case WaveFile::Pcm8:
	HPICall(HPI_FormatCreate(&hformat,getChannels(),
				 HPI_FORMAT_PCM8_UNSIGNED,
				 getSamplesPerSec(),getHeadBitRate(),0));
	state=HPI_InStreamQueryFormat(NULL,histream,&hformat);
	break;

      case WaveFile::Pcm16:
	HPICall(HPI_FormatCreate(&hformat,getChannels(),
				 HPI_FORMAT_PCM16_SIGNED,
				 getSamplesPerSec(),getHeadBitRate(),0));
	state=HPI_InStreamQueryFormat(NULL,histream,&hformat);
	break;

      case WaveFile::MpegL1:
	HPICall(HPI_FormatCreate(&hformat,getChannels(),HPI_FORMAT_MPEG_L1,
				  getSamplesPerSec(),getHeadBitRate(),0));
	state=HPI_InStreamQueryFormat(NULL,histream,&hformat);
	break;

      case WaveFile::MpegL2:
	HPICall(HPI_FormatCreate(&hformat,getChannels(),HPI_FORMAT_MPEG_L2,
				 getSamplesPerSec(),getHeadBitRate(),0));
	state=HPI_InStreamQueryFormat(NULL,histream,&hformat);
	break;

      case WaveFile::MpegL3:
	HPICall(HPI_FormatCreate(&hformat,getChannels(),HPI_FORMAT_MPEG_L3,
				 getSamplesPerSec(),getHeadBitRate(),0));
	state=HPI_InStreamQueryFormat(NULL,histream,&hformat);
	break;

      default:
	state=1;
	break;
  }
  if(!is_open) {
    HPICall(HPI_InStreamClose(NULL,histream));
  }
  if(state!=0) {
    return false;
  }
  return true;
}


bool RecordStream::formatSupported()
{
  switch(getFormatTag()) {
      case WAVE_FORMAT_PCM:
	switch(getBitsPerSample()) {
	    case 8:
	      return formatSupported(WaveFile::Pcm8);
	      break;

	    case 16:
	      return formatSupported(WaveFile::Pcm16);
	      break;

	    default:
	      return false;
	}
	break;

      case WAVE_FORMAT_MPEG:
	switch(getHeadLayer()) {
	    case 1:
	      return formatSupported(WaveFile::MpegL1);
	      break;

	    case 2:
	      return formatSupported(WaveFile::MpegL2);
	      break;

	    case 3:
	      return formatSupported(WaveFile::MpegL3);
	      break;

	    default:
	      return false;
	}
	break;

      default:
	return false;
  }
}


int RecordStream::getCard() const
{
  return card_number;
}


void RecordStream::setCard(int card)
{
  if(!is_recording) {
    card_number=card;
    if(debug) {
      printf("RecordStream: using card %d\n",card_number);
    }
  }
}


int RecordStream::getStream() const
{
  return stream_number;
}


void RecordStream::setStream(int stream)
{
  stream_number=stream;
}


bool RecordStream::haveInputVOX() const
{
  return sound_card->haveInputStreamVOX(card_number,stream_number);
}


RecordStream::RecordState RecordStream::getState()
{
  if(is_recording) {
    if(record_started) {
      return RecordStream::RecordStarted;
    }
    return RecordStream::Recording;
  }
  if(is_paused) {
    return RecordStream::Paused;
  }
  if(is_ready) {
    return RecordStream::RecordReady;
  }
  return RecordStream::Stopped;
}


int RecordStream::getPosition() const
{
  if((!is_recording)&&(!is_ready)&&(!is_paused)) {
    return 0;
  }
  return samples_recorded;
}


bool RecordStream::recordReady()
{
  uint16_t hpi_error=0;
  char hpi_text[200];

  if(debug) {
    printf("RecordStream: received recordReady()\n");
  }
  if(!is_open) {
    return false;
  }
  if((!is_recording)&&(!is_paused)) {
    resetWave();
    if(HPI_InStreamGetInfoEx(NULL,hpi_stream,
			     &state,&buffer_size,&data_recorded,
			     &samples_recorded,&reserved)!=0) {
      if(debug) {
	printf("RecordStream: HPI_InStreamGetInfoEx() failed\n");
      }
      return false;
    }
    fragment_size=buffer_size/4;
    if(fragment_size>192000) {  // ALSA Compatibility Limitation
      fragment_size=192000;
    }
    fragment_time=(1000*fragment_size)/(getAvgBytesPerSec());
    if(pdata!=NULL) {
      delete pdata;
    }
    pdata=(uint8_t *)malloc(fragment_size);
    if(pdata==NULL) {
      if(debug) {
	printf("RecordStream: couldn't allocate buffer\n");
      }
      return false;
    }
    switch(getFormatTag()) {
	case WAVE_FORMAT_PCM:
	  if(debug) {
	    printf("RecordStream: using PCM%d format\n",
		   getBitsPerSample());
	  }
	  switch(getBitsPerSample()) {
	      case 8:
		HPICall(HPI_FormatCreate(&format,getChannels(),
					 HPI_FORMAT_PCM8_UNSIGNED,
					 getSamplesPerSec(),0,0));
		break;
	      case 16:
		HPICall(HPI_FormatCreate(&format,getChannels(),
					 HPI_FORMAT_PCM16_SIGNED,
					 getSamplesPerSec(),0,0));
		break;
	      case 32:
		HPICall(HPI_FormatCreate(&format,getChannels(),
					 HPI_FORMAT_PCM32_SIGNED,
					 getSamplesPerSec(),0,0));
		break;
	      default:
		if(debug) {
		  printf("RecordStream: unsupported sample size\n");
		}
		return false;
	  }
	  break;

	case WAVE_FORMAT_MPEG:
	  if(debug) {
	    printf("RecordStream: using MPEG-1 Layer %d\n",getHeadLayer());
	  }
	  switch(getHeadLayer()) {
	      case 1:
		HPICall(HPI_FormatCreate(&format,getChannels(),
					 HPI_FORMAT_MPEG_L1,getSamplesPerSec(),
					 getHeadBitRate(),getHeadFlags()));
		break;
	      case 2:
		HPICall(HPI_FormatCreate(&format,getChannels(),
					 HPI_FORMAT_MPEG_L2,getSamplesPerSec(),
					 getHeadBitRate(),getHeadFlags()));
		break;
	      case 3:
		HPICall(HPI_FormatCreate(&format,getChannels(),
					 HPI_FORMAT_MPEG_L3,getSamplesPerSec(),
					 getHeadBitRate(),getHeadFlags()));
		break;
	      default:
		HPICall(HPI_AdapterClose(NULL,card_number));
		if(debug) {
		  printf("RecordStream: invalid MPEG-1 layer\n");
		}
		return false;
	  }
	  if(getMextChunk()) {
	    setMextHomogenous(true);
	    setMextPaddingUsed(false);
	    setMextHackedBitRate(true);
	    setMextFreeFormat(false);
	    setMextFrameSize(144*getHeadBitRate()/getSamplesPerSec());
	    setMextAncillaryLength(5);
	    setMextLeftEnergyPresent(true);
	    if(getChannels()>1) {
	      setMextRightEnergyPresent(true);
	    }
	    else {
	      setMextRightEnergyPresent(false);
	    }
	    setMextPrivateDataPresent(false);
	  }
	  break;

	case WAVE_FORMAT_VORBIS:
	  if(debug) {
	    printf("RecordStream: using OggVorbis\n");
	  }
	  HPICall(HPI_FormatCreate(&format,getChannels(),
				   HPI_FORMAT_PCM16_SIGNED,getSamplesPerSec(),
				   0,0));
	  break;

	default:
	  if(debug) {
	    printf("RecordStream: invalid format tag\n");
	  }
	  return false;
	  break;
    }
    if((hpi_error=HPI_InStreamQueryFormat(NULL,hpi_stream,
			       &format))!=0) {
      if(debug) {
	HPI_GetErrorText(hpi_error,hpi_text);
	printf("Num: %d\n",hpi_error);
	printf("RecordStream: %s\n",hpi_text);
      }
      return false;
    }
  }
#if HPI_VER < 0x00030500
  HPICall(HPI_DataCreate(&hpi_data,&format,pdata,fragment_size));
#endif
  HPICall(HPI_InStreamSetFormat(NULL,hpi_stream,&format));
  HPICall(HPI_InStreamStart(NULL,hpi_stream));
//  clock->start(2*fragment_time/3);
  clock->start(100);
  is_ready=true;
  is_recording=false;
  is_paused=false;
  stopping=false;
  emit isStopped(false);
  emit ready();
  emit stateChanged(card_number,stream_number,1);  // RecordReady
  if(debug) {
    printf("RecordStream: emitted isStopped(false)\n");
    printf("RecordStream: emitted ready()\n");
    printf("RecordStream: emitted stateChanged(%d,%d,RecordStream::RecordReady)\n",card_number,stream_number);
  }

  return true;
}


void RecordStream::record()
{
  if(debug) {
    printf("RecordStream: received record()\n");
  }
  if(!is_open) {
    return;
  }
  if(!is_ready) {
    recordReady();
  }
  record_started=false;
  HPICall(HPI_InStreamReset(NULL,hpi_stream));
  HPICall(HPI_InStreamStart(NULL,hpi_stream));
  is_recording=true;
  is_paused=false;
  emit isStopped(false);
  emit recording();
  emit stateChanged(card_number,stream_number,0);  // Recording
  if(debug) {
    printf("RecordStream: emitted isStopped(false)\n");
    printf("RecordStream: emitted recording()\n");
    printf("RecordStream: emitted stateChanged(%d,%d,RecordStream::Recording)\n",card_number,stream_number);
  }
  tickClock();
}


void RecordStream::pause()
{
  if(debug) {
    printf("RecordStream: received pause()\n");
  }
  if(!is_recording) {
    return;
  }
  HPICall(HPI_InStreamStop(NULL,hpi_stream));
  tickClock();
  HPICall(HPI_InStreamGetInfoEx(NULL,hpi_stream,
				&state,&buffer_size,&data_recorded,
				&samples_recorded,&reserved));
  is_recording=false;
  is_paused=true;
  HPICall(HPI_InStreamStart(NULL,hpi_stream));
  emit paused();
  emit stateChanged(card_number,stream_number,2);  // Paused
  if(debug) {
    printf("RecordStream: emitted paused()\n");
    printf("RecordStream: emitted stateChanged(%d,%d,RecordStream::Paused)\n",card_number,stream_number);
  }
}


void RecordStream::stop()
{
  if(debug) {
    printf("RecordStream: received stop()\n");
  }
  if(is_ready|is_recording|is_paused) {
    HPICall(HPI_InStreamStop(NULL,hpi_stream));
    tickClock();
    clock->stop();
    is_recording=false;
    is_paused=false;
    is_ready=false;
    if(pdata!=NULL) {
      delete pdata;
      pdata=NULL;
    }
    emit isStopped(true);
    emit stopped();
    emit stateChanged(card_number,stream_number,RecordStream::Stopped);
    emit position(0);
    if(debug) {
      printf("RecordStream: emitted isStopped(true)\n");
      printf("RecordStream: emitted stopped()\n");
      printf("RecordStream: emitted stateChanged(%d,%d,RecordStream::Stopped)\n",card_number,stream_number);
      printf("RecordStream: emitted position(0)\n");
    }
  }
}


void RecordStream::setInputVOX(int gain)
{
  sound_card->setInputStreamVOX(card_number,stream_number,gain);
}


void RecordStream::setRecordLength(int length)
{
  record_length=length;
}


void RecordStream::tickClock()
{
  HPICall(HPI_InStreamGetInfoEx(NULL,hpi_stream,
				&state,&buffer_size,&data_recorded,
				&samples_recorded,&reserved));
  if((!record_started)&&(is_recording)) {
    if(samples_recorded>0) {
      if(record_length>0) {
        length_timer->start(record_length,true);
      }
      emit recordStart();
      emit stateChanged(card_number,stream_number,4);  // RecordStarted
      if(debug) {
	printf("RecordStream: emitted recordStart()\n");
	printf("RecordStream: emitted stateChanged(%d,%d,RecordStream::RecordStarted)\n",card_number,stream_number);
      }
      record_started=true;
    }
  }
  while(data_recorded>fragment_size) {
#if HPI_VER > 0x00030500
    HPICall(HPI_InStreamReadBuf(NULL,hpi_stream,pdata,fragment_size));
#else
    HPICall(HPI_InStreamRead(NULL,hpi_stream,&hpi_data));
#endif
    if(is_recording) {
      writeWave(pdata,fragment_size);
    }
    HPICall(HPI_InStreamGetInfoEx(NULL,hpi_stream,
				  &state,&buffer_size,&data_recorded,
				  &samples_recorded,&reserved));
  }
  if(state==HPI_STATE_STOPPED) {
#if HPI_VER > 0x00030500
    HPICall(HPI_InStreamReadBuf(NULL,hpi_stream,pdata,data_recorded));
#else
    HPICall(HPI_DataCreate(&hpi_data,&format,pdata,data_recorded));
    HPICall(HPI_InStreamRead(NULL,hpi_stream,&hpi_data)_;
#endif
    if(is_recording) {
      writeWave(pdata,data_recorded);
    }
  }
  emit position(samples_recorded);
  if(debug) {
    printf("RecordStream: emitted position(%u)\n",
	   (unsigned)samples_recorded);
  }
}


bool RecordStream::GetStream()
{
  uint16_t hpi_err;
  char hpi_text[100];

  if((hpi_err=
      HPI_InStreamOpen(NULL,card_number,stream_number,&hpi_stream))!=0) {
    if(debug) {
      HPI_GetErrorText(hpi_err,hpi_text);
      fprintf(stderr,"*** HPI Error: %s ***\n",hpi_text);
    }
    return false;
  }
  return true;
}


void RecordStream::FreeStream()
{
  HPICall(HPI_InStreamClose(NULL,hpi_stream));
}


bool RecordStream::HPICall(uint16_t hpi_err) const
{
  char text[1024];

  if(hpi_err!=0) {
    HPI_GetErrorText(hpi_err,text);
    fprintf(stderr,"RecordStream::HPI Error: %s\n",text);
  }
  return hpi_err==0;
}
