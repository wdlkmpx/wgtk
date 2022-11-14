include ../config.mk

LIB_STATIC = libwgtk.a

# ls *.c | sed "s%\.c%.o%g" | tr "\n" " " >> Makefile
OBJS = w_gtk.o \
	w_gtk_combobox.o \
	w_gtk_listbox.o \
	w_gtk_menu.o \
	w_gtk_text_view.o \
	w_gtk_compat2.o

#CPPFLAGS += -I..
CFLAGS += -I..

CFLAGS += $(GTK_CFLAGS)
LIBS   += $(GTK_LIBS)

all: $(LIB_STATIC)

#-------------------------------------

$(LIB_STATIC): $(OBJS)
	$(AR) rcs $(LIB_STATIC) $(OBJS)

w_gtk.o: w_gtk.c w_gtk.h
	$(CC) $(CFLAGS) -c -o w_gtk.o w_gtk.c

w_gtk_combo.o: w_gtk_combo.c w_gtk.h
	$(CC) $(CFLAGS) -c -o w_gtk_combo.o w_gtk_combo.c

w_gtk_text_view.o: w_gtk_text_view.c w_gtk.h
	$(CC) $(CFLAGS) -c -o w_gtk_text_view.o w_gtk_text_view.c

#-------------------------------------

strip:

distclean: clean

clean:
	rm -f *.o *~ *.a *.so

install:

install-strip: install

uninstall:
	rm -f $(DESTDIR)$(libdir)/$(LIB_STATIC)*
