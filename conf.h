// conf.h
//
// The header file for the rconf package
//
//   (C) Copyright 1996-2004,2016 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef CONF_H
#define CONF_H

#define MAX_RETRIES 10
#include <stdlib.h>
#include <stdio.h>

#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtSql>

/* Function Prototypes */
int GetPrivateProfileBool(const char *,const char *,const char *,bool);
int GetPrivateProfileString(const char *,const char *,const char *,char *,
			    const char *,int);
int GetPrivateProfileInt(const char *,const char *,const char *,int);
int GetPrivateProfileHex(const char *,const char *,const char *,int);
double GetPrivateProfileDouble(const char *,const char *,const char *,double);
int GetIni(const char *,const char *,const char *,char *,int);
int GetIniLine(FILE *,char *);
void Prepend(char *,char *);
int IncrementIndex(char *,int);
void StripLevel(char *); 
bool GetLock(const char *);
void ClearLock(const char *);
QString GetPathPart(QString path);
QString GetBasePart(QString path);
QString GetShortDate(QDate);
/**
 * Returns the name of the weekday in english regardless of the locale
 * configured.
 * @param weekday Integer value for the weekday; 1 = "Mon", 2 = "Tue", 
 * ... 7 = "Sun".  If the value is out of range 1 is defaulted to.
 **/
QString GetShortDayNameEN(int weekday);
QFont::Weight GetFontWeight(QString);
bool Detach();
bool Bool(QString);
QString YesNo(bool);
QHostAddress GetHostAddr();
QString GetDisplay(bool strip_point=false);
/*
bool DoesRowExist(QString table,QString name,QString test,
		   QSqlDatabase *db=0);
bool DoesRowExist(QString table,QString name,unsigned test,
		   QSqlDatabase *db=0);
QVariant GetSqlValue(QString table,QString name,QString test,
		      QString param,QSqlDatabase *db=0,bool *valid=0);
QVariant GetSqlValue(QString table,QString name,unsigned test,
		      QString param,QSqlDatabase *db=0,bool *valid=0);
bool IsSqlNull(QString table,QString name,QString test,
		QString param,QSqlDatabase *db=0);
bool IsSqlNull(QString table,QString name,unsigned test,
		QString param,QSqlDatabase *db=0);
*/
QString GetTimeLength(int mseconds,bool leadzero=false,bool tenths=true);
int SetTimeLength(QString string);
bool Copy(QString srcfile,QString destfile);
#ifndef WIN32
bool WritePid(QString dirname,QString filename,int owner=-1,int group=-1);
void DeletePid(QString dirname,QString filename);
bool CheckPid(QString dirname,QString filename);
pid_t GetPid(QString pidfile);
#endif  // WIN32
QString GetHomeDir(bool *found=0);
bool TimeSynced();
QString TruncateAfterWord(QString str,int word,bool add_dots=false);
QString HomeDir();
QString TempDir();
QString TimeZoneName(const QDateTime &datetime);

#endif   // CONF_H
