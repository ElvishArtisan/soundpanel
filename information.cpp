// information.cpp
//
// A Container Class for AudioScience HPI Adapter Info
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: information.cpp,v 1.2 2012/09/10 11:57:29 cvs Exp $
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

#include <information.h>


Information::Information()
{
  clear();
}


unsigned Information::modelNumber() const
{
  return model_number;
}


void Information::setModelNumber(unsigned model)
{
  model_number=model;
}


unsigned Information::serialNumber() const
{
  return serial_number;
}


void Information::setSerialNumber(unsigned num)
{
  serial_number=num;
}


unsigned Information::hpiMajorVersion() const
{
  return hpi_major_version;
}


void Information::setHpiMajorVersion(unsigned ver)
{
  hpi_major_version=ver;
}


unsigned Information::hpiMinorVersion() const
{
  return hpi_minor_version;
}


void Information::setHpiMinorVersion(unsigned ver)
{
  hpi_minor_version=ver;
}


unsigned Information::dspMajorVersion() const
{
  return dsp_major_version;
}


void Information::setDspMajorVersion(unsigned ver)
{
  dsp_major_version=ver;
}


unsigned Information::dspMinorVersion() const
{
  return dsp_minor_version;
}


void Information::setDspMinorVersion(unsigned ver)
{
  dsp_minor_version=ver;
}


char Information::pcbVersion() const
{
  return pcb_version;
}


void Information::setPcbVersion(char ver)
{
  pcb_version=ver;
}


unsigned Information::assemblyVersion() const
{
  return assembly_version;
}


void Information::setAssemblyVersion(unsigned ver)
{
  assembly_version=ver;
}


void Information::clear()
{
  model_number=0;
  serial_number=0;
  dsp_major_version=0;
  dsp_minor_version=0;
  pcb_version='0';
  assembly_version=0;
}
