## automake.am
##
## Top level automake.am for SoundPanel
##
## by Fred Gleason <fredg@paravelsystems.com>
##
## Use automake to process this into a Makefile.in
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License version 2 as
##   published by the Free Software Foundation.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##

AM_CPPFLAGS = -Wall -DQT3_SUPPORT @QT4_CFLAGS@
LIBS = 
MOC = @MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

bin_PROGRAMS = soundpanel
dist_soundpanel_SOURCES = soundpanel.cpp soundpanel.h defaults.h globals.h\
                          edit_button.cpp edit_button.h\
                          snd_properties.cpp snd_properties.h\
                          edit_devices.cpp edit_devices.h\
                          record_audio.cpp record_audio.h\
                          ripper_dialog.cpp ripper_dialog.h\
                          edit_ripper.cpp edit_ripper.h\
                          edit_editor.cpp edit_editor.h\
                          search_array.cpp search_array.h\
                          panelbox.cpp panelbox.h\
                          panel_name.cpp panel_name.h\
                          panel.cpp panel.h\
                          about.cpp about.h\
                          array_button.cpp array_button.h\
                          edit_general.cpp edit_general.h\
                          soundcard.cpp soundcard.h\
                          playstream.cpp playstream.h\
                          recordstream.cpp recordstream.h\
                          transportbutton.cpp transportbutton.h\
                          playmeter.cpp playmeter.h\
                          segmeter.cpp segmeter.h\
                          wavefile.cpp wavefile.h\
                          wavedata.cpp wavedata.h\
                          information.cpp information.h\
                          ringbuffer.cpp ringbuffer.h\
                          audiosettings.cpp audiosettings.h\
                          audiosettingsdialog.cpp audiosettingsdialog.h\
                          pushbutton.cpp pushbutton.h\
                          cardselector.cpp cardselector.h\
                          stereometer.cpp stereometer.h\
                          cdplayer.cpp cdplayer.h\
                          cddbrecord.cpp cddbrecord.h\
                          conf.cpp conf.h\
                          cddblookup.cpp cddblookup.h\
                          profile.cpp profile.h\
                          profilesection.cpp profilesection.h\
                          profileline.cpp profileline.h

nodist_soundpanel_SOURCES = moc_soundpanel.cpp moc_edit_button.cpp\
                            moc_snd_properties.cpp moc_edit_devices.cpp\
                            moc_record_audio.cpp moc_about.cpp\
                            moc_ripper_dialog.cpp moc_edit_ripper.cpp\
                            moc_edit_editor.cpp moc_search_array.cpp\
                            moc_panelbox.cpp moc_panel_name.cpp\
                            moc_array_button.cpp moc_edit_general.cpp\
                            moc_soundcard.cpp moc_playstream.cpp\
                            moc_recordstream.cpp moc_transportbutton.cpp\
                            moc_playmeter.cpp moc_segmeter.cpp\
                            moc_audiosettingsdialog.cpp\
                            moc_pushbutton.cpp moc_cardselector.cpp\
                            moc_stereometer.cpp moc_cdplayer.cpp\
                            moc_cddblookup.cpp

soundpanel_LDADD = -lQtGui -lQt3Support -lhpi -lid3 @FLAC_LIBS@ @LIBVORBIS@ @QT4_LIBS@

rpm:	dist
	cp soundpanel-$(VERSION).tar.gz $(RPM_ROOT)/SOURCES/
	$(RPMBUILD) -ba --target $(ARCH)-$(VENDOR)-linux soundpanel.spec
	mv $(RPM_ROOT)/RPMS/$(ARCH)/soundpanel-$(VERSION)-@RPM_RELEASE@.$(ARCH).rpm $(top_srcdir)/
	mv $(RPM_ROOT)/SRPMS/soundpanel-$(VERSION)-@RPM_RELEASE@.src.rpm $(top_srcdir)/
	rm $(RPM_ROOT)/SOURCES/soundpanel-$(VERSION).tar.gz
	rm -rf $(RPM_ROOT)/BUILD/soundpanel-$(VERSION)

slack:	all
	./make_slack

slax:	all
	rm -f soundpanel-$(VERSION)-$(ARCH)-$(RPM_RELEASE).mo
	./make_slack
	tgz2mo soundpanel-$(VERSION)-$(ARCH)-$(RPM_RELEASE).tgz soundpanel-$(VERSION)-$(ARCH)-$(RPM_RELEASE).mo

EXTRA_DIST = TODO autogen.sh soundpanel.spec.in soundpanel-48x48.xpm\
             soundpanel-16x16.xpm soundpanel-22x22.xpm soundpanel-32x32.xpm\
             soundpanel_soundpanel.desktop make_slack.in slack-desc.in\
             get_target.sh get_distro.sh

CLEANFILES = *~ *.tar.gz *.tgz *.mo *.rpm
DISTCLEANFILES = soundpanel.spec make_slack slack-desc
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 *.tar.gz moc_*\
                       depcomp install-sh missing mkinstalldirs
