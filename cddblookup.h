//   cddblookup.h
//
//   A Qt class for accessing the FreeDB CD Database.
//
//   (C) Copyright 2003,2016 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef CDDBLOOKUP_H
#define CDDBLOOKUP_H

#include <QtGui>
#include <QtNetwork>

#include "cddbrecord.h"

//
// Default Settings
//
#define CDDBLOOKUP_DEFAULT_PORT 8880
#define CDDBLOOKUP_DEFAULT_USER "libradio"
#define CDDBLOOKUP_DEFAULT_HOSTNAME "linux"

/**
 * @short Lookup CD Data from the FreeDB CD Database
 * @author Fred Gleason <fredg@paravelsystems.com>
 *
 * This class implements an object for accessing a remote FreeDB CD
 * database server.  
 **/

class CddbLookup : public QObject
{
  Q_OBJECT
  public:
   enum Result {ExactMatch=0,PartialMatch=1,NoMatch=2,
		ProtocolError=3,NetworkError=4};
   CddbLookup(QObject *parent=0,const char *name=0);
   ~CddbLookup();
   void setCddbRecord(CddbRecord *);
   void lookupRecord(QString cdda_dir,QString cdda_dev,QString hostname,
		     Q_UINT16 port=CDDBLOOKUP_DEFAULT_PORT,
		     QString username="",
		     QString appname=PACKAGE_NAME,QString ver=VERSION);

  private slots:
   void connectedData();
   void readyReadData();
   void errorData(QAbstractSocket::SocketError);

  signals:
   void done(CddbLookup::Result);

  private:
   QString DecodeString(QString &str);
   void ParsePair(QString *line,QString *tag,QString *value,int *index);
   int GetIndex(QString *tag);
   bool GetCddaData(QString &cdda_dir,QString &cdda_dev);
   CddbRecord *lookup_record;
   QTcpSocket *lookup_socket;
   int lookup_state;
   QString lookup_username;
   QString lookup_appname;
   QString lookup_appver;
   QString lookup_hostname;
};

#endif  // CDDBLOOKUP_H
