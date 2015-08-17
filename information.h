// information.h
//
// A Container Class for AudioScience HPI Adapter Info
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: information.h,v 1.2 2012/09/10 11:57:29 cvs Exp $
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

#ifndef INFORMATION_H
#define INFORMATION_H


class Information
{
 public:
  Information();
  unsigned modelNumber() const;
  void setModelNumber(unsigned model);
  unsigned serialNumber() const;
  void setSerialNumber(unsigned num);
  unsigned hpiMajorVersion() const;
  void setHpiMajorVersion(unsigned ver);
  unsigned hpiMinorVersion() const;
  void setHpiMinorVersion(unsigned ver);
  unsigned dspMajorVersion() const;
  void setDspMajorVersion(unsigned ver);
  unsigned dspMinorVersion() const;
  void setDspMinorVersion(unsigned ver);
  char pcbVersion() const;
  void setPcbVersion(char ver);
  unsigned assemblyVersion() const;
  void setAssemblyVersion(unsigned ver);
  void clear();

 private:
  unsigned model_number;
  unsigned serial_number;
  unsigned hpi_major_version;
  unsigned hpi_minor_version;
  unsigned dsp_major_version;
  unsigned dsp_minor_version;
  char pcb_version;
  unsigned assembly_version;
};


#endif  // INFORMATION_H

