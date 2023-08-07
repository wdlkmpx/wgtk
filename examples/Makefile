include ../config.mk

APPS  = 

LIBWGTK_DIR = ../0libwgtk
LIBWGTK_CFLAGS = -I$(LIBWGTK_DIR)
#LIBWGTK_LIBS  = -L$(LIBWGTK_DIR) -lwgtk

#ifndef GTK_CFLAGS
#$(error GTK_CFLAGS is not defined)
#endif

#ifneq (,$(findstring gtk-3.0,$(GTK_CFLAGS)))
#APPS += w_gtk_menu_example2
#endif

ifdef GTK1_LIBS
APPS += w_gtk_combo_example.gtk1 w_gtk_menu_example.gtk1
LIBWGTK1_LIBS = -L$(LIBWGTK_DIR) -lwgtk_gtk1
endif

ifdef GTK2_LIBS
APPS += w_gtk_combo_example.gtk2 w_gtk_menu_example.gtk2
LIBWGTK2_LIBS = -L$(LIBWGTK_DIR) -lwgtk_gtk2
endif

ifdef GTK3_LIBS
APPS += w_gtk_combo_example.gtk3 w_gtk_menu_example.gtk3 w_gtk_menu_example2.gtk3
LIBWGTK3_LIBS = -L$(LIBWGTK_DIR) -lwgtk_gtk3
endif

#ifdef GTK4_LIBS
#APPS += w_gtk_combo_example.gtk4 w_gtk_menu_example.gtk4
#LIBWGTK4_LIBS = -L$(LIBWGTK_DIR) -lwgtk_gtk4
#endif

CFLAGS += -I.. $(LIBWGTK_CFLAGS)
#LIBS   += $(LIBWGTK_LIBS)

all: $(APPS)

#---------------------------------------------------------------------------

ifdef GTK1_LIBS
%.gtk1:
	$(CC) $(CFLAGS) $(GTK1_CFLAGS) -o $@ $(subst .gtk1,.c, $@) $(LDFLAGS) $(LIBS) $(GTK1_LIBS) $(LIBWGTK1_LIBS)
endif

ifdef GTK2_LIBS
%.gtk2:
	$(CC) $(CFLAGS) $(GTK2_CFLAGS) -o $@ $(subst .gtk2,.c, $@) $(LDFLAGS) $(LIBS) $(GTK2_LIBS) $(LIBWGTK2_LIBS)
endif

ifdef GTK3_LIBS
%.gtk3:
	$(CC) $(CFLAGS) $(GTK3_CFLAGS) -o $@ $(subst .gtk3,.c, $@) $(LDFLAGS) $(LIBS) $(GTK3_LIBS) $(LIBWGTK3_LIBS)
endif

ifdef GTK4_LIBS
%.gtk4:
	$(CC) $(CFLAGS) $(GTK4_CFLAGS) -o $@ $(subst .gtk4,.c, $@) $(LDFLAGS) $(LIBS) $(GTK4_LIBS) $(LIBWGTK4_LIBS)
endif

#---------------------------------------------------------------------------

strip: all
	$(STRIP) $(APPS)

distclean: clean

clean:
	rm -f *.o *~ $(APPS)
#	make -C $(LIBWGTK_DIR) clean

install:

install-strip: strip

uninstall:


