include ../config.mk

APPS  = w_gtk_combo_example w_gtk_menu_example
#APPS  = w_gtk_combo_example

WGTK = ../0libwgtk
WGTK_CFLAGS = -I../0libwgtk
WGTK_LIBS   = -L../0libwgtk -lwgtk

ifndef GTK_CFLAGS
$(error GTK_CFLAGS is not defined)
endif

ifneq (,$(findstring gtk-3.0,$(GTK_CFLAGS)))
APPS = w_gtk_menu_example2
endif

LDFLAGS += 
#CPPFLAGS += -I..
CFLAGS += -I.. $(GTK_CFLAGS) $(WGTK_CFLAGS)
LIBS   += $(GTK_LIBS) $(WGTK_LIBS)

all: $(APPS)

w_gtk_combo_example: $(WOBJ) w_gtk_combo_example.o
	$(CC) $(CFLAGS) $(WOBJ) w_gtk_combo_example.o -o w_gtk_combo_example $(LDFLAGS) $(LIBS)

w_gtk_menu_example: $(WOBJ) w_gtk_menu_example.o
	$(CC) $(CFLAGS) $(WOBJ) w_gtk_menu_example.o -o w_gtk_menu_example $(LDFLAGS) $(LIBS)

w_gtk_menu_example2: $(WOBJ) w_gtk_menu_example2.o
	$(CC) $(CFLAGS) $(WOBJ) w_gtk_menu_example2.o -o w_gtk_menu_example2 $(LDFLAGS) $(LIBS)

strip: all
	$(STRIP) $(APPS)

distclean: clean

clean:
	rm -f *.o *~ $(APPS)
#	make -C $(WGTK) clean

install:

install-strip: strip

uninstall:


