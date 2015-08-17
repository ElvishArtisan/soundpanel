//   soundcard.cpp
//
//   The audio card subsystem for the HPI Library.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: soundcard.cpp,v 1.5 2012/09/10 11:57:30 cvs Exp $
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

#include <unistd.h>

#include <soundcard.h>

SoundCard::SoundCard(QObject *parent,const char *name)
  : QObject(parent,name)
{
  card_quantity=0;
  fade_type=SoundCard::Log;
  for(int i=0;i<HPI_MAX_ADAPTERS;i++) {
    card_index[i]=0;
    card_input_streams[i]=0;
    card_output_streams[i]=0;
    card_input_ports[i]=0;
    card_output_ports[i]=0;
    input_mux_type[i]=false;
    timescale_support[i]=false;
    for(int j=0;j<HPI_MAX_NODES;j++) {
      input_port_level[i][j]=false;
      output_port_level[i][j]=false;
      input_port_meter[i][j]=false;
      output_port_meter[i][j]=false;
      input_port_mux[i][j]=false;
      input_port_mux_type[i][j][0]=false;
      input_port_mux_type[i][j][1]=false;
      input_mux_index[i][j][0]=0;
      input_mux_index[i][j][1]=0;
      input_port_aesebu[i][j]=false;
      input_port_aesebu_error[i][j]=false;
      for(int k=0;k<HPI_MAX_STREAMS;k++) {
	input_stream_volume[i][k][j]=false;
	output_stream_volume[i][k][j]=false;
      }
      for(int k=0;k<HPI_MAX_NODES;k++) {
	passthrough_port_volume[i][j][k]=false;
      }
    }
    for(int j=0;j<HPI_MAX_STREAMS;j++) {
      input_stream_meter[i][j]=false;
      output_stream_meter[i][j]=false;
      input_stream_mode[i][j]=false;
      output_stream_mode[i][j]=false;
      input_stream_vox[i][j]=false;
      input_stream_mux[i][j]=false;
    }
  }
  if(HPI_SubSysCreate()==NULL) {
    return;
  }
  HPIProbe();
}


SoundCard::~SoundCard()
{
  HPI_SubSysFree(NULL);
}


SoundCard::Driver SoundCard::driver() const
{
  return SoundCard::Hpi;
}


int SoundCard::getCardQuantity() const
{
  return card_quantity;
}


int SoundCard::getCardInputStreams(int card) const
{
  return card_input_streams[card];
}


int SoundCard::getCardOutputStreams(int card) const
{
  return card_output_streams[card];
}


int SoundCard::getCardInputPorts(int card) const
{
  return card_input_ports[card];
}


int SoundCard::getCardOutputPorts(int card) const
{
  return card_output_ports[card];
}


const Information *SoundCard::getCardInfo(int card) const
{
  return &hpi_info[card];
}


QString SoundCard::getCardDescription(int card) const
{
  return card_description[card];
}


QString SoundCard::getInputStreamDescription(int card,int stream) const
{
  return input_stream_description[card][stream];
}


QString SoundCard::getOutputStreamDescription(int card,int stream) const
{
  return output_stream_description[card][stream];
}


QString SoundCard::getInputPortDescription(int card,int port) const
{
  return input_port_description[card][port];
}


QString SoundCard::getOutputPortDescription(int card,int port) const
{
  return output_port_description[card][port];
}


void SoundCard::setClockSource(int card,SoundCard::ClockSource src)
{
  /*
  switch(src) {
      case SoundCard::Internal:
	HPICall(HPI_SampleClock_SetSource(NULL,clock_source_control[card],
					  HPI_SAMPLECLOCK_SOURCE_LOCAL));
	break;
      case SoundCard::AesEbu:
      case SoundCard::SpDiff:
	HPICall(HPI_SampleClock_SetSource(NULL,clock_source_control[card],
					  HPI_SAMPLECLOCK_SOURCE_AESEBU_SYNC));
	break;
      case SoundCard::WordClock:
	HPICall(HPI_SampleClock_SetSource(NULL,clock_source_control[card],
					  HPI_SAMPLECLOCK_SOURCE_WORD));
	break;
  }
  */
}


