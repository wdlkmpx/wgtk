#!/bin/sh

GTK1_CFLAGS=`gtk-config --cflags`
if [ -n "${GTK1_CFLAGS}" ] ; then
    CFLAGS=${GTK1_CFLAGS} geany -g gtk1.c.tags /usr/include/gtk-1.2/gtk/gtk.h
fi

GTK2_CFLAGS=`pkg-config --cflags gtk+-2.0`
if [ -n "${GTK2_CFLAGS}" ] ; then
    CFLAGS=${GTK2_CFLAGS} geany -g gtk2.c.tags /usr/include/gtk-2.0/gtk/gtk.h
fi

GTK3_CFLAGS=`pkg-config --cflags gtk+-3.0`
if [ -n "${GTK3_CFLAGS}" ] ; then
    CFLAGS=${GTK3_CFLAGS} geany -g gtk3.c.tags /usr/include/gtk-3.0/gtk/gtk.h
fi
