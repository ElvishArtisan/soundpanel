dnl configure.in
dnl
dnl   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
dnl
dnl   This program is free software; you can redistribute it and/or modify
dnl   it under the terms of the GNU General Public License as
dnl   published by the Free Software Foundation; either version 2 of
dnl   the License, or (at your option) any later version.
dnl
dnl   This program is distributed in the hope that it will be useful,
dnl   but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl   GNU General Public License for more details.
dnl
dnl   You should have received a copy of the GNU General Public
dnl   License along with this program; if not, write to the Free Software
dnl   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
dnl
dnl 
dnl Use autoconf to process this into a configure script

AC_INIT(soundpanel,0.9.9,Fred Gleason <fredg@paravelsystems.com>)
AM_INIT_AUTOMAKE(soundpanel,0.9.9)
AC_SUBST(RPM_RELEASE,1)

#
# Some Fully Resolved Install Paths
# (for the packaging system and friends)
#
if test ${prefix} = "NONE" ; then
  AC_SUBST(LOCAL_PREFIX,/usr/local)
else
  AC_SUBST(LOCAL_PREFIX,${prefix})
fi

#
# Basic Compiler Checks
#
AC_PROG_CXX
AC_LANG(C++)

#
# Option Switches
#
AC_ARG_ENABLE(flac,[  --disable-flac          disable FLAC encode/decode support],
		      [FLAC_DISABLED=yes],[])
AC_ARG_WITH(debug,[  --with-debug            include debugging symbols in RPMs],[].[STRIP=yes])

#
# Determine the target architecture
#
AR_GCC_TARGET()
AC_SUBST(VENDOR,$ar_gcc_distro)
AC_SUBST(ARCH,$ar_gcc_arch)

#
# Determine Distro
#
AR_GET_DISTRO()

#
# Check for Qt
#
PKG_CHECK_MODULES(QT4,QtGui QtCore QtNetwork Qt3Support,[],[AC_MSG_ERROR([*** Qt4 not found ***])])
AC_CHECK_PROG(MOC_NAME,moc-qt4,[moc-qt4],[moc])
AC_SUBST(MOC,$MOC_NAME)

#
# Check for OggVorbis
#
AC_CHECK_HEADER(vorbis/vorbisfile.h,[VORBIS_FOUND=yes],[])
if test $VORBIS_FOUND = yes ; then
  AC_DEFINE(HAVE_VORBIS)
  AC_SUBST(LIBVORBIS,"-logg -lvorbis -lvorbisenc -lvorbisfile")
else
  AC_SUBST(LIBVORBIS,"")
fi

#
# Check for FLAC
#
if test -z $FLAC_DISABLED ; then
  AC_CHECK_HEADER(FLAC++/encoder.h,[FLAC_FOUND=yes],[])
  if test $FLAC_FOUND ; then
    AC_SUBST(FLAC_LIBS,"-lFLAC -lFLAC++")
    AC_DEFINE(HAVE_FLAC)
  else
    AC_SUBST(FLAC_LIBS,"")
  fi
  AC_CHECK_LIB(FLAC,FLAC__metadata_get_tags,[FLAC_METADATA_FOUND=yes],[])
fi

#
# Debug Build Configuration
#
if test -n $STRIP ; then
  AC_SUBST(STRIP_DEBUG,yes)
else
  AC_SUBST(STRIP_DEBUG,"")
fi

#
# Configure RPM Build
#
AC_CHECK_PROG(RPMBUILD_FOUND,rpmbuild,[yes],[])
if test -z $RPMBUILD_FOUND ; then
  AC_SUBST(RPMBUILD,rpm)
else
  AC_SUBST(RPMBUILD,rpmbuild)
fi
if test -d /usr/src/redhat ; then
  AC_SUBST(RPM_ROOT,/usr/src/redhat)
  AC_SUBST(VENDOR,redhat)
else
  AC_SUBST(RPM_ROOT,/usr/src/packages)
  AC_SUBST(VENDOR,suse)
fi 


AC_OUTPUT(Makefile \
          make_slack \
          slack-desc \
          soundpanel.spec \
)
chmod 755 make_slack