bool SoundCard::haveTimescaling(int card) const
{
  return timescale_support[card];
}


bool SoundCard::haveInputVolume(int card,int stream,int port) const
{
  return input_stream_volume[card][stream][port];
}


bool SoundCard::haveOutputVolume(int card,int stream,int port) const
{
  return output_stream_volume[card][stream][port];
}


bool SoundCard::haveInputLevel(int card,int port) const
{
  return input_port_level[card][port];
}


bool SoundCard::haveOutputLevel(int card,int port) const
{
  return output_port_level[card][port];
}


bool SoundCard::haveInputStreamVOX(int card,int stream) const
{
  return input_stream_vox[card][stream];
}


SoundCard::SourceNode SoundCard::getInputPortMux(int card,int port)
{
  uint16_t type;
  uint16_t index;

  HPICall(HPI_Multiplexer_GetSource(NULL,input_mux_control[card][port],
				    &type,&index));
  return (SoundCard::SourceNode)type;
}


bool SoundCard::setInputPortMux(int card,int port,SoundCard::SourceNode node)
{
  uint16_t hpi_error;

  switch(node) {
      case SoundCard::LineIn:
	if(HPI_Multiplexer_SetSource(NULL,
				     input_mux_control[card][port],
				     node,0)!=0) {
	  return false;
	}
	break;
      case SoundCard::AesEbuIn:
	if((hpi_error=
	    HPI_Multiplexer_SetSource(NULL,
				      input_mux_control[card][port],node,
				    input_mux_index[card][port][1]))!=0) {
	  return false;
	}
	break;
      default:
	return false;
	break;
  }
  return true;
}


unsigned short SoundCard::getInputPortError(int card,int port)
{
  uint16_t error_word=0;

  if(input_port_aesebu[card][port]) {
    HPICall(HPI_AESEBU_Receiver_GetErrorStatus(NULL,
	       			       input_port_aesebu_control[card][port],
					       &error_word));
  }
  return error_word;
}


SoundCard::FadeProfile SoundCard::getFadeProfile() const
{
  return fade_type;
}


void SoundCard::setFadeProfile(SoundCard::FadeProfile profile)
{
  fade_type=profile;
  switch(fade_type) {
      case SoundCard::Linear:
	hpi_fade_type=HPI_VOLUME_AUTOFADE_LINEAR;
	break;
      case SoundCard::Log:
	hpi_fade_type=HPI_VOLUME_AUTOFADE_LOG;
	break;
  }
}


bool SoundCard::haveInputStreamMeter(int card,int stream) const
{
  return input_stream_meter[card][stream];
}


bool SoundCard::haveInputPortMeter(int card,int port) const
{
  return input_stream_meter[card][port];
}


bool SoundCard::haveOutputStreamMeter(int card,int stream) const
{
  return output_stream_meter[card][stream];
}


bool SoundCard::haveOutputPortMeter(int card,int port) const
{
  return output_port_meter[card][port];
}


bool SoundCard::haveTuner(int card,int port) const
{
  return false;
}


void SoundCard::setTunerBand(int card,int port,
				 SoundCard::TunerBand band)
{
}


int SoundCard::tunerFrequency(int card,int port)
{
  return 0;
}


void SoundCard::setTunerFrequency(int card,int port,int freq)
{
}


bool SoundCard::tunerSubcarrier(int card,int port,
				    SoundCard::Subcarrier sub)
{
  return false;
}


int SoundCard::tunerLowFrequency(int card,int port,
				     SoundCard::TunerBand band)
{
  return 0;
}


int SoundCard::tunerHighFrequency(int card,int port,
				      SoundCard::TunerBand band)
{
  return 0;
}


