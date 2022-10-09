#!/bin/sh
# This is free and unencumbered software released into the public domain.
# For more information, please refer to <http://unlicense.org/>

infile="$1"

if [ -z "$infile" ] ; then
	echo "Syntax: $0 <targetfile>"
	echo
	echo "Will use <targetfile>.in as the source"
	exit 1
fi

if [ -z "$VERSION" ] ; then
	if [ -f config.sh ] ; then
		. ./config.sh
	elif [ -f ../config.sh ] ; then
		. ../config.sh
	elif [ -f ../../config.sh ] ; then
		. ../../config.sh
	fi
fi

if [ -f ${infile}.in ] ; then
	echo "Processing ${infile}.in > ${infile}"
	sed -e "s:@VERSION@:${VERSION}:g" \
		-e "s:@PACKAGE@:${PACKAGE}:g" \
		-e "s:@PACKAGE_URL@:${PACKAGE_URL}:g" \
		-e "s:@prefix@:${prefix}:g" \
		-e "s:@exec_prefix@:${exec_prefix}:g" \
		-e "s:@libdir@:${libdir}:g" \
		-e "s:@bindir@:${bindir}:g" \
		-e "s:@sbindir@:${sbindir}:g" \
		-e "s:@libexecdir@:${libexecdir}:g" \
		-e "s:@includedir@:${includedir}:g" \
		-e "s:@datadir@:${datadir}:g" \
		-e "s:@localstatedir@:${localstatedir}:g" \
		-e "s:@sysconfdir@:${sysconfdir}:g" \
		-e "s:@docdir@:${docdir}:g" \
		-e "s:@infodir@:${infodir}:g" \
		-e "s:@mandir@:${mandir}:g" \
		-e "s:@localedir@:${localedir}:g" \
		-e "s:@runstatedir@:${runstatedir}:g" \
		-e "s:@CMAKE_INSTALL_PREFIX@:${prefix}:g" \
		-e "s:@CMAKE_INSTALL_FULL_BINDIR@:${prefix}:g" \
		-e "s:@CMAKE_INSTALL_FULL_LIBDIR@:${libdir}:g" \
		-e "s:@CMAKE_INSTALL_FULL_INCLUDEDIR@:${includedir}:g" \
		-e "s:@PROJECT_VERSION@:${VERSION}:g" \
		${infile}.in > ${infile}
else
	echo "WARNING: ${infile}.in doesn't exist"
	printf "" > ${infile}
fi

