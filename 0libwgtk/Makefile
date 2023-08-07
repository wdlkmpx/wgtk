include ../config.mk

OBJS = w_gtk \
	w_gtk_combobox \
	w_gtk_compat2 \
	w_gtk_compat4 \
	w_gtk_listbox \
	w_gtk_menu \
	w_gtk_text_view

ifdef GTK1_LIBS
LIB_WGTK1 = libwgtk_gtk1.a
OBJS_WGTK1 = $(addsuffix .gtk1.o, $(OBJS))
endif

ifdef GTK2_LIBS
LIB_WGTK2 = libwgtk_gtk2.a
OBJS_WGTK2 = $(addsuffix .gtk2.o, $(OBJS))
endif

ifdef GTK3_LIBS
LIB_WGTK3 = libwgtk_gtk3.a
OBJS_WGTK3 = $(addsuffix .gtk3.o, $(OBJS))
endif

ifdef GTK4_LIBS
LIB_WGTK4 = libwgtk_gtk4.a
OBJS_WGTK4 = $(addsuffix .gtk4.o, $(OBJS))
endif

CFLAGS += -I..

all: $(LIB_WGTK1) $(LIB_WGTK2) $(LIB_WGTK3) $(LIB_WGTK4)

#-------------------------------------

ifdef GTK1_LIBS
$(LIB_WGTK1): $(OBJS_WGTK1)
	$(AR) rcs $(LIB_WGTK1) $(OBJS_WGTK1)

%.gtk1.o:
	$(CC) $(CFLAGS) $(GTK1_CFLAGS) -c -o $@ $(subst .gtk1.o,.c, $@)
endif

#-------------------------------------

ifdef GTK2_LIBS
$(LIB_WGTK2): $(OBJS_WGTK2)
	$(AR) rcs $(LIB_WGTK2) $(OBJS_WGTK2)

%.gtk2.o:
	$(CC) $(CFLAGS) $(GTK2_CFLAGS) -c -o $@ $(subst .gtk2.o,.c, $@)
endif

#-------------------------------------

ifdef GTK3_LIBS
$(LIB_WGTK3): $(OBJS_WGTK3)
	$(AR) rcs $(LIB_WGTK3) $(OBJS_WGTK3)

%.gtk3.o:
	$(CC) $(CFLAGS) $(GTK3_CFLAGS) -c -o $@ $(subst .gtk3.o,.c, $@)
endif

#-------------------------------------

ifdef GTK4_LIBS
$(LIB_WGTK4): $(OBJS_WGTK4)
	$(AR) rcs $(LIB_WGTK4) $(OBJS_WGTK4)

%.gtk4.o:
	$(CC) $(CFLAGS) $(GTK4_CFLAGS) -c -o $@ $(subst .gtk4.o,.c, $@)
endif

#-------------------------------------

strip:

distclean: clean

clean:
	rm -f *.o *~ *.a *.so

install:

install-strip: install

uninstall:
	rm -f $(DESTDIR)$(libdir)/libwgtk_*.a