bool SoundCard::inputStreamMeter(int card,int stream,short *level)
{
  if(card>=card_quantity) {
    return false;
  }
  if(stream>=card_input_streams[card]) {
    return false;
  }
  HPICall(HPI_MeterGetPeak(NULL,input_stream_meter_control[card][stream],
			   level));
  return true;
}


bool SoundCard::outputStreamMeter(int card,int stream,short *level)
{
  if(card>=card_quantity) {
    return false;
  }
  if(stream>=card_output_streams[card]) {
    return false;
  }
  HPICall(HPI_MeterGetPeak(NULL,output_stream_meter_control[card][stream],
			   level));
  return true;
}


bool SoundCard::inputPortMeter(int card,int port,short *level)
{
  if(card>=card_quantity) {
    return false;
  }
  if(port>=card_input_ports[card]) {
    return false;
  }
  HPICall(HPI_MeterGetPeak(NULL,input_port_meter_control[card][port],level));
  return true;
}


bool SoundCard::outputPortMeter(int card,int port,short *level)
{
  if(card>=card_quantity) {
    return false;
  }
  if(port>=card_output_ports[card]) {
    return false;
  }
  HPICall(HPI_MeterGetPeak(NULL,output_port_meter_control[card][port],level));
  return true;
}


bool SoundCard::haveInputMode(int card,int stream) const
{
  return false;
}


bool SoundCard::haveOutputMode(int card,int stream) const
{
  return false;
}


bool SoundCard::haveInputPortMux(int card,int port) const
{
  return input_port_mux[card][port];
}


bool SoundCard::queryInputPortMux(int card,int port,SourceNode node) const
{
  switch(node) {
      case SoundCard::LineIn:
	return input_port_mux_type[card][port][0];
	break;
      case SoundCard::AesEbuIn:
	return input_port_mux_type[card][port][1];
	break;
      default:
	return false;
	break;
  }
}


bool SoundCard::haveInputStreamMux(int card,int stream) const
{
  return input_stream_mux[card][stream];
}


int SoundCard::getInputVolume(int card,int stream,int port)
{
  short gain[2];

  HPICall(HPI_VolumeGetGain(NULL,
			    input_stream_volume_control[card][stream][port],
			    gain));
  return gain[0];
}


int SoundCard::getOutputVolume(int card,int stream,int port)
{
  short gain[2];

  HPICall(HPI_VolumeGetGain(NULL,
			    output_stream_volume_control[card][stream][port],
			    gain));
  return gain[0];
}


int SoundCard::getInputLevel(int card,int port)
{
  short gain[2];

  HPICall(HPI_VolumeGetGain(NULL,
			    input_port_level_control[card][port],gain));
  return gain[0];
}


int SoundCard::getOutputLevel(int card,int port)
{
  short gain[2];

  HPICall(HPI_VolumeGetGain(NULL,
			    output_port_level_control[card][port],gain));
  return gain[0];
}



void SoundCard::setInputVolume(int card,int stream,int level)
{
  if(!haveInputVolume(card,stream,0)) {
    return;
  }
  short gain[2];
  gain[0]=level;
  gain[1]=level;
  HPICall(HPI_VolumeSetGain(NULL,
			    input_stream_volume_control[card][stream][0],
			    gain));
}


void SoundCard::setOutputVolume(int card,int stream,int port,int level)
{
  if(!haveOutputVolume(card,stream,port)) {
    return;
  }
  short gain[2];
  gain[0]=level;
  gain[1]=level;
  HPICall(HPI_VolumeSetGain(NULL,
			    output_stream_volume_control[card][stream][port],
			    gain));
}



void SoundCard::fadeOutputVolume(int card,int stream,int port,
				     int level,int length)
{
  if(!haveOutputVolume(card,stream,port)) {
    return;
  }
  short gain[2];

  gain[0]=level;
  gain[1]=level;
  HPICall(HPI_VolumeAutoFadeProfile(NULL,
		       	    output_stream_volume_control[card][stream][port],
				    gain,length,hpi_fade_type));
}


void SoundCard::setInputLevel(int card,int port,int level)
{
  if(!haveInputLevel(card,port)) {
    return;
  }
  short gain[2];
  gain[0]=level;
  gain[1]=level;
  HPICall(HPI_VolumeSetGain(NULL,
			    input_port_level_control[card][port],gain));
}


void SoundCard::setOutputLevel(int card,int port,int level)
{
  if(!haveOutputLevel(card,port)) {
    return;
  }
  short gain[2];

  gain[0]=level;
  gain[1]=level;
  HPICall(HPI_VolumeSetGain(NULL,
			    output_port_level_control[card][port],gain));
}


void SoundCard::setInputMode(int card,int stream,
				 SoundCard::ChannelMode mode)
{

}


void SoundCard::setOutputMode(int card,int stream,
				  SoundCard::ChannelMode mode)
{

}


void SoundCard::setInputStreamVOX(int card,int stream,short gain)
{
  HPICall(HPI_VoxSetThreshold(NULL,input_stream_vox_control[card][stream],
			      gain));
}


bool SoundCard::havePassthroughVolume(int card,int in_port,int out_port)
{
  return passthrough_port_volume[card][in_port][out_port];
}


bool SoundCard::setPassthroughVolume(int card,int in_port,int out_port,
					 int level)
{
  if(!passthrough_port_volume[card][in_port][out_port]) {
    return false;
  }
  short gain[2];
  gain[0]=level;
  gain[1]=level;
  HPICall(HPI_VolumeSetGain(NULL,
		     passthrough_port_volume_control[card][in_port][out_port],
			    gain));
  return true;
}


void SoundCard::clock()
{
  uint16_t error_word;

  for(int i=0;i<card_quantity;i++) {
    for(int j=0;j<HPI_MAX_NODES;j++) {
      if(input_port_aesebu[i][j]) {
	error_word=getInputPortError(i,j);
	if(error_word!=input_port_aesebu_error[i][j]) {
	  input_port_aesebu_error[i][j]=error_word;
	  emit inputPortError(i,j);
	}
      }
    }
  }
}


bool SoundCard::HPICall(uint16_t hpi_err) const
{
  char text[1024];

  if(hpi_err!=0) {
    HPI_GetErrorText(hpi_err,text);
    fprintf(stderr,"SoundCard::HPI Error: %s\n",text);
  }
  return hpi_err==0;
}


void SoundCard::HPIProbe()
{
  uint16_t hpi_adapter_list[HPI_MAX_ADAPTERS];
  uint32_t dummy_serial;
  uint32_t dummy_hpi;
  uint16_t dummy_version;
  uint16_t dummy_type;
  uint16_t l;
  uint16_t type;
  uint16_t index;
  QString str;
  uint16_t hpi_err;

  hpi_fade_type=HPI_VOLUME_AUTOFADE_LOG;
#if HPI_VER < 0x00030600
  hpi_err=HPI_SubSysGetVersion(NULL,&dummy_hpi);
  HPI_SubSysFindAdapters(NULL,(uint16_t *)&card_quantity,
			 hpi_adapter_list,HPI_MAX_ADAPTERS);  
#else
  hpi_err=HPI_SubSysGetVersionEx(NULL,&dummy_hpi);
  hpi_err=HPI_SubSysGetNumAdapters(NULL,&card_quantity);
#endif  // HPI_VER
  for(int i=0;i<card_quantity;i++) {
#if HPI_VER < 0x00030600
    card_index[i]=i;
#else
    hpi_err=HPI_SubSysGetAdapter(NULL,i,card_index+i,hpi_adapter_list+i);
#endif  // HPI_VER
    card_input_ports[i]=0;
    card_output_ports[i]=0;
    card_description[i]=QString().sprintf("AudioScience %04X [%d]",
					  hpi_adapter_list[i],i+1);
    HPICall(HPI_AdapterOpen(NULL,card_index[i]));
    HPICall(HPI_AdapterGetInfo(NULL,card_index[i],
		       &card_output_streams[card_index[i]],
		       &card_input_streams[card_index[i]],
		       &dummy_version,(uint32_t *)&dummy_serial,
			       &dummy_type));
    hpi_info[card_index[i]].setModelNumber(dummy_type);
    hpi_info[card_index[i]].setSerialNumber(dummy_serial);
    hpi_info[card_index[i]].setHpiMajorVersion(dummy_hpi>>8);
    hpi_info[card_index[i]].setHpiMinorVersion(dummy_hpi&255);
    hpi_info[card_index[i]].setDspMajorVersion((dummy_version>>13)&7);
    hpi_info[card_index[i]].setDspMinorVersion((dummy_version>>7)&63);
    hpi_info[card_index[i]].setPcbVersion((char)(((dummy_version>>3)&7)+'A'));
    hpi_info[card_index[i]].setAssemblyVersion(dummy_version&7);
    HPI_AdapterClose(NULL,card_index[i]);
    str=QString(tr("Input Stream"));
    for(int j=0;j<card_input_streams[card_index[i]];j++) {
      input_stream_description[card_index[i]][j]=
	QString().sprintf("%s - %s %d",(const char *)card_description[card_index[i]],
			  (const char *)str,j+1);
    }
    str=QString(tr("Output Stream"));
    for(int j=0;j<card_output_streams[card_index[i]];j++) {
      output_stream_description[card_index[i]][j]=
	QString().sprintf("%s - %s %d",(const char *)card_description[card_index[i]],
			  (const char *)str,j+1);
    }
  }

  //
  // Mixer Initialization
  //
  for(int i=0;i<card_quantity;i++) {
    HPICall(HPI_MixerOpen(NULL,card_index[i],&hpi_mixer[i]));

    //
    // Get Input Ports
    //
    str=QString(tr("Input Port"));
    for(int k=0;k<HPI_MAX_NODES;k++) {    
      if(HPI_MixerGetControl(NULL,hpi_mixer[i],
			     0,0,
			     HPI_DESTNODE_ISTREAM,k,
			     HPI_CONTROL_MULTIPLEXER,
			     &input_stream_volume_control[i][0][k])==0) {
	card_input_ports[i]++;
	input_port_description[i][k]=
	  QString().sprintf("%s - %s %d",(const char *)card_description[i],
			    (const char *)str,
			    card_input_ports[i]);
      }
    }

    //
    // Get Output Ports
    //
    str=QString(tr("Output Port"));
    for(int k=0;k<HPI_MAX_NODES;k++) {
      if(HPI_MixerGetControl(NULL,hpi_mixer[i],
			     HPI_SOURCENODE_OSTREAM,0,
			     HPI_DESTNODE_LINEOUT,k,
			     HPI_CONTROL_VOLUME,
			     &output_stream_volume_control[i][0][k])==0) {
	output_stream_volume[i][0][k]=true;
	card_output_ports[i]++;
	output_port_description[i][k]=
	  QString().sprintf("%s - %s %d",(const char *)card_description[i],
			    (const char *)str,
			    card_output_ports[i]);
      }
    }
    hpi_err=HPI_MixerGetControl(NULL,hpi_mixer[i],
				HPI_SOURCENODE_CLOCK_SOURCE,0,
				0,0,
				HPI_CONTROL_SAMPLECLOCK,
				&clock_source_control[i]);
    /*
    HPI_MixerGetControl(NULL,hpi_mixer[i],
			HPI_SAMPLECLOCK_SOURCE,0,
			0,0,
			HPI_SOURCENODE_CLOCK_SOURCE,
			&clock_source_control[i]);
    */
    for(int j=0;j<card_input_streams[i];j++) {
      if(HPI_MixerGetControl(NULL,hpi_mixer[i],  // VOX Controls
			     0,0,
			     HPI_DESTNODE_ISTREAM,j,
			     HPI_CONTROL_VOX,
			     &input_stream_vox_control[i][j])==0) {
	input_stream_vox[i][j]=true;
      }
      else {
	input_stream_vox[i][j]=false;
      }


      if(input_mux_type[i]) {
	if(HPI_MixerGetControl(NULL,hpi_mixer[i],  // MUX Controls
			       0,0,
			       HPI_DESTNODE_ISTREAM,j,
			       HPI_CONTROL_MULTIPLEXER,
			       &input_mux_control[i][j])==0) {
	  input_stream_mux[i][j]=true;
	  l=0;
	  input_port_mux_type[i][j][0]=false;
	  input_port_mux_type[i][j][1]=false;
	  while(HPI_Multiplexer_QuerySource(NULL,
					    input_mux_control[i][j],
					    l++,&type,&index)==0) {
	    switch(type) {
		case HPI_SOURCENODE_LINEIN:
		  input_port_mux_type[i][j][0]=true;
		  input_mux_index[i][j][0]=index;
		  break;
		case HPI_SOURCENODE_AESEBU_IN:
		  input_port_mux_type[i][j][1]=true;
		  input_mux_index[i][j][1]=index;
		  break;
	    }
	  }
	}
	else {
	  input_stream_mux[i][j]=false;
	}
      }
    }
    for(int j=0;j<card_output_streams[i];j++) {
      for(int k=0;k<HPI_MAX_NODES;k++) {
	if(HPI_MixerGetControl(NULL,hpi_mixer[i],
			       HPI_SOURCENODE_LINEIN,j,
			       HPI_DESTNODE_ISTREAM,k,
			       HPI_CONTROL_VOLUME,
			       &input_stream_volume_control[i][j][k])==0) {
	  input_stream_volume[i][j][k]=true;
	}
	else {
	  input_stream_volume[i][j][k]=false;
	}
	if(HPI_MixerGetControl(NULL,hpi_mixer[i],
			       HPI_SOURCENODE_OSTREAM,j,
			       HPI_DESTNODE_LINEOUT,k,
			       HPI_CONTROL_VOLUME,
			       &output_stream_volume_control[i][j][k])==0) {
	  output_stream_volume[i][j][k]=true;
	}
	else {
	  output_stream_volume[i][j][k]=false;
	}
      }
      //
      // A hack to make the ASI4215 Work with the summed output port
      //
      if(hpi_adapter_list[i]==0x4215) {
	output_stream_volume_control[i][0][4]=
	  output_stream_volume_control[i][0][0];
	output_stream_volume[i][0][4]=true;
	output_stream_volume_control[i][1][4]=
	  output_stream_volume_control[i][1][1];
	output_stream_volume[i][1][4]=true;
	output_stream_volume_control[i][2][4]=
	  output_stream_volume_control[i][2][2];
	output_stream_volume[i][2][4]=true;
	output_stream_volume_control[i][3][4]=
	  output_stream_volume_control[i][3][3];
	output_stream_volume[i][3][4]=true;
      }
      if(HPI_MixerGetControl(NULL,hpi_mixer[i],
			     0,0,
			     HPI_DESTNODE_ISTREAM,j,
			     HPI_CONTROL_METER,
			     &input_stream_meter_control[i][j])==0) {
	input_stream_meter[i][j]=true;
      }
      else {
	input_stream_meter[i][j]=false;
      }
      if(HPI_MixerGetControl(NULL,hpi_mixer[i],
			     HPI_SOURCENODE_OSTREAM,j,
			     0,0,
			     HPI_CONTROL_METER,
			     &output_stream_meter_control[i][j])==0) {
	output_stream_meter[i][j]=true;
      }
      else {
	output_stream_meter[i][j]=false;
      }
    }
    for(int j=0;j<HPI_MAX_NODES;j++) {
      if(HPI_MixerGetControl(NULL,hpi_mixer[i],  // Input Level Controls
			     HPI_SOURCENODE_LINEIN,j,
			     0,0,
			     HPI_CONTROL_LEVEL,
			     &input_port_level_control[i][j])==0) {
	input_port_level[i][j]=true;
      }
      else {
	input_port_level[i][j]=false;
      }
      if(HPI_MixerGetControl(NULL,hpi_mixer[i],  // Output Level Controls
			     0,0,
			     HPI_DESTNODE_LINEOUT,j,
			     HPI_CONTROL_LEVEL,
			     &output_port_level_control[i][j])==0) {
	output_port_level[i][j]=true;
      }
      else {
	output_port_level[i][j]=false;
      }
      if(HPI_MixerGetControl(NULL,hpi_mixer[i],  // Input Port Meter
			     HPI_SOURCENODE_LINEIN,j,
			     0,0,
			     HPI_CONTROL_METER,
			     &input_port_meter_control[i][j])==0) {
	input_port_meter[i][j]=true;
      }
      else {
	input_port_meter[i][j]=false;
      }
      if(HPI_MixerGetControl(NULL,hpi_mixer[i],  // Output Port Meter
			     0,0,
			     HPI_DESTNODE_LINEOUT,j,
			     HPI_CONTROL_METER,
			     &output_port_meter_control[i][j])==0) {
	output_port_meter[i][j]=true;
      }
      else {
	output_port_meter[i][j]=false;
      }
      if(HPI_MixerGetControl(NULL,hpi_mixer[i],  // Input Port AES/EBU
			     HPI_SOURCENODE_AESEBU_IN,j,
			     0,0,
			     HPI_CONTROL_AESEBU_RECEIVER,
			     &input_port_aesebu_control[i][j])==0) {
	input_port_aesebu[i][j]=true;
      }
      else {
	input_port_aesebu[i][j]=false;
      }
      if(!input_mux_type[i]) {
	if(HPI_MixerGetControl(NULL,hpi_mixer[i],  // Input Port Mux
			       HPI_SOURCENODE_LINEIN,j,
			       0,0,
			       HPI_CONTROL_MULTIPLEXER,
			       &input_mux_control[i][j])==0) {
	  input_port_mux[i][j]=true;
	  l=0;
	  input_port_mux_type[i][j][0]=false;
	  input_port_mux_type[i][j][1]=false;
	  while(HPI_Multiplexer_QuerySource(NULL,
					    input_mux_control[i][j],
					    l++,&type,&index)==0) {
	    switch(type) {
		case HPI_SOURCENODE_LINEIN:
		  input_port_mux_type[i][j][0]=true;
		  input_mux_index[i][j][0]=index;
		  break;
		case HPI_SOURCENODE_AESEBU_IN:
		  input_port_mux_type[i][j][1]=true;
		  input_mux_index[i][j][1]=index;
		  break;
	    }
	  }
	}
	else {
	  input_port_mux[i][j]=false;
	}
      }
    }

    //
    // Get The Passthroughs
    //
    for(int j=0;j<HPI_MAX_NODES;j++) {
      for(int k=0;k<HPI_MAX_NODES;k++) {
	if(HPI_MixerGetControl(NULL,hpi_mixer[i],
			       HPI_SOURCENODE_LINEIN,j,
			       HPI_DESTNODE_LINEOUT,k,
			       HPI_CONTROL_VOLUME,
			       &passthrough_port_volume_control[i][j][k])==0) {
	  passthrough_port_volume[i][j][k]=true;
	}
	else {
	  passthrough_port_volume[i][j][k]=false;
	}
      }
    }
  }
  clock_timer=new QTimer(this,"clock_timer");
  connect(clock_timer,SIGNAL(timeout()),this,SLOT(clock()));
  clock_timer->start(METER_INTERVAL);
}
