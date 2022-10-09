include ../config.mk

APPS  = w_gtk_combo_example w_gtk_menu_example w_gtk_menu_example2
WGTK = ../0libwgtk

WOBJ = $(WGTK)/w_gtk.o

ifndef GTK_CFLAGS
$(error GTK_CFLAGS is not defined)
endif

LDFLAGS += 
#CPPFLAGS += -I..
CFLAGS += -I.. -I$(WGTK) $(GTK_CFLAGS)
LIBS   += $(GTK_LIBS)

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
	rm -f *.o *~ $(APPS) $(WGTK)/*.o

install:

install-strip: strip

uninstall:


